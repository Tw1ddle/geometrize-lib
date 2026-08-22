#include "model.h"

#include <algorithm>
#include <atomic>
#include <cassert>
#include <condition_variable>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "bitmap/bitmap.h"
#include "commonutil.h"
#include "core.h"
#include "gpu/opencl_energy_evaluator.h"
#include "rasterizer/rasterizer.h"
#include "shape/shape.h"
#include "shaperesult.h"
#include "shape/shapetypes.h"

namespace
{

bool defaultAddShapePrecondition(
    const double lastScore,
    const double newScore,
    const geometrize::Shape&,
    const std::vector<geometrize::Scanline>&,
    const geometrize::rgba&,
    const geometrize::Bitmap&,
    const geometrize::Bitmap&,
    const geometrize::Bitmap&)
{
    return newScore < lastScore; // Adds the shape if the score improved (that is: the difference decreased)
}

struct GpuHillClimbLane
{
    std::vector<std::vector<geometrize::Scanline>> initialScanlines;
    std::vector<double> initialScores;
    std::vector<geometrize::State> mutationStates;
    std::vector<std::vector<geometrize::Scanline>> mutationScanlines;
    std::vector<geometrize::commonutil::RandomGeneratorState> mutationRandomStates;
    std::vector<double> mutationScores;
    std::uint32_t age{0U};
    bool initialReady{false};
    bool initialScoresReady{false};
    bool ready{false};
    bool scoreReady{false};
    bool done{false};
};

std::uint32_t gpuSpeculationLimit()
{
    const char* value{std::getenv("GEOMETRIZE_GPU_SPECULATION")};
    if(value == nullptr) {
        return 1U;
    }
    char* end{nullptr};
    const unsigned long parsed{std::strtoul(value, &end, 10)};
    if(end != value && *end == '\0' && parsed >= 1UL && parsed <= 16UL) {
        return static_cast<std::uint32_t>(parsed);
    }
    return 1U;
}

std::uint32_t gpuSpeculationWindow(
        const std::uint32_t age,
        const std::uint32_t remaining,
        const std::size_t activeLaneCount)
{
    std::uint32_t desired{1U};
    if(age >= 24U) {
        desired = 4U;
    } else if(age >= 8U) {
        desired = 2U;
    }
    desired = (std::min)(desired, gpuSpeculationLimit());
    if(activeLaneCount != 0U) {
        desired = (std::min)(desired, static_cast<std::uint32_t>((std::max)(
            std::size_t{1U}, std::size_t{64U} / activeLaneCount)));
    }
    return (std::min)(desired, remaining);
}

void sortGpuScanlines(std::vector<geometrize::Scanline>& lines)
{
    std::sort(lines.begin(), lines.end(), [](const geometrize::Scanline& a, const geometrize::Scanline& b) {
        return a.y < b.y || (a.y == b.y && (a.x1 < b.x1 || (a.x1 == b.x1 && a.x2 < b.x2)));
    });
}

struct GpuHillClimbCoordinator
{
    explicit GpuHillClimbCoordinator(const std::size_t laneCount) : lanes(laneCount) {}

    std::mutex mutex;
    std::condition_variable condition;
    std::vector<GpuHillClimbLane> lanes;
    std::exception_ptr exception;
    bool abort{false};
};

class CoordinatorAbortGuard
{
public:
    explicit CoordinatorAbortGuard(GpuHillClimbCoordinator& coordinator) : m_coordinator{coordinator} {}

    ~CoordinatorAbortGuard()
    {
        if(!m_active) {
            return;
        }
        {
            std::lock_guard<std::mutex> lock{m_coordinator.mutex};
            m_coordinator.abort = true;
        }
        m_coordinator.condition.notify_all();
    }

    void dismiss() { m_active = false; }

private:
    GpuHillClimbCoordinator& m_coordinator;
    bool m_active{true};
};

}

namespace geometrize
{

class Model::ModelImpl
{
public:
    ModelImpl(const geometrize::Bitmap& target) :
        m_target{target},
        m_current{target.getWidth(), target.getHeight(), geometrize::commonutil::getAverageImageColor(m_target)},
        m_lastScore{geometrize::core::differenceFull(m_target, m_current)},
        m_baseRandomSeed{0U},
        m_randomSeedOffset{0U}
    {}

