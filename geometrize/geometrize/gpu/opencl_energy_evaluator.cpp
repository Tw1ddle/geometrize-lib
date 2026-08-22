#include "opencl_energy_evaluator.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "../bitmap/bitmap.h"

#if defined(GEOMETRIZE_OPENCL) && defined(_WIN32)
#define NOMINMAX
#include <Windows.h>
#include "opencl_types.h"
#endif

namespace geometrize
{
namespace gpu
{

#if defined(GEOMETRIZE_OPENCL) && defined(_WIN32)

namespace
{

constexpr std::size_t workGroupSize{256U};
constexpr std::uint32_t multiWorkGroupMaxGroups{32U};
constexpr std::size_t multiWorkGroupMaxCandidates{64U};
constexpr std::uint64_t multiWorkGroupMinPixelsPerCandidate{131072ULL};
constexpr std::uint64_t multiWorkGroupTargetPixelsPerGroup{32768ULL};
constexpr std::size_t multiWorkGroupTargetGroupCount{256U};
constexpr std::uint64_t maximumSafePixelCount{
    static_cast<std::uint64_t>((std::numeric_limits<std::int64_t>::max)()) / 16842495ULL};

struct alignas(16) DeviceScanline
{
    std::int32_t y;
    std::int32_t x1;
    std::int32_t x2;
    std::int32_t padding;
};

struct CandidateRange
{
    std::uint32_t offset;
    std::uint32_t count;
};

struct alignas(16) DeviceErrorPartial
{
    std::uint64_t before;
    std::uint64_t after;
};

static_assert(sizeof(DeviceScanline) == 16U, "OpenCL int4 ABI mismatch");
static_assert(sizeof(CandidateRange) == 8U, "OpenCL uint2 ABI mismatch");
static_assert(sizeof(DeviceErrorPartial) == 16U, "OpenCL ulong2 ABI mismatch");
static_assert(alignof(DeviceErrorPartial) == 16U, "OpenCL ulong2 alignment mismatch");

const char* kernelSource = R"CLC(
__kernel __attribute__((reqd_work_group_size(256, 1, 1)))
void compute_scores(
        __global const uchar4* target,
        __global const uchar4* current,
        __global const int4* lines,
        __global const uint2* ranges,
        const int width,
        const int color_scale,
        const uchar alpha,
        __global ulong* before_sse,
        __global ulong* after_sse)
{
    const uint candidate = get_group_id(0);
    const uint lid = get_local_id(0);
    const uint2 range = ranges[candidate];

    long red = 0;
    long green = 0;
    long blue = 0;
    ulong count = 0;

    for(uint line_index = 0; line_index < range.y; ++line_index) {
        const int4 line = lines[range.x + line_index];
        for(int x = line.y + (int)lid; x <= line.z; x += 256) {
            const size_t pixel_index = (size_t)line.x * (size_t)width + (size_t)x;
            const uchar4 t = target[pixel_index];
            const uchar4 c = current[pixel_index];
            red += ((int)t.x - (int)c.x) * (long)color_scale + (int)c.x * 257L;
            green += ((int)t.y - (int)c.y) * (long)color_scale + (int)c.y * 257L;
            blue += ((int)t.z - (int)c.z) * (long)color_scale + (int)c.z * 257L;
            ++count;
        }
    }

    // Four reusable 64-bit arrays keep local memory at exactly 8 KiB. Signed
    // color totals are reduced through their two's-complement bit patterns so
    // the addition itself is well-defined even when a batch is very large.
    __local ulong scratch0[256];
    __local ulong scratch1[256];
    __local ulong scratch2[256];
    __local ulong scratch3[256];
    scratch0[lid] = as_ulong(red);
    scratch1[lid] = as_ulong(green);
    scratch2[lid] = as_ulong(blue);
    scratch3[lid] = count;
    barrier(CLK_LOCAL_MEM_FENCE);

    for(uint stride = 128; stride > 0; stride >>= 1) {
        if(lid < stride) {
            scratch0[lid] += scratch0[lid + stride];
            scratch1[lid] += scratch1[lid + stride];
            scratch2[lid] += scratch2[lid + stride];
            scratch3[lid] += scratch3[lid + stride];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    if(lid == 0) {
        uint packed = ((uint)alpha) << 24;
        if(scratch3[0] != 0) {
            const long count_total = (long)scratch3[0];
            const int r = clamp(convert_int(as_long(scratch0[0]) / count_total) >> 8, 0, 255);
            const int g = clamp(convert_int(as_long(scratch1[0]) / count_total) >> 8, 0, 255);
            const int b = clamp(convert_int(as_long(scratch2[0]) / count_total) >> 8, 0, 255);
            packed |= (uint)r | ((uint)g << 8) | ((uint)b << 16);
        }
        scratch3[0] = (ulong)packed;
    }
    barrier(CLK_LOCAL_MEM_FENCE);

    const uint packed = (uint)scratch3[0];

    const uint color_r = packed & 255U;
    const uint color_g = (packed >> 8) & 255U;
    const uint color_b = (packed >> 16) & 255U;
    const uint color_a = (packed >> 24) & 255U;

    uint sr = color_r;
    sr |= sr << 8;
    sr *= color_a;
    sr /= 255U;
    uint sg = color_g;
    sg |= sg << 8;
    sg *= color_a;
    sg /= 255U;
    uint sb = color_b;
    sb |= sb << 8;
    sb *= color_a;
    sb /= 255U;
    uint sa = color_a;
    sa |= sa << 8;

    const uint m = 65535U;
    const uint aa = (m - sa) * 257U;

    ulong before_total = 0;
    ulong after_total = 0;

    for(uint line_index = 0; line_index < range.y; ++line_index) {
        const int4 line = lines[range.x + line_index];
        for(int x = line.y + (int)lid; x <= line.z; x += 256) {
            const size_t pixel_index = (size_t)line.x * (size_t)width + (size_t)x;
            const uchar4 t = target[pixel_index];
            const uchar4 c = current[pixel_index];

            const uchar4 blended = (uchar4)(
                (uchar)((((uint)c.x * aa + sr * m) / m) >> 8),
                (uchar)((((uint)c.y * aa + sg * m) / m) >> 8),
                (uchar)((((uint)c.z * aa + sb * m) / m) >> 8),
                (uchar)((((uint)c.w * aa + sa * m) / m) >> 8));

            const int4 db = convert_int4(t) - convert_int4(c);
            const int4 da = convert_int4(t) - convert_int4(blended);
            before_total += (ulong)(db.x * db.x + db.y * db.y + db.z * db.z + db.w * db.w);
            after_total += (ulong)(da.x * da.x + da.y * da.y + da.z * da.z + da.w * da.w);
        }
    }

    scratch0[lid] = before_total;
    scratch1[lid] = after_total;
    barrier(CLK_LOCAL_MEM_FENCE);

    for(uint stride = 128; stride > 0; stride >>= 1) {
        if(lid < stride) {
            scratch0[lid] += scratch0[lid + stride];
            scratch1[lid] += scratch1[lid + stride];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    if(lid == 0) {
        before_sse[candidate] = scratch0[0];
        after_sse[candidate] = scratch1[0];
    }
}

// The multi-work-group path splits a large candidate across several work
// groups. It deliberately uses three kernels instead of 64-bit atomics: this
// stays within OpenCL 1.2 and makes the integer result independent of device
// scheduling order.
__kernel __attribute__((reqd_work_group_size(256, 1, 1)))
void compute_color_partials(
        __global const uchar4* target,
        __global const uchar4* current,
        __global const int4* lines,
        __global const uint2* ranges,
        const int width,
        const int color_scale,
        const uint groups_per_candidate,
        __global ulong4* partial_colors)
{
    const size_t flat_group = get_group_id(0);
    const uint candidate = (uint)(flat_group / (size_t)groups_per_candidate);
    const uint partition = (uint)(flat_group % (size_t)groups_per_candidate);
    const uint lid = get_local_id(0);
    const uint2 range = ranges[candidate];

    long red = 0;
    long green = 0;
    long blue = 0;
    ulong count = 0;

    for(uint line_index = partition; line_index < range.y; line_index += groups_per_candidate) {
        const int4 line = lines[range.x + line_index];
        for(int x = line.y + (int)lid; x <= line.z; x += 256) {
            const size_t pixel_index = (size_t)line.x * (size_t)width + (size_t)x;
            const uchar4 t = target[pixel_index];
            const uchar4 c = current[pixel_index];
            red += ((int)t.x - (int)c.x) * (long)color_scale + (int)c.x * 257L;
            green += ((int)t.y - (int)c.y) * (long)color_scale + (int)c.y * 257L;
            blue += ((int)t.z - (int)c.z) * (long)color_scale + (int)c.z * 257L;
            ++count;
        }
    }

    __local ulong scratch0[256];
    __local ulong scratch1[256];
    __local ulong scratch2[256];
    __local ulong scratch3[256];
    scratch0[lid] = as_ulong(red);
    scratch1[lid] = as_ulong(green);
    scratch2[lid] = as_ulong(blue);
    scratch3[lid] = count;
    barrier(CLK_LOCAL_MEM_FENCE);

    for(uint stride = 128; stride > 0; stride >>= 1) {
        if(lid < stride) {
            scratch0[lid] += scratch0[lid + stride];
            scratch1[lid] += scratch1[lid + stride];
            scratch2[lid] += scratch2[lid + stride];
            scratch3[lid] += scratch3[lid + stride];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    if(lid == 0) {
        partial_colors[flat_group] = (ulong4)(
            scratch0[0], scratch1[0], scratch2[0], scratch3[0]);
    }
}

__kernel void finalize_colors(
        __global const ulong4* partial_colors,
        const uint groups_per_candidate,
        const uchar alpha,
        __global uint* packed_colors)
{
    const size_t candidate = get_global_id(0);
    const size_t base = candidate * (size_t)groups_per_candidate;
    ulong4 total = (ulong4)(0UL, 0UL, 0UL, 0UL);
    for(uint group = 0; group < groups_per_candidate; ++group) {
        total += partial_colors[base + (size_t)group];
    }

    uint packed = ((uint)alpha) << 24;
    if(total.w != 0) {
        const long count = (long)total.w;
        const int r = clamp(convert_int(as_long(total.x) / count) >> 8, 0, 255);
        const int g = clamp(convert_int(as_long(total.y) / count) >> 8, 0, 255);
        const int b = clamp(convert_int(as_long(total.z) / count) >> 8, 0, 255);
        packed |= (uint)r | ((uint)g << 8) | ((uint)b << 16);
    }
    packed_colors[candidate] = packed;
}

__kernel __attribute__((reqd_work_group_size(256, 1, 1)))
void compute_error_partials(
        __global const uchar4* target,
        __global const uchar4* current,
        __global const int4* lines,
        __global const uint2* ranges,
        const int width,
        const uint groups_per_candidate,
        __global const uint* packed_colors,
        __global ulong2* partial_errors)
{
    const size_t flat_group = get_group_id(0);
    const uint candidate = (uint)(flat_group / (size_t)groups_per_candidate);
    const uint partition = (uint)(flat_group % (size_t)groups_per_candidate);
    const uint lid = get_local_id(0);
    const uint2 range = ranges[candidate];
    const uint packed = packed_colors[candidate];

    const uint color_r = packed & 255U;
    const uint color_g = (packed >> 8) & 255U;
    const uint color_b = (packed >> 16) & 255U;
    const uint color_a = (packed >> 24) & 255U;

    uint sr = color_r;
    sr |= sr << 8;
    sr *= color_a;
    sr /= 255U;
    uint sg = color_g;
    sg |= sg << 8;
    sg *= color_a;
    sg /= 255U;
    uint sb = color_b;
    sb |= sb << 8;
    sb *= color_a;
    sb /= 255U;
    uint sa = color_a;
    sa |= sa << 8;

    const uint m = 65535U;
    const uint aa = (m - sa) * 257U;
    ulong before_total = 0;
    ulong after_total = 0;

    for(uint line_index = partition; line_index < range.y; line_index += groups_per_candidate) {
        const int4 line = lines[range.x + line_index];
        for(int x = line.y + (int)lid; x <= line.z; x += 256) {
            const size_t pixel_index = (size_t)line.x * (size_t)width + (size_t)x;
            const uchar4 t = target[pixel_index];
            const uchar4 c = current[pixel_index];
            const uchar4 blended = (uchar4)(
                (uchar)((((uint)c.x * aa + sr * m) / m) >> 8),
                (uchar)((((uint)c.y * aa + sg * m) / m) >> 8),
                (uchar)((((uint)c.z * aa + sb * m) / m) >> 8),
                (uchar)((((uint)c.w * aa + sa * m) / m) >> 8));

            const int4 db = convert_int4(t) - convert_int4(c);
            const int4 da = convert_int4(t) - convert_int4(blended);
            before_total += (ulong)(db.x * db.x + db.y * db.y + db.z * db.z + db.w * db.w);
            after_total += (ulong)(da.x * da.x + da.y * da.y + da.z * da.z + da.w * da.w);
        }
    }

    __local ulong before_scratch[256];
    __local ulong after_scratch[256];
    before_scratch[lid] = before_total;
    after_scratch[lid] = after_total;
    barrier(CLK_LOCAL_MEM_FENCE);

    for(uint stride = 128; stride > 0; stride >>= 1) {
        if(lid < stride) {
            before_scratch[lid] += before_scratch[lid + stride];
            after_scratch[lid] += after_scratch[lid + stride];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    if(lid == 0) {
        partial_errors[flat_group] = (ulong2)(before_scratch[0], after_scratch[0]);
    }
}

__kernel __attribute__((reqd_work_group_size(256, 1, 1)))
void commit_lines(
        __global uchar4* current,
        __global const int4* lines,
        const uint line_count,
        const int width,
        const uint packed)
{
    const uint lid = get_local_id(0);
    const uint color_r = packed & 255U;
    const uint color_g = (packed >> 8) & 255U;
    const uint color_b = (packed >> 16) & 255U;
    const uint color_a = (packed >> 24) & 255U;

    uint sr = color_r;
    sr |= sr << 8;
    sr *= color_a;
    sr /= 255U;
    uint sg = color_g;
    sg |= sg << 8;
    sg *= color_a;
    sg /= 255U;
    uint sb = color_b;
    sb |= sb << 8;
    sb *= color_a;
    sb /= 255U;
    uint sa = color_a;
    sa |= sa << 8;

    const uint m = 65535U;
    const uint aa = (m - sa) * 257U;

    for(uint line_index = 0; line_index < line_count; ++line_index) {
        const int4 line = lines[line_index];
        for(int x = line.y + (int)lid; x <= line.z; x += 256) {
            const int pixel_index = line.x * width + x;
            const uchar4 c = current[pixel_index];
            current[pixel_index] = (uchar4)(
                (uchar)((((uint)c.x * aa + sr * m) / m) >> 8),
                (uchar)((((uint)c.y * aa + sg * m) / m) >> 8),
                (uchar)((((uint)c.z * aa + sb * m) / m) >> 8),
                (uchar)((((uint)c.w * aa + sa * m) / m) >> 8));
        }
    }
}
)CLC";

bool isGpuDisabledByEnvironment()
{
    const char* value = std::getenv("GEOMETRIZE_GPU");
    if(value == nullptr) {
        return false;
    }
    std::string mode{value};
    std::transform(mode.begin(), mode.end(), mode.begin(), [](const unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return mode == "0" || mode == "off" || mode == "false" || mode == "cpu";
}

std::string gpuDeviceOverride()
{
    const char* value = std::getenv("GEOMETRIZE_GPU_DEVICE");
    if(value == nullptr) {
        return {};
    }
    std::string name{value};
    std::transform(name.begin(), name.end(), name.begin(), [](const unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return name;
}

// -1 selects automatically, 0 disables the split path, 1 forces automatic
// sizing, and 2..32 force that exact number of work groups per candidate.
int multiWorkGroupOverride()
{
    const char* value = std::getenv("GEOMETRIZE_GPU_MULTIGROUP");
    if(value == nullptr) {
        return -1;
    }
    std::string mode{value};
    std::transform(mode.begin(), mode.end(), mode.begin(), [](const unsigned char c) { return static_cast<char>(std::tolower(c)); });
    if(mode == "0" || mode == "off" || mode == "false" || mode == "single") {
        return 0;
    }
    if(mode == "1" || mode == "on" || mode == "true" || mode == "force") {
        return 1;
    }

    char* end{nullptr};
    const unsigned long groups{std::strtoul(mode.c_str(), &end, 10)};
    if(end != mode.c_str() && *end == '\0' && groups >= 2UL && groups <= multiWorkGroupMaxGroups) {
        return static_cast<int>(groups);
    }
    return -1;
}

std::uint32_t chooseMultiWorkGroupCount(
        const std::size_t candidateCount,
        const std::uint64_t totalPixelCount,
        const std::uint64_t largestPixelCount,
        const std::size_t largestLineCount)
{
    const int overrideValue{multiWorkGroupOverride()};
    if(overrideValue == 0 || candidateCount == 0U || largestLineCount < 2U) {
        return 1U;
    }

    const std::uint64_t averagePixelCount{totalPixelCount / candidateCount};
    if(overrideValue < 0
            && (candidateCount > multiWorkGroupMaxCandidates
                || averagePixelCount < multiWorkGroupMinPixelsPerCandidate)) {
        return 1U;
    }

    std::uint64_t desiredForPixels{
        (largestPixelCount + multiWorkGroupTargetPixelsPerGroup - 1ULL)
        / multiWorkGroupTargetPixelsPerGroup};
    const std::size_t desiredForOccupancy{
        (multiWorkGroupTargetGroupCount + candidateCount - 1U) / candidateCount};
    std::uint64_t desired{(std::max)(std::uint64_t{2U}, desiredForPixels)};
    desired = (std::max)(desired, static_cast<std::uint64_t>(desiredForOccupancy));
    if(overrideValue >= 2) {
        desired = static_cast<std::uint64_t>(overrideValue);
    }

    std::uint32_t rounded{1U};
    while(rounded < desired && rounded < multiWorkGroupMaxGroups) {
        rounded <<= 1U;
    }
    rounded = (std::min)(rounded, multiWorkGroupMaxGroups);
    rounded = (std::min)(rounded, static_cast<std::uint32_t>((std::min)(
        largestLineCount, static_cast<std::size_t>(multiWorkGroupMaxGroups))));
    return rounded >= 2U ? rounded : 1U;
}

std::uint32_t packColor(const geometrize::rgba color)
{
    return static_cast<std::uint32_t>(color.r)
        | (static_cast<std::uint32_t>(color.g) << 8U)
        | (static_cast<std::uint32_t>(color.b) << 16U)
        | (static_cast<std::uint32_t>(color.a) << 24U);
}

bool normalizeScanlines(
        const std::vector<geometrize::Scanline>& lines,
        const std::uint32_t width,
        const std::uint32_t height,
        std::vector<geometrize::Scanline>& normalized)
{
    normalized = lines;
    const auto lessScanline = [](const geometrize::Scanline& a, const geometrize::Scanline& b) {
        return a.y < b.y || (a.y == b.y && (a.x1 < b.x1 || (a.x1 == b.x1 && a.x2 < b.x2)));
    };
    if(!std::is_sorted(normalized.begin(), normalized.end(), lessScanline)) {
        std::sort(normalized.begin(), normalized.end(), lessScanline);
    }

    std::int32_t lastY{std::numeric_limits<std::int32_t>::min()};
    std::int32_t lastX2{std::numeric_limits<std::int32_t>::min()};
    for(const geometrize::Scanline& line : normalized) {
        if(line.y < 0 || line.y >= static_cast<std::int32_t>(height)
                || line.x1 < 0 || line.x2 < line.x1 || line.x2 >= static_cast<std::int32_t>(width)) {
            return false;
        }
        if(line.y == lastY && line.x1 <= lastX2) {
            return false;
        }
        if(line.y != lastY) {
            lastY = line.y;
        }
        lastX2 = line.x2;
    }
    return true;
}

}

class OpenClEnergyEvaluator::Impl
{
public:
    Impl(const geometrize::Bitmap& target, const geometrize::Bitmap& current) :
        m_width{target.getWidth()},
        m_height{target.getHeight()}
    {
        if(isGpuDisabledByEnvironment()) {
            m_lastError = "disabled by GEOMETRIZE_GPU";
            return;
        }
        if(target.getWidth() != current.getWidth() || target.getHeight() != current.getHeight()) {
            m_lastError = "target/current size mismatch";
            return;
        }
        if(!loadRuntime() || !selectDevice() || !createRuntime() || !uploadImages(target, current)) {
            releaseRuntime();
            return;
        }
        m_available = true;
        std::cerr << "[Geometrize GPU] OpenCL active: " << m_deviceName << std::endl;
    }

    ~Impl()
    {
        releaseRuntime();
    }

    bool evaluate(
            const std::vector<std::vector<geometrize::Scanline>>& candidateLines,
            const std::uint8_t alpha,
            const double score,
            std::vector<double>& scores)
    {
        scores.clear();
        if(!m_available || alpha == 0 || candidateLines.empty()) {
            return false;
        }
        if(candidateLines.size() > std::numeric_limits<std::uint32_t>::max()
                || candidateLines.size() > std::numeric_limits<std::size_t>::max() / workGroupSize) {
            m_lastError = "candidate batch is too large";
            return false;
        }

        // Single pass: normalize into one reused buffer and append straight to
        // the flat device array. The previous two-pass form kept a normalized
        // copy of every candidate alive just to walk them again, which meant a
        // vector allocation per candidate per batch.
        std::vector<DeviceScanline>& flatLines{m_scratchFlatLines};
        std::vector<CandidateRange>& ranges{m_scratchRanges};
        flatLines.clear();
        ranges.clear();
        ranges.reserve(candidateLines.size());

        std::size_t totalLineCount{0};
        std::size_t largestLineCount{0};
        std::uint64_t totalPixelCount{0};
        std::uint64_t largestPixelCount{0};
        for(const auto& lines : candidateLines) {
            std::vector<geometrize::Scanline>& normalized{m_scratchNormalized};
            if(!normalizeScanlines(lines, m_width, m_height, normalized)) {
                m_lastError = "candidate contains overlapping, unordered, or out-of-bounds scanlines";
                return false;
            }
            if(normalized.size() > std::numeric_limits<std::uint32_t>::max() - totalLineCount) {
                m_lastError = "candidate scanline batch is too large";
                return false;
            }
            std::uint64_t candidatePixelCount{0};
            for(const geometrize::Scanline& line : normalized) {
                const std::uint64_t span{
                    static_cast<std::uint64_t>(static_cast<std::int64_t>(line.x2) - line.x1 + 1)};
                if(candidatePixelCount > std::numeric_limits<std::uint64_t>::max() - span) {
                    m_lastError = "candidate pixel count is too large";
                    return false;
                }
                candidatePixelCount += span;
            }
            if(totalPixelCount > std::numeric_limits<std::uint64_t>::max() - candidatePixelCount) {
                m_lastError = "candidate batch pixel count is too large";
                return false;
            }
            // Per-channel color accumulation is signed 64-bit on both CPU and
            // GPU. Reject a theoretical mask large enough to overflow rather
            // than relying on undefined signed arithmetic.
            if(candidatePixelCount > maximumSafePixelCount) {
                m_lastError = "candidate pixel count exceeds exact OpenCL accumulator range";
                return false;
            }
            totalPixelCount += candidatePixelCount;
            largestPixelCount = (std::max)(largestPixelCount, candidatePixelCount);
            largestLineCount = (std::max)(largestLineCount, normalized.size());
            totalLineCount += normalized.size();

            if(flatLines.size() > std::numeric_limits<std::uint32_t>::max()
                    || normalized.size() > std::numeric_limits<std::uint32_t>::max() - flatLines.size()) {
                m_lastError = "candidate batch is too large";
                return false;
            }
            ranges.push_back(CandidateRange{
                static_cast<std::uint32_t>(flatLines.size()),
                static_cast<std::uint32_t>(normalized.size())});
            for(const geometrize::Scanline& line : normalized) {
                flatLines.push_back(DeviceScanline{line.y, line.x1, line.x2, 0});
            }
        }

        const std::size_t lineBytes{(std::max)(std::size_t{1U}, flatLines.size() * sizeof(DeviceScanline))};
        const std::size_t rangeBytes{ranges.size() * sizeof(CandidateRange)};
        const std::size_t scoreBytes{ranges.size() * sizeof(std::uint64_t)};
        const std::uint32_t groupsPerCandidate{chooseMultiWorkGroupCount(
            ranges.size(), totalPixelCount, largestPixelCount, largestLineCount)};
        const bool useMultiWorkGroupPath{groupsPerCandidate > 1U};

        if(!ensureBuffer(m_lines, m_linesCapacity, lineBytes, cl::mem_read_only)
                || !ensureBuffer(m_ranges, m_rangesCapacity, rangeBytes, cl::mem_read_only)) {
            disable("failed to allocate batch buffers");
            return false;
        }

        // The command queue is in-order. Keep uploads asynchronous and let the
        // final blocking result read synchronize the complete write/kernel
        // chain, instead of draining the GPU before every dispatch.
        if(!flatLines.empty() && !writeBufferNonBlocking(m_lines, flatLines.data(), flatLines.size() * sizeof(DeviceScanline))) {
            disable("failed to upload scanlines");
            return false;
        }
        if(!writeBufferNonBlocking(m_ranges, ranges.data(), rangeBytes)) {
            drainAndDisable("failed to upload candidate ranges");
            return false;
        }

        const std::int32_t width{static_cast<std::int32_t>(m_width)};
        const std::int32_t colorScale{static_cast<std::int32_t>(257.0f * 255.0f / static_cast<float>(alpha))};
        const std::size_t localSize{workGroupSize};
        // assign() rather than resize(): the multi-work-group path accumulates
        // into these, so they must start at zero on every batch.
        std::vector<std::uint64_t>& beforeValues{m_scratchBefore};
        std::vector<std::uint64_t>& afterValues{m_scratchAfter};
        beforeValues.assign(ranges.size(), 0ULL);
        afterValues.assign(ranges.size(), 0ULL);

        if(useMultiWorkGroupPath) {
            const std::size_t groupsPerCandidateSize{groupsPerCandidate};
            if(ranges.size() > std::numeric_limits<std::size_t>::max() / groupsPerCandidateSize) {
                drainAndDisable("multi-work-group candidate batch is too large");
                return false;
            }
            const std::size_t groupCount{ranges.size() * groupsPerCandidateSize};
            if(groupCount > std::numeric_limits<std::size_t>::max() / workGroupSize
                    || groupCount > std::numeric_limits<std::size_t>::max() / (4U * sizeof(std::uint64_t))
                    || groupCount > std::numeric_limits<std::size_t>::max() / sizeof(DeviceErrorPartial)) {
                drainAndDisable("multi-work-group buffer size is too large");
                return false;
            }
            const std::size_t partialColorBytes{groupCount * 4U * sizeof(std::uint64_t)};
            const std::size_t packedColorBytes{ranges.size() * sizeof(std::uint32_t)};
            const std::size_t partialErrorBytes{groupCount * sizeof(DeviceErrorPartial)};
            if(!ensureBuffer(m_partialColors, m_partialColorsCapacity, partialColorBytes, cl::mem_read_write)
                    || !ensureBuffer(m_packedColors, m_packedColorsCapacity, packedColorBytes, cl::mem_read_write)
                    || !ensureBuffer(m_partialErrors, m_partialErrorsCapacity, partialErrorBytes, cl::mem_write_only)) {
                drainAndDisable("failed to allocate multi-work-group scoring buffers");
                return false;
            }

            if(!setArg(m_computeColorPartialsKernel, 0, m_target)
                    || !setArg(m_computeColorPartialsKernel, 1, m_current)
                    || !setArg(m_computeColorPartialsKernel, 2, m_lines)
                    || !setArg(m_computeColorPartialsKernel, 3, m_ranges)
                    || !setScalarArg(m_computeColorPartialsKernel, 4, width)
                    || !setScalarArg(m_computeColorPartialsKernel, 5, colorScale)
                    || !setScalarArg(m_computeColorPartialsKernel, 6, groupsPerCandidate)
                    || !setArg(m_computeColorPartialsKernel, 7, m_partialColors)
                    || !setArg(m_finalizeColorsKernel, 0, m_partialColors)
                    || !setScalarArg(m_finalizeColorsKernel, 1, groupsPerCandidate)
                    || !setScalarArg(m_finalizeColorsKernel, 2, alpha)
                    || !setArg(m_finalizeColorsKernel, 3, m_packedColors)
                    || !setArg(m_computeErrorPartialsKernel, 0, m_target)
                    || !setArg(m_computeErrorPartialsKernel, 1, m_current)
                    || !setArg(m_computeErrorPartialsKernel, 2, m_lines)
                    || !setArg(m_computeErrorPartialsKernel, 3, m_ranges)
                    || !setScalarArg(m_computeErrorPartialsKernel, 4, width)
                     || !setScalarArg(m_computeErrorPartialsKernel, 5, groupsPerCandidate)
                     || !setArg(m_computeErrorPartialsKernel, 6, m_packedColors)
                     || !setArg(m_computeErrorPartialsKernel, 7, m_partialErrors)) {
                drainAndDisable("failed to set multi-work-group scoring arguments");
                return false;
            }

            const std::size_t partialGlobalSize{groupCount * workGroupSize};
            const std::size_t finalizeGlobalSize{ranges.size()};
            if(m_api.enqueueNDRangeKernel(m_queue, m_computeColorPartialsKernel, 1U, nullptr,
                        &partialGlobalSize, &localSize, 0U, nullptr, nullptr) != cl::success
                    || m_api.enqueueNDRangeKernel(m_queue, m_finalizeColorsKernel, 1U, nullptr,
                        &finalizeGlobalSize, nullptr, 0U, nullptr, nullptr) != cl::success
                    || m_api.enqueueNDRangeKernel(m_queue, m_computeErrorPartialsKernel, 1U, nullptr,
                        &partialGlobalSize, &localSize, 0U, nullptr, nullptr) != cl::success) {
                drainAndDisable("failed to dispatch multi-work-group scoring kernels");
                return false;
            }

            std::vector<DeviceErrorPartial>& partialErrors{m_scratchPartialErrors};
            partialErrors.resize(groupCount);
            if(!readBuffer(m_partialErrors, partialErrors.data(), partialErrorBytes)) {
                drainAndDisable("failed to read multi-work-group scoring results");
                return false;
            }
            for(std::size_t candidate = 0; candidate < ranges.size(); ++candidate) {
                const std::size_t base{candidate * groupsPerCandidateSize};
                for(std::size_t group = 0; group < groupsPerCandidateSize; ++group) {
                    beforeValues[candidate] += partialErrors[base + group].before;
                    afterValues[candidate] += partialErrors[base + group].after;
                }
            }
        } else {
            if(!ensureBuffer(m_before, m_beforeCapacity, scoreBytes, cl::mem_write_only)
                    || !ensureBuffer(m_after, m_afterCapacity, scoreBytes, cl::mem_write_only)) {
                drainAndDisable("failed to allocate fused scoring buffers");
                return false;
            }
            if(!setArg(m_computeScoresKernel, 0, m_target)
                    || !setArg(m_computeScoresKernel, 1, m_current)
                    || !setArg(m_computeScoresKernel, 2, m_lines)
                    || !setArg(m_computeScoresKernel, 3, m_ranges)
                    || !setScalarArg(m_computeScoresKernel, 4, width)
                    || !setScalarArg(m_computeScoresKernel, 5, colorScale)
                    || !setScalarArg(m_computeScoresKernel, 6, alpha)
                    || !setArg(m_computeScoresKernel, 7, m_before)
                    || !setArg(m_computeScoresKernel, 8, m_after)) {
                drainAndDisable("failed to set compute_scores arguments");
                return false;
            }

            const std::size_t globalSize{ranges.size() * workGroupSize};
            if(m_api.enqueueNDRangeKernel(m_queue, m_computeScoresKernel, 1U, nullptr,
                        &globalSize, &localSize, 0U, nullptr, nullptr) != cl::success) {
                drainAndDisable("failed to dispatch OpenCL scoring kernel");
                return false;
            }
            if(!readBufferNonBlocking(m_before, beforeValues.data(), scoreBytes)
                    || !readBuffer(m_after, afterValues.data(), scoreBytes)) {
                drainAndDisable("failed to read OpenCL scoring results");
                return false;
            }
        }

        const std::uint64_t rgbaCount{static_cast<std::uint64_t>(m_width) * static_cast<std::uint64_t>(m_height) * 4ULL};
        const double scaledScore{score * 255.0};
        const std::uint64_t baseline{static_cast<std::uint64_t>(scaledScore * scaledScore * static_cast<double>(rgbaCount))};
        scores.resize(ranges.size());
        for(std::size_t i = 0; i < ranges.size(); ++i) {
            std::uint64_t total{baseline};
            total -= beforeValues[i];
            total += afterValues[i];
            scores[i] = std::sqrt(static_cast<double>(total) / static_cast<double>(rgbaCount)) / 255.0;
        }
        return true;
    }

    bool commit(const std::vector<geometrize::Scanline>& lines, const geometrize::rgba color)
    {
        if(!m_available || lines.empty()) {
            return m_available;
        }
        if(lines.size() > std::numeric_limits<std::uint32_t>::max()) {
            disable("commit scanline batch is too large");
            return false;
        }
        std::vector<geometrize::Scanline> normalized;
        if(!normalizeScanlines(lines, m_width, m_height, normalized)) {
            disable("commit contains unsupported scanlines");
            return false;
        }

        std::vector<DeviceScanline> deviceLines;
        deviceLines.reserve(normalized.size());
        for(const geometrize::Scanline& line : normalized) {
            deviceLines.push_back(DeviceScanline{line.y, line.x1, line.x2, 0});
        }
        const std::size_t bytes{deviceLines.size() * sizeof(DeviceScanline)};
        if(!ensureBuffer(m_lines, m_linesCapacity, bytes, cl::mem_read_only)
                || !writeBuffer(m_lines, deviceLines.data(), bytes)) {
            disable("failed to upload committed scanlines");
            return false;
        }

        const std::uint32_t lineCount{static_cast<std::uint32_t>(deviceLines.size())};
        const std::int32_t width{static_cast<std::int32_t>(m_width)};
        const std::uint32_t packed{packColor(color)};
        if(!setArg(m_commitKernel, 0, m_current)
                || !setArg(m_commitKernel, 1, m_lines)
                || !setScalarArg(m_commitKernel, 2, lineCount)
                || !setScalarArg(m_commitKernel, 3, width)
                || !setScalarArg(m_commitKernel, 4, packed)) {
            disable("failed to set commit kernel arguments");
            return false;
        }

        const std::size_t globalSize{workGroupSize};
        const std::size_t localSize{workGroupSize};
        // No finish() here. The queue is in-order (created with properties 0),
        // so every later command that touches m_current - the next batch's
        // kernels, resetCurrent, teardown - is already ordered behind this
        // kernel. Draining the whole pipeline once per accepted shape only
        // bought a stall. The scanline upload above stays blocking because its
        // host buffer dies with this call.
        if(m_api.enqueueNDRangeKernel(m_queue, m_commitKernel, 1U, nullptr, &globalSize, &localSize, 0U, nullptr, nullptr) != cl::success) {
            disable("failed to commit current bitmap on OpenCL device");
            return false;
        }
        return true;
    }

    bool resetCurrent(const geometrize::Bitmap& current)
    {
        if(!m_available || current.getWidth() != m_width || current.getHeight() != m_height) {
            return false;
        }
        if(!writeBuffer(m_current, current.getDataRef().data(), current.getDataRef().size())) {
            disable("failed to reset current bitmap on OpenCL device");
            return false;
        }
        return true;
    }

    bool isAvailable() const { return m_available; }
    const std::string& getDeviceName() const { return m_deviceName; }
    const std::string& getLastError() const { return m_lastError; }

private:
    struct Api
    {
        cl::get_platform_ids_fn getPlatformIDs{};
        cl::get_platform_info_fn getPlatformInfo{};
        cl::get_device_ids_fn getDeviceIDs{};
        cl::get_device_info_fn getDeviceInfo{};
        cl::create_context_fn createContext{};
        cl::release_context_fn releaseContext{};
        cl::create_command_queue_fn createCommandQueue{};
        cl::release_command_queue_fn releaseCommandQueue{};
        cl::create_program_with_source_fn createProgramWithSource{};
        cl::build_program_fn buildProgram{};
        cl::get_program_build_info_fn getProgramBuildInfo{};
        cl::release_program_fn releaseProgram{};
        cl::create_kernel_fn createKernel{};
        cl::release_kernel_fn releaseKernel{};
        cl::create_buffer_fn createBuffer{};
        cl::release_mem_object_fn releaseMemObject{};
        cl::set_kernel_arg_fn setKernelArg{};
        cl::enqueue_write_buffer_fn enqueueWriteBuffer{};
        cl::enqueue_read_buffer_fn enqueueReadBuffer{};
        cl::enqueue_nd_range_kernel_fn enqueueNDRangeKernel{};
        cl::finish_fn finish{};
    };

    template<typename T>
    bool loadFunction(T& destination, const char* name)
    {
        destination = reinterpret_cast<T>(GetProcAddress(m_library, name));
        if(destination == nullptr) {
            m_lastError = std::string{"missing OpenCL entry point: "} + name;
            return false;
        }
        return true;
    }

    bool loadRuntime()
    {
        m_library = LoadLibraryExW(L"OpenCL.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if(m_library == nullptr) {
            m_lastError = "OpenCL.dll is unavailable";
            return false;
        }
        return loadFunction(m_api.getPlatformIDs, "clGetPlatformIDs")
            && loadFunction(m_api.getPlatformInfo, "clGetPlatformInfo")
            && loadFunction(m_api.getDeviceIDs, "clGetDeviceIDs")
            && loadFunction(m_api.getDeviceInfo, "clGetDeviceInfo")
            && loadFunction(m_api.createContext, "clCreateContext")
            && loadFunction(m_api.releaseContext, "clReleaseContext")
            && loadFunction(m_api.createCommandQueue, "clCreateCommandQueue")
            && loadFunction(m_api.releaseCommandQueue, "clReleaseCommandQueue")
            && loadFunction(m_api.createProgramWithSource, "clCreateProgramWithSource")
            && loadFunction(m_api.buildProgram, "clBuildProgram")
            && loadFunction(m_api.getProgramBuildInfo, "clGetProgramBuildInfo")
            && loadFunction(m_api.releaseProgram, "clReleaseProgram")
            && loadFunction(m_api.createKernel, "clCreateKernel")
            && loadFunction(m_api.releaseKernel, "clReleaseKernel")
            && loadFunction(m_api.createBuffer, "clCreateBuffer")
            && loadFunction(m_api.releaseMemObject, "clReleaseMemObject")
            && loadFunction(m_api.setKernelArg, "clSetKernelArg")
            && loadFunction(m_api.enqueueWriteBuffer, "clEnqueueWriteBuffer")
            && loadFunction(m_api.enqueueReadBuffer, "clEnqueueReadBuffer")
            && loadFunction(m_api.enqueueNDRangeKernel, "clEnqueueNDRangeKernel")
            && loadFunction(m_api.finish, "clFinish");
    }

    std::string getPlatformString(const cl::platform_id platform, const cl::platform_info field) const
    {
        std::size_t size{0};
        if(m_api.getPlatformInfo(platform, field, 0, nullptr, &size) != cl::success || size == 0) {
            return {};
        }
        std::string result(size, '\0');
        if(m_api.getPlatformInfo(platform, field, size, result.data(), nullptr) != cl::success) {
            return {};
        }
        while(!result.empty() && result.back() == '\0') {
            result.pop_back();
        }
        return result;
    }

    std::string getDeviceString(const cl::device_id device, const cl::device_info field) const
    {
        std::size_t size{0};
        if(m_api.getDeviceInfo(device, field, 0, nullptr, &size) != cl::success || size == 0) {
            return {};
        }
        std::string result(size, '\0');
        if(m_api.getDeviceInfo(device, field, size, result.data(), nullptr) != cl::success) {
            return {};
        }
        while(!result.empty() && result.back() == '\0') {
            result.pop_back();
        }
        return result;
    }

    template<typename T>
    bool getDeviceValue(const cl::device_id device, const cl::device_info field, T& value) const
    {
        return m_api.getDeviceInfo(device, field, sizeof(T), &value, nullptr) == cl::success;
    }

    bool selectDevice()
    {
        cl::uint_t platformCount{0};
        if(m_api.getPlatformIDs(0, nullptr, &platformCount) != cl::success || platformCount == 0) {
            m_lastError = "no OpenCL platforms found";
            return false;
        }
        std::vector<cl::platform_id> platforms(platformCount);
        if(m_api.getPlatformIDs(platformCount, platforms.data(), nullptr) != cl::success) {
            m_lastError = "failed to enumerate OpenCL platforms";
            return false;
        }

        const std::string deviceOverride{gpuDeviceOverride()};
        std::uint64_t bestScore{0};
        for(const cl::platform_id platform : platforms) {
            cl::uint_t deviceCount{0};
            const cl::int_t countResult{m_api.getDeviceIDs(platform, cl::device_type_gpu, 0, nullptr, &deviceCount)};
            if((countResult != cl::success && countResult != cl::device_not_found) || deviceCount == 0) {
                continue;
            }
            std::vector<cl::device_id> devices(deviceCount);
            if(m_api.getDeviceIDs(platform, cl::device_type_gpu, deviceCount, devices.data(), nullptr) != cl::success) {
                continue;
            }
            for(const cl::device_id device : devices) {
                cl::bool_t available{cl::false_value};
                cl::bool_t compilerAvailable{cl::false_value};
                cl::bool_t unified{cl::true_value};
                cl::uint_t computeUnits{0};
                std::size_t maxWorkGroupSize{0};
                getDeviceValue(device, cl::device_available, available);
                getDeviceValue(device, cl::device_compiler_available, compilerAvailable);
                getDeviceValue(device, cl::device_host_unified_memory, unified);
                getDeviceValue(device, cl::device_max_compute_units, computeUnits);
                getDeviceValue(device, cl::device_max_work_group_size, maxWorkGroupSize);
                if(available == cl::false_value || compilerAvailable == cl::false_value || maxWorkGroupSize < workGroupSize) {
                    continue;
                }
                const std::string platformName{getPlatformString(platform, cl::platform_name)};
                const std::string deviceName{getDeviceString(device, cl::device_name)};
                if(!deviceOverride.empty()) {
                    std::string searchable{platformName + " " + deviceName};
                    std::transform(searchable.begin(), searchable.end(), searchable.begin(), [](const unsigned char c) { return static_cast<char>(std::tolower(c)); });
                    if(searchable.find(deviceOverride) == std::string::npos) {
                        continue;
                    }
                }
                const std::uint64_t score{static_cast<std::uint64_t>(computeUnits) * (unified == cl::false_value ? 1000ULL : 1ULL)};
                if(score > bestScore) {
                    bestScore = score;
                    m_platform = platform;
                    m_device = device;
                    m_deviceName = deviceName;
                    if(!platformName.empty()) {
                        m_deviceName += " (" + platformName + ")";
                    }
                }
            }
        }
        if(m_device == nullptr) {
            m_lastError = "no suitable OpenCL GPU found";
            return false;
        }
        return true;
    }

    bool createRuntime()
    {
        cl::int_t error{cl::success};
        const cl::context_properties properties[]{
            cl::context_platform,
            reinterpret_cast<cl::context_properties>(m_platform),
            0};
        m_context = m_api.createContext(properties, 1U, &m_device, nullptr, nullptr, &error);
        if(error != cl::success || m_context == nullptr) {
            m_lastError = "failed to create OpenCL context";
            return false;
        }
        m_queue = m_api.createCommandQueue(m_context, m_device, 0, &error);
        if(error != cl::success || m_queue == nullptr) {
            m_lastError = "failed to create OpenCL command queue";
            return false;
        }

        const std::size_t sourceLength{std::strlen(kernelSource)};
        m_program = m_api.createProgramWithSource(m_context, 1U, &kernelSource, &sourceLength, &error);
        if(error != cl::success || m_program == nullptr) {
            m_lastError = "failed to create OpenCL program";
            return false;
        }
        error = m_api.buildProgram(m_program, 1U, &m_device, "-cl-std=CL1.2", nullptr, nullptr);
        if(error != cl::success) {
            std::size_t logSize{0};
            m_api.getProgramBuildInfo(m_program, m_device, cl::program_build_log, 0, nullptr, &logSize);
            std::string log(logSize, '\0');
            if(logSize != 0) {
                m_api.getProgramBuildInfo(m_program, m_device, cl::program_build_log, logSize, log.data(), nullptr);
            }
            m_lastError = "OpenCL kernel compilation failed: " + log;
            return false;
        }

        m_computeScoresKernel = m_api.createKernel(m_program, "compute_scores", &error);
        if(error != cl::success || m_computeScoresKernel == nullptr) {
            m_lastError = "failed to create compute_scores kernel";
            return false;
        }
        m_computeColorPartialsKernel = m_api.createKernel(m_program, "compute_color_partials", &error);
        if(error != cl::success || m_computeColorPartialsKernel == nullptr) {
            m_lastError = "failed to create compute_color_partials kernel";
            return false;
        }
        m_finalizeColorsKernel = m_api.createKernel(m_program, "finalize_colors", &error);
        if(error != cl::success || m_finalizeColorsKernel == nullptr) {
            m_lastError = "failed to create finalize_colors kernel";
            return false;
        }
        m_computeErrorPartialsKernel = m_api.createKernel(m_program, "compute_error_partials", &error);
        if(error != cl::success || m_computeErrorPartialsKernel == nullptr) {
            m_lastError = "failed to create compute_error_partials kernel";
            return false;
        }
        m_commitKernel = m_api.createKernel(m_program, "commit_lines", &error);
        if(error != cl::success || m_commitKernel == nullptr) {
            m_lastError = "failed to create commit_lines kernel";
            return false;
        }
        return true;
    }

    bool uploadImages(const geometrize::Bitmap& target, const geometrize::Bitmap& current)
    {
        cl::int_t error{cl::success};
        const std::size_t bytes{target.getDataRef().size()};
        m_target = m_api.createBuffer(m_context, cl::mem_read_only, bytes, nullptr, &error);
        if(error != cl::success || m_target == nullptr) {
            m_lastError = "failed to allocate OpenCL target image";
            return false;
        }
        m_current = m_api.createBuffer(m_context, cl::mem_read_write, bytes, nullptr, &error);
        if(error != cl::success || m_current == nullptr) {
            m_lastError = "failed to allocate OpenCL current image";
            return false;
        }
        return writeBuffer(m_target, target.getDataRef().data(), bytes)
            && writeBuffer(m_current, current.getDataRef().data(), bytes);
    }

    bool ensureBuffer(cl::mem& buffer, std::size_t& capacity, const std::size_t requestedBytes, const cl::mem_flags flags)
    {
        if(buffer != nullptr && capacity >= requestedBytes) {
            return true;
        }
        if(buffer != nullptr) {
            m_api.releaseMemObject(buffer);
            buffer = nullptr;
            capacity = 0;
        }
        std::size_t newCapacity{1U};
        while(newCapacity < requestedBytes && newCapacity <= (std::numeric_limits<std::size_t>::max() / 2U)) {
            newCapacity *= 2U;
        }
        if(newCapacity < requestedBytes) {
            newCapacity = requestedBytes;
        }
        cl::int_t error{cl::success};
        buffer = m_api.createBuffer(m_context, flags, newCapacity, nullptr, &error);
        if(error != cl::success || buffer == nullptr) {
            return false;
        }
        capacity = newCapacity;
        return true;
    }

    bool writeBuffer(const cl::mem buffer, const void* data, const std::size_t bytes)
    {
        return m_api.enqueueWriteBuffer(m_queue, buffer, cl::true_value, 0, bytes, data, 0, nullptr, nullptr) == cl::success;
    }

    bool writeBufferNonBlocking(const cl::mem buffer, const void* data, const std::size_t bytes)
    {
        return m_api.enqueueWriteBuffer(m_queue, buffer, cl::false_value, 0, bytes, data, 0, nullptr, nullptr) == cl::success;
    }

    bool readBuffer(const cl::mem buffer, void* data, const std::size_t bytes)
    {
        return m_api.enqueueReadBuffer(m_queue, buffer, cl::true_value, 0, bytes, data, 0, nullptr, nullptr) == cl::success;
    }

    bool readBufferNonBlocking(const cl::mem buffer, void* data, const std::size_t bytes)
    {
        return m_api.enqueueReadBuffer(m_queue, buffer, cl::false_value, 0, bytes, data, 0, nullptr, nullptr) == cl::success;
    }

    bool setArg(const cl::kernel kernel, const cl::uint_t index, const cl::mem value)
    {
        return m_api.setKernelArg(kernel, index, sizeof(cl::mem), &value) == cl::success;
    }

    template<typename T>
    bool setScalarArg(const cl::kernel kernel, const cl::uint_t index, const T& value)
    {
        return m_api.setKernelArg(kernel, index, sizeof(T), &value) == cl::success;
    }

    void disable(const std::string& error)
    {
        m_lastError = error;
        m_available = false;
        std::cerr << "[Geometrize GPU] Falling back to CPU: " << error << std::endl;
    }

    void drainAndDisable(const std::string& error)
    {
        if(m_queue != nullptr && m_api.finish != nullptr) {
            static_cast<void>(m_api.finish(m_queue));
        }
        disable(error);
    }

    void releaseMem(cl::mem& value)
    {
        if(value != nullptr && m_api.releaseMemObject != nullptr) {
            m_api.releaseMemObject(value);
            value = nullptr;
        }
    }

    void releaseRuntime()
    {
        // Pending nonblocking transfers may still reference host staging
        // vectors owned by evaluate(). Drain before releasing device objects.
        if(m_queue != nullptr && m_api.finish != nullptr) {
            static_cast<void>(m_api.finish(m_queue));
        }
        releaseMem(m_partialErrors);
        releaseMem(m_packedColors);
        releaseMem(m_partialColors);
        releaseMem(m_after);
        releaseMem(m_before);
        releaseMem(m_ranges);
        releaseMem(m_lines);
        releaseMem(m_current);
        releaseMem(m_target);
        if(m_commitKernel != nullptr && m_api.releaseKernel != nullptr) {
            m_api.releaseKernel(m_commitKernel);
            m_commitKernel = nullptr;
        }
        if(m_computeErrorPartialsKernel != nullptr && m_api.releaseKernel != nullptr) {
            m_api.releaseKernel(m_computeErrorPartialsKernel);
            m_computeErrorPartialsKernel = nullptr;
        }
        if(m_finalizeColorsKernel != nullptr && m_api.releaseKernel != nullptr) {
            m_api.releaseKernel(m_finalizeColorsKernel);
            m_finalizeColorsKernel = nullptr;
        }
        if(m_computeColorPartialsKernel != nullptr && m_api.releaseKernel != nullptr) {
            m_api.releaseKernel(m_computeColorPartialsKernel);
            m_computeColorPartialsKernel = nullptr;
        }
        if(m_computeScoresKernel != nullptr && m_api.releaseKernel != nullptr) {
            m_api.releaseKernel(m_computeScoresKernel);
            m_computeScoresKernel = nullptr;
        }
        if(m_program != nullptr && m_api.releaseProgram != nullptr) {
            m_api.releaseProgram(m_program);
            m_program = nullptr;
        }
        if(m_queue != nullptr && m_api.releaseCommandQueue != nullptr) {
            m_api.releaseCommandQueue(m_queue);
            m_queue = nullptr;
        }
        if(m_context != nullptr && m_api.releaseContext != nullptr) {
            m_api.releaseContext(m_context);
            m_context = nullptr;
        }
        if(m_library != nullptr) {
            FreeLibrary(m_library);
            m_library = nullptr;
        }
        m_available = false;
    }

    Api m_api{};
    HMODULE m_library{};
    cl::platform_id m_platform{};
    cl::device_id m_device{};
    cl::context m_context{};
    cl::command_queue m_queue{};
    cl::program m_program{};
    cl::kernel m_computeScoresKernel{};
    cl::kernel m_computeColorPartialsKernel{};
    cl::kernel m_finalizeColorsKernel{};
    cl::kernel m_computeErrorPartialsKernel{};
    cl::kernel m_commitKernel{};
    cl::mem m_target{};
    cl::mem m_current{};
    cl::mem m_lines{};
    cl::mem m_ranges{};
    cl::mem m_before{};
    cl::mem m_after{};
    cl::mem m_partialColors{};
    cl::mem m_packedColors{};
    cl::mem m_partialErrors{};
    std::size_t m_linesCapacity{};
    std::size_t m_rangesCapacity{};
    std::size_t m_beforeCapacity{};
    std::size_t m_afterCapacity{};
    std::size_t m_partialColorsCapacity{};
    std::size_t m_packedColorsCapacity{};
    std::size_t m_partialErrorsCapacity{};
    // Scratch reused across evaluate() calls. A hill climb issues thousands of
    // batches per shape, so allocating these fresh every time costs more host
    // time than the kernels themselves. evaluate()/commit() are only ever
    // driven by the single coordinator thread in Model, never concurrently.
    std::vector<geometrize::Scanline> m_scratchNormalized{};
    std::vector<DeviceScanline> m_scratchFlatLines{};
    std::vector<CandidateRange> m_scratchRanges{};
    std::vector<std::uint64_t> m_scratchBefore{};
    std::vector<std::uint64_t> m_scratchAfter{};
    std::vector<DeviceErrorPartial> m_scratchPartialErrors{};
    std::uint32_t m_width{};
    std::uint32_t m_height{};
    bool m_available{false};
    std::string m_deviceName{};
    std::string m_lastError{};
};

#else

class OpenClEnergyEvaluator::Impl
{
public:
    Impl(const geometrize::Bitmap&, const geometrize::Bitmap&) {}
    bool isAvailable() const { return false; }
    const std::string& getDeviceName() const { return m_empty; }
    const std::string& getLastError() const { return m_error; }
    bool evaluate(const std::vector<std::vector<geometrize::Scanline>>&, std::uint8_t, double, std::vector<double>& scores) { scores.clear(); return false; }
    bool commit(const std::vector<geometrize::Scanline>&, geometrize::rgba) { return false; }
    bool resetCurrent(const geometrize::Bitmap&) { return false; }
private:
    std::string m_empty{};
    std::string m_error{"OpenCL support was not compiled into this build"};
};

#endif

OpenClEnergyEvaluator::OpenClEnergyEvaluator(const geometrize::Bitmap& target, const geometrize::Bitmap& current) :
    d{std::unique_ptr<OpenClEnergyEvaluator::Impl>(new OpenClEnergyEvaluator::Impl(target, current))}
{}

OpenClEnergyEvaluator::~OpenClEnergyEvaluator() = default;

bool OpenClEnergyEvaluator::isAvailable() const
{
    return d->isAvailable();
}

const std::string& OpenClEnergyEvaluator::getDeviceName() const
{
    return d->getDeviceName();
}

const std::string& OpenClEnergyEvaluator::getLastError() const
{
    return d->getLastError();
}

bool OpenClEnergyEvaluator::evaluate(
        const std::vector<std::vector<geometrize::Scanline>>& candidateLines,
        const std::uint8_t alpha,
        const double score,
        std::vector<double>& scores)
{
    return d->evaluate(candidateLines, alpha, score, scores);
}

bool OpenClEnergyEvaluator::commit(const std::vector<geometrize::Scanline>& lines, const geometrize::rgba color)
{
    return d->commit(lines, color);
}

bool OpenClEnergyEvaluator::resetCurrent(const geometrize::Bitmap& current)
{
    return d->resetCurrent(current);
}

}
}