    ModelImpl(const geometrize::Bitmap& target, const geometrize::Bitmap& initial) :
        m_target{target},
        m_current{initial},
        m_lastScore{geometrize::core::differenceFull(m_target, m_current)},
        m_baseRandomSeed{0U},
        m_randomSeedOffset{0U}
    {
        assert(m_target.getWidth() == m_current.getWidth());
        assert(m_target.getHeight() == m_current.getHeight());
    }

    ~ModelImpl() = default;
    ModelImpl& operator=(const ModelImpl&) = delete;
    ModelImpl(const ModelImpl&) = delete;

    void reset(const geometrize::rgba backgroundColor)
    {
        m_current.fill(backgroundColor);
        m_lastScore = geometrize::core::differenceFull(m_target, m_current);
        if(m_gpuEvaluator && !m_gpuEvaluator->resetCurrent(m_current)) {
            m_gpuEvaluator.reset();
        }
    }

    std::int32_t getWidth() const
    {
        return m_target.getWidth();
    }

    std::int32_t getHeight() const
    {
        return m_target.getHeight();
    }

    std::vector<geometrize::State> getHillClimbState(
            const std::function<std::shared_ptr<geometrize::Shape>(void)> shapeCreator,
            const std::uint8_t alpha,
            const std::uint32_t shapeCount,
            const std::uint32_t maxShapeMutations,
            std::uint32_t maxThreads,
            const geometrize::core::EnergyFunction energyFunction,
            const bool allowGpuBatching)
    {
        resyncIfDirty();

        // Ensure that the maximum number of threads is a sane value
        if(maxThreads == 0) {
            maxThreads = std::thread::hardware_concurrency();
            if(maxThreads == 0) {
                assert(0 && "Failed to get the number of concurrent threads supported by the implementation");
                maxThreads = defaultMaxThreads;
            }
        }

        std::vector<std::uint32_t> seeds(maxThreads);
        for(std::uint32_t& seed : seeds) {
            seed = m_baseRandomSeed + m_randomSeedOffset++;
        }

        const std::uint64_t batchSize{static_cast<std::uint64_t>(maxThreads) * (static_cast<std::uint64_t>(shapeCount) + 2ULL)};
        const std::uint64_t estimatedEvaluationCount{
            static_cast<std::uint64_t>(maxThreads)
            * (static_cast<std::uint64_t>(shapeCount) + static_cast<std::uint64_t>(maxShapeMutations) + 2ULL)};
        const std::uint64_t imagePixels{static_cast<std::uint64_t>(m_target.getWidth()) * static_cast<std::uint64_t>(m_target.getHeight())};
        const bool enoughGpuWork{
            batchSize >= minimumGpuBatchSize
            && imagePixels != 0U
            && estimatedEvaluationCount >= (minimumGpuWork / (std::min)(imagePixels, minimumGpuWork))};
        if(allowGpuBatching && !energyFunction && enoughGpuWork
                && (!m_gpuInitializationAttempted || (m_gpuEvaluator && m_gpuEvaluator->isAvailable()))) {
            std::vector<geometrize::State> gpuStates;
            if(getGpuHillClimbStates(shapeCreator, alpha, shapeCount, maxShapeMutations, seeds, gpuStates)) {
                return gpuStates;
            }
            const std::string reason{m_gpuEvaluator ? m_gpuEvaluator->getLastError() : "OpenCL is unavailable"};
            std::cerr << "[Geometrize GPU] Falling back to CPU: " << reason << std::endl;
            m_gpuEvaluator.reset();
        }

        std::vector<std::future<geometrize::State>> futures{maxThreads};
        for(std::uint32_t i = 0; i < futures.size(); i++) {
            std::future<geometrize::State> handle{std::async(std::launch::async, [&](const std::uint32_t seed, const double lastScore) {
                // Ensure that the results of the random generation are the same between tasks with identical settings
                // The RNG is thread-local and std::async may use a thread pool (which is why this is necessary)
                // Note this implementation requires maxThreads to be the same between tasks for each task to produce the same results.
                geometrize::commonutil::seedRandomGenerator(seed);

                if(energyFunction) {
                    geometrize::Bitmap buffer{m_current};
                    return core::bestHillClimbState(shapeCreator, alpha, shapeCount, maxShapeMutations, m_target, m_current, buffer, lastScore, energyFunction);
                }
                geometrize::Bitmap buffer{m_current};
                return core::bestHillClimbState(shapeCreator, alpha, shapeCount, maxShapeMutations, m_target, m_current, buffer, lastScore);
            }, seeds[i], m_lastScore)};
            futures[i] = std::move(handle);
        }

        std::vector<geometrize::State> states;

        for(auto& f : futures) {
            try {
                states.emplace_back(f.get());
            } catch(std::exception& e) {
                assert(0 && "Encountered exception when getting hill climb state");
                std::cout << e.what() << std::endl;
                throw e;
            } catch (...) {
                assert(0 && "Encountered exception when getting hill climb state");
                throw;
            }
        }
        return states;
    }

    bool ensureGpuEvaluator()
    {
        if(!m_gpuInitializationAttempted) {
            m_gpuInitializationAttempted = true;
            m_gpuEvaluator = std::unique_ptr<geometrize::gpu::OpenClEnergyEvaluator>(
                new geometrize::gpu::OpenClEnergyEvaluator(m_target, m_current));
        }
        return m_gpuEvaluator && m_gpuEvaluator->isAvailable();
    }

    void resyncIfDirty()
    {
        if(!m_bitmapsMayHaveChanged) {
            return;
        }
        m_lastScore = geometrize::core::differenceFull(m_target, m_current);
        m_gpuEvaluator.reset();
        m_gpuInitializationAttempted = false;
        // A mutable Bitmap reference can outlive this call and be changed
        // again without passing through Model. Keep the flag sticky once such
        // a reference has escaped so every later step remains correct.
    }

    bool evaluateCandidateBatch(
            const std::vector<std::vector<geometrize::Scanline>>& candidates,
            const std::uint8_t alpha,
            std::vector<double>& scores)
    {
        // Kernel launches and readback dominate very sparse masks. Keep those
        // batches on the CPU, while preserving the same lane/RNG schedule used
        // by the GPU hill climber. Filled-shape batches quickly cross this
        // threshold and stay resident on the device.
        std::uint64_t coveredPixels{0U};
        for(const auto& lines : candidates) {
            for(const geometrize::Scanline& line : lines) {
                if(line.x2 >= line.x1) {
                    const std::uint64_t count{static_cast<std::uint64_t>(
                        static_cast<std::int64_t>(line.x2) - static_cast<std::int64_t>(line.x1) + 1)};
                    if(count >= minimumGpuCoveredPixels - (std::min)(coveredPixels, minimumGpuCoveredPixels)) {
                        coveredPixels = minimumGpuCoveredPixels;
                        break;
                    }
                    coveredPixels += count;
                }
            }
            if(coveredPixels >= minimumGpuCoveredPixels) {
                break;
            }
        }

        if(coveredPixels < minimumGpuCoveredPixels) {
            // The desktop path supplies built-in non-overlapping rasterizers,
            // but the public Model API can opt a custom creator into batching.
            // Keep a correctly sized fallback buffer for overlapping custom
            // masks rather than assuming they obey the built-in contract.
            geometrize::Bitmap fallbackBuffer{m_current};
            scores.clear();
            scores.reserve(candidates.size());
            for(const auto& lines : candidates) {
                scores.push_back(geometrize::core::defaultEnergyFunction(
                    lines, alpha, m_target, m_current, fallbackBuffer, m_lastScore));
            }
            return true;
        }

        return ensureGpuEvaluator() && m_gpuEvaluator->evaluate(candidates, alpha, m_lastScore, scores);
    }

    bool getGpuHillClimbStates(
            const std::function<std::shared_ptr<geometrize::Shape>(void)>& shapeCreator,
            const std::uint8_t alpha,
            const std::uint32_t shapeCount,
            const std::uint32_t maxShapeMutations,
            const std::vector<std::uint32_t>& seeds,
            std::vector<geometrize::State>& results)
    {
        const std::size_t candidatesPerLane{static_cast<std::size_t>(shapeCount) + 2U};
        GpuHillClimbCoordinator coordinator{seeds.size()};
        std::vector<std::future<geometrize::State>> hillClimbFutures;
        hillClimbFutures.reserve(seeds.size());
        CoordinatorAbortGuard abortGuard{coordinator};
        for(std::size_t laneIndex = 0; laneIndex < seeds.size(); ++laneIndex) {
            hillClimbFutures.emplace_back(std::async(std::launch::async, [&, laneIndex]() {
                try {
                    geometrize::commonutil::seedRandomGenerator(seeds[laneIndex]);
                    std::vector<geometrize::State> candidates;
                    std::vector<std::vector<geometrize::Scanline>> initialScanlines;
                    candidates.reserve(candidatesPerLane);
                    initialScanlines.reserve(candidatesPerLane);
                    for(std::size_t i = 0; i < candidatesPerLane; ++i) {
                        candidates.emplace_back(shapeCreator(), alpha);
                        const std::shared_ptr<geometrize::Shape>& shape{candidates.back().m_shape};
                        initialScanlines.emplace_back(shape->rasterize(*shape));
                        sortGpuScanlines(initialScanlines.back());
                    }

                    std::vector<double> initialScores;
                    {
                        std::unique_lock<std::mutex> lock{coordinator.mutex};
                        GpuHillClimbLane& lane{coordinator.lanes[laneIndex]};
                        lane.initialScanlines = std::move(initialScanlines);
                        lane.initialReady = true;
                        coordinator.condition.notify_all();
                        coordinator.condition.wait(lock, [&]() { return coordinator.abort || lane.initialScoresReady; });
                        if(coordinator.abort) {
                            return candidates.front();
                        }
                        initialScores = std::move(lane.initialScores);
                    }

                    for(std::size_t i = 0; i < candidates.size(); ++i) {
                        candidates[i].m_score = initialScores[i];
                    }

                    // Match bestRandomState exactly: its first candidate is
                    // scored, but candidate index 1 unconditionally replaces it.
                    geometrize::State bestState{candidates.front()};
                    double bestEnergy{bestState.m_score};
                    for(std::size_t i = 1; i < candidates.size(); ++i) {
                        const double energy{candidates[i].m_score};
                        if(i == 1U || energy < bestEnergy) {
                            bestEnergy = energy;
                            bestState = candidates[i];
                        }
                    }

                    geometrize::State state{bestState};
                    while(true) {
                        std::uint32_t windowSize{0U};
                        {
                            std::unique_lock<std::mutex> lock{coordinator.mutex};
                            GpuHillClimbLane& lane{coordinator.lanes[laneIndex]};
                            if(lane.age >= maxShapeMutations) {
                                lane.done = true;
                                lock.unlock();
                                coordinator.condition.notify_all();
                                return bestState;
                            }
                            std::size_t activeLaneCount{0U};
                            for(const GpuHillClimbLane& other : coordinator.lanes) {
                                activeLaneCount += other.done ? 0U : 1U;
                            }
                            windowSize = gpuSpeculationWindow(
                                lane.age, maxShapeMutations - lane.age, activeLaneCount);
                        }

                        std::vector<geometrize::State> mutationStates;
                        std::vector<std::vector<geometrize::Scanline>> mutationScanlines;
                        std::vector<geometrize::commonutil::RandomGeneratorState> mutationRandomStates;
                        mutationStates.reserve(windowSize);
                        mutationScanlines.reserve(windowSize);
                        mutationRandomStates.reserve(windowSize);
                        for(std::uint32_t i = 0; i < windowSize; ++i) {
                            geometrize::State candidate{state};
                            // State::mutate() creates an undo clone. Each speculative
                            // candidate is already an independent clone of the base,
                            // so mutate the owned shape directly and avoid a second
                            // deep copy that would immediately be discarded.
                            candidate.m_shape->mutate(*candidate.m_shape);
                            candidate.m_score = -1.0;
                            std::vector<geometrize::Scanline> lines{candidate.m_shape->rasterize(*candidate.m_shape)};
                            sortGpuScanlines(lines);
                            geometrize::commonutil::RandomGeneratorState randomState{
                                geometrize::commonutil::captureRandomGeneratorState()};
                            mutationStates.emplace_back(std::move(candidate));
                            mutationScanlines.emplace_back(std::move(lines));
                            mutationRandomStates.emplace_back(std::move(randomState));
                        }

                        {
                            std::unique_lock<std::mutex> lock{coordinator.mutex};
                            GpuHillClimbLane& lane{coordinator.lanes[laneIndex]};
                            lane.mutationStates = std::move(mutationStates);
                            lane.mutationScanlines = std::move(mutationScanlines);
                            lane.mutationRandomStates = std::move(mutationRandomStates);
                            lane.mutationScores.clear();
                            lane.ready = true;
                            coordinator.condition.notify_all();
                            coordinator.condition.wait(lock, [&]() { return coordinator.abort || lane.scoreReady; });
                            if(coordinator.abort) {
                                return bestState;
                            }
                            lane.scoreReady = false;
                            bool accepted{false};
                            for(std::size_t i = 0; i < lane.mutationScores.size(); ++i) {
                                const double energy{lane.mutationScores[i]};
                                if(energy >= bestEnergy) {
                                    ++lane.age;
                                    continue;
                                }
                                state = lane.mutationStates[i];
                                state.m_score = energy;
                                bestEnergy = energy;
                                bestState = state;
                                lane.age = 0U;
                                geometrize::commonutil::restoreRandomGeneratorState(lane.mutationRandomStates[i]);
                                accepted = true;
                                break;
                            }
                            if(!accepted) {
                                assert(lane.mutationScores.size() == windowSize);
                            }
                        }
                    }
                } catch(...) {
                    {
                        std::lock_guard<std::mutex> lock{coordinator.mutex};
                        if(!coordinator.exception) {
                            coordinator.exception = std::current_exception();
                        }
                        coordinator.abort = true;
                        coordinator.lanes[laneIndex].done = true;
                    }
                    coordinator.condition.notify_all();
                    throw;
                }
            }));
        }

        bool gpuSucceeded{true};
        std::vector<std::vector<geometrize::Scanline>> flatScanlines;
        {
            std::unique_lock<std::mutex> lock{coordinator.mutex};
            coordinator.condition.wait(lock, [&]() {
                if(coordinator.abort) {
                    return true;
                }
                for(const GpuHillClimbLane& lane : coordinator.lanes) {
                    if(!lane.initialReady) {
                        return false;
                    }
                }
                return true;
            });
            if(coordinator.abort) {
                gpuSucceeded = false;
            } else {
                flatScanlines.reserve(candidatesPerLane * coordinator.lanes.size());
                for(const GpuHillClimbLane& lane : coordinator.lanes) {
                    for(const auto& lines : lane.initialScanlines) {
                        flatScanlines.push_back(lines);
                    }
                }
            }
        }

        if(gpuSucceeded) {
            std::vector<double> scores;
            if(!evaluateCandidateBatch(flatScanlines, alpha, scores)
                    || scores.size() != flatScanlines.size()) {
                std::lock_guard<std::mutex> lock{coordinator.mutex};
                coordinator.abort = true;
                gpuSucceeded = false;
                coordinator.condition.notify_all();
            } else {
                std::lock_guard<std::mutex> lock{coordinator.mutex};
                std::size_t scoreIndex{0U};
                for(GpuHillClimbLane& lane : coordinator.lanes) {
                    lane.initialScores.assign(
                        scores.begin() + scoreIndex,
                        scores.begin() + scoreIndex + candidatesPerLane);
                    scoreIndex += candidatesPerLane;
                    lane.initialScoresReady = true;
                }
                coordinator.condition.notify_all();
            }
        }

        while(true) {
            if(!gpuSucceeded) {
                break;
            }
            std::vector<std::size_t> activeLaneIndices;
            std::vector<std::vector<geometrize::Scanline>> mutationBatch;
            {
                std::unique_lock<std::mutex> lock{coordinator.mutex};
                coordinator.condition.wait(lock, [&]() {
                    if(coordinator.abort) {
                        return true;
                    }
                    for(const GpuHillClimbLane& lane : coordinator.lanes) {
                        if(!lane.done && !lane.ready) {
                            return false;
                        }
                    }
                    return true;
                });
                if(coordinator.abort) {
                    gpuSucceeded = false;
                    break;
                }
                for(std::size_t i = 0; i < coordinator.lanes.size(); ++i) {
                    if(coordinator.lanes[i].ready) {
                        activeLaneIndices.push_back(i);
                        for(const auto& lines : coordinator.lanes[i].mutationScanlines) {
                            mutationBatch.push_back(lines);
                        }
                    }
                }
            }

            if(activeLaneIndices.empty()) {
                break;
            }

            std::vector<double> mutationScores;
            if(!evaluateCandidateBatch(mutationBatch, alpha, mutationScores)
                    || mutationScores.size() != mutationBatch.size()) {
                {
                    std::lock_guard<std::mutex> lock{coordinator.mutex};
                    coordinator.abort = true;
                }
                coordinator.condition.notify_all();
                gpuSucceeded = false;
                break;
            }

            {
                std::lock_guard<std::mutex> lock{coordinator.mutex};
                std::size_t scoreIndex{0U};
                for(const std::size_t laneIndex : activeLaneIndices) {
                    GpuHillClimbLane& lane{coordinator.lanes[laneIndex]};
                    const std::size_t scoreCount{lane.mutationScanlines.size()};
                    lane.mutationScores.assign(
                        mutationScores.begin() + scoreIndex,
                        mutationScores.begin() + scoreIndex + scoreCount);
                    scoreIndex += scoreCount;
                    lane.ready = false;
                    lane.scoreReady = true;
                }
                assert(scoreIndex == mutationScores.size());
            }
            coordinator.condition.notify_all();
        }

        results.clear();
        results.reserve(hillClimbFutures.size());
        for(auto& future : hillClimbFutures) {
            try {
                results.emplace_back(future.get());
            } catch(...) {
                std::lock_guard<std::mutex> lock{coordinator.mutex};
                if(!coordinator.exception) {
                    coordinator.exception = std::current_exception();
                }
            }
        }
        abortGuard.dismiss();
        if(coordinator.exception) {
            std::rethrow_exception(coordinator.exception);
        }
        return gpuSucceeded;
    }

    std::vector<geometrize::ShapeResult> step(
            const std::function<std::shared_ptr<geometrize::Shape>(void)> shapeCreator,
            const std::uint8_t alpha,
            const std::uint32_t shapeCount,
            const std::uint32_t maxShapeMutations,
            const std::uint32_t maxThreads,
            const geometrize::core::EnergyFunction& energyFunction,
            const geometrize::ShapeAcceptancePreconditionFunction& addShapePrecondition,
            const bool allowGpuBatching)
    {
        std::vector<geometrize::State> states{getHillClimbState(shapeCreator, alpha, shapeCount, maxShapeMutations, maxThreads, energyFunction, allowGpuBatching)};
        if(states.empty()) {
            assert(0 && "Failed to get a hill climb state");
            return {};
        }

        std::vector<geometrize::State>::iterator it = std::min_element(states.begin(), states.end(), [](const geometrize::State& a, const geometrize::State& b) {
            return a.m_score < b.m_score;
        });

        // Draw the shape onto the image
        const std::shared_ptr<geometrize::Shape> shape = it->m_shape;
        const std::vector<geometrize::Scanline> lines{shape->rasterize(*shape)};
        const geometrize::rgba color(geometrize::core::computeColor(m_target, m_current, lines, alpha));

        // For the built-in search and acceptance rule, the winning state's
        // score is already the exact post-draw score. Avoid copying the entire
        // current bitmap merely to recalculate and possibly roll back it.
        if(allowGpuBatching && !energyFunction && !addShapePrecondition) {
            const double newScore{it->m_score};
            if(newScore >= m_lastScore) {
                return {};
            }
            geometrize::drawLines(m_current, color, lines);
            m_lastScore = newScore;
            if(m_gpuEvaluator && !m_gpuEvaluator->commit(lines, color)) {
                m_gpuEvaluator.reset();
            }
            return { geometrize::ShapeResult{m_lastScore, color, shape} };
        }

        const geometrize::Bitmap before{m_current};
        geometrize::drawLines(m_current, color, lines);

        // Check for an improvement - if not, roll back and return no result
        const double newScore = geometrize::core::differencePartial(m_target, before, m_current, m_lastScore, lines);
        const auto& addShapeCondition = addShapePrecondition ? addShapePrecondition : defaultAddShapePrecondition;
        if(!addShapeCondition(m_lastScore, newScore, *shape, lines, color, before, m_current, m_target)) {
            m_current = before;
            return {};
        }

        // Improvement - set new baseline and return the new shape
        m_lastScore = newScore;
        if(m_gpuEvaluator && !m_gpuEvaluator->commit(lines, color)) {
            m_gpuEvaluator.reset();
        }
        const geometrize::ShapeResult result{m_lastScore, color, shape};
        return { result };
    }

    geometrize::ShapeResult drawShape(
            const std::shared_ptr<geometrize::Shape> shape,
            const geometrize::rgba color)
    {
        resyncIfDirty();
        const std::vector<geometrize::Scanline> lines{shape->rasterize(*shape)};
        const geometrize::Bitmap before{m_current};
        geometrize::drawLines(m_current, color, lines);

        m_lastScore = geometrize::core::differencePartial(m_target, before, m_current, m_lastScore, lines);

        if(m_gpuEvaluator && !m_gpuEvaluator->commit(lines, color)) {
            m_gpuEvaluator.reset();
        }

        const geometrize::ShapeResult result{m_lastScore, color, shape};
        return result;
    }

    geometrize::Bitmap& getTarget()
    {
        m_bitmapsMayHaveChanged = true;
        return m_target;
    }

    geometrize::Bitmap& getCurrent()
    {
        m_bitmapsMayHaveChanged = true;
        return m_current;
    }

    const geometrize::Bitmap& getTarget() const
    {
        return m_target;
    }

    const geometrize::Bitmap& getCurrent() const
    {
        return m_current;
    }

    void setSeed(const std::uint32_t seed)
    {
        m_baseRandomSeed = seed;
    }

private:
    geometrize::Bitmap m_target; ///< The target bitmap, the bitmap we aim to approximate.
    geometrize::Bitmap m_current; ///< The current bitmap.
    double m_lastScore; ///< Score derived from calculating the difference between bitmaps.
    const static std::uint32_t defaultMaxThreads{4};
    std::atomic<std::uint32_t> m_baseRandomSeed; ///< The base value used for seeding the random number generator (the one the user has control over).
    std::atomic<std::uint32_t> m_randomSeedOffset; ///< Seed used for random number generation. Note: incremented by each std::async call used for model stepping.
    const static std::size_t minimumGpuBatchSize{64U};
    const static std::uint64_t minimumGpuWork{UINT64_C(200000000)};
    const static std::uint64_t minimumGpuCoveredPixels{UINT64_C(250000)};
    bool m_gpuInitializationAttempted{false};
    bool m_bitmapsMayHaveChanged{false}; ///< Sticky after a mutable bitmap reference has been exposed.
    std::unique_ptr<geometrize::gpu::OpenClEnergyEvaluator> m_gpuEvaluator;
};

Model::Model(const geometrize::Bitmap& target) : d{std::unique_ptr<Model::ModelImpl>(new Model::ModelImpl(target))}
{}

Model::Model(const geometrize::Bitmap& target, const geometrize::Bitmap& initial) : d{std::unique_ptr<Model::ModelImpl>(new Model::ModelImpl(target, initial))}
{}

Model::~Model()
{}

void Model::reset(const geometrize::rgba backgroundColor)
{
    d->reset(backgroundColor);
}

std::int32_t Model::getWidth() const
{
    return d->getWidth();
}

std::int32_t Model::getHeight() const
{
    return d->getHeight();
}

std::vector<geometrize::ShapeResult> Model::step(
        const std::function<std::shared_ptr<geometrize::Shape>(void)>& shapeCreator,
        const std::uint8_t alpha,
        const std::uint32_t shapeCount,
        const std::uint32_t maxShapeMutations,
        const std::uint32_t maxThreads,
        const geometrize::core::EnergyFunction& energyFunction,
        const geometrize::ShapeAcceptancePreconditionFunction& addShapePrecondition)
{
    return d->step(shapeCreator, alpha, shapeCount, maxShapeMutations, maxThreads, energyFunction, addShapePrecondition, false);
}

std::vector<geometrize::ShapeResult> Model::stepWithGpuBatching(
        const std::function<std::shared_ptr<geometrize::Shape>(void)>& shapeCreator,
        const std::uint8_t alpha,
        const std::uint32_t shapeCount,
        const std::uint32_t maxShapeMutations,
        const std::uint32_t maxThreads,
        const geometrize::core::EnergyFunction& energyFunction,
        const geometrize::ShapeAcceptancePreconditionFunction& addShapePrecondition,
        const bool allowGpuBatching)
{
    return d->step(shapeCreator, alpha, shapeCount, maxShapeMutations, maxThreads, energyFunction, addShapePrecondition, allowGpuBatching);
}

geometrize::ShapeResult Model::drawShape(std::shared_ptr<geometrize::Shape> shape, geometrize::rgba color)
{
    return d->drawShape(shape, color);
}

geometrize::Bitmap& Model::getTarget()
{
    return d->getTarget();
}

geometrize::Bitmap& Model::getCurrent()
{
    return d->getCurrent();
}

const geometrize::Bitmap& Model::getTarget() const
{
    const Model::ModelImpl& impl{*d};
    return impl.getTarget();
}

const geometrize::Bitmap& Model::getCurrent() const
{
    const Model::ModelImpl& impl{*d};
    return impl.getCurrent();
}

void Model::setSeed(const std::uint32_t seed)
{
    d->setSeed(seed);
}

}
