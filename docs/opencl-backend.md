# OpenCL backend

Geometrize spends nearly all of its time in one place: scoring candidate shapes. Every model step
rasterizes thousands of candidates, computes the best average color under each one, blends it into
the current image and measures how much the error against the target changed. The code under
`geometrize/geometrize/gpu/` moves that inner loop onto an OpenCL device, scoring a whole
hill-climbing round in a single batch instead of one candidate at a time.

Two things are worth stating up front, because they bound everything else in this document:

* **The backend is compiled out by default.** A stock build of this library behaves exactly as it
  did before, on the CPU. See [Requirements](#requirements).
* **It does not change the output.** With the same seed and settings, the GPU path picks the same
  shapes in the same order as the CPU path. See [Determinism](#determinism).

## Requirements

|                     |                                                                                                                                                                        |
| ------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Platform            | **Windows only.** The runtime is loaded with `LoadLibraryExW(L"OpenCL.dll", …, LOAD_LIBRARY_SEARCH_SYSTEM32)`. There is no `dlopen` path for Linux or macOS.               |
| Compile-time        | **`GEOMETRIZE_OPENCL` must be defined.** It is the only switch; nothing else in the build changes.                                                                        |
| Build dependencies  | None. No OpenCL SDK, headers or import library are needed — the API types are redeclared in `gpu/opencl_types.h` and every entry point is resolved with `GetProcAddress`. |
| Runtime             | `OpenCL.dll` in `System32` (installed with the GPU driver) and a GPU device that reports an available compiler and a maximum work-group size of at least 256.             |

To enable it under qmake:

```qmake
DEFINES += GEOMETRIZE_OPENCL
```

Under any other build system, pass `-DGEOMETRIZE_OPENCL`.

`geometrize.pri` deliberately does not define it. Without the define — or on any non-Windows
platform — `OpenClEnergyEvaluator` compiles to an inert stub: `isAvailable()` returns `false`,
`getLastError()` returns `"OpenCL support was not compiled into this build"`, and every step runs on
the CPU. The new source files still compile, so the define is the only thing standing between a
CPU-only build and a GPU-capable one.

## How it plugs in

```
ImageRunner::step
  └── Model::stepWithGpuBatching(…, allowGpuBatching)
        └── Model::getHillClimbState        gates, batches and schedules lanes
              └── gpu::OpenClEnergyEvaluator::evaluate    scores a batch of candidates
              └── gpu::OpenClEnergyEvaluator::commit      blends an accepted shape on the device
```

`ImageRunner::step` opts in automatically, but only when the caller has **not** supplied its own
shape creator — the built-in creators are the ones known to produce non-overlapping scanlines inside
the target.

The pre-existing `Model::step` is untouched and always runs on the CPU. Callers that drive `Model`
directly and want batching use the new overload:

```cpp
std::vector<geometrize::ShapeResult> Model::stepWithGpuBatching(
        const std::function<std::shared_ptr<geometrize::Shape>(void)>& shapeCreator,
        std::uint8_t alpha,
        std::uint32_t shapeCount,
        std::uint32_t maxShapeMutations,
        std::uint32_t maxThreads,
        const geometrize::core::EnergyFunction& energyFunction,
        const geometrize::ShapeAcceptancePreconditionFunction& addShapePrecondition,
        bool allowGpuBatching);
```

Set `allowGpuBatching` only for a pure, thread-safe shape creator whose rasterized scanlines lie
inside the target and do not overlap. A custom energy function is always honored, and always on the
CPU.

`gpu::OpenClEnergyEvaluator` itself is a small, self-contained class (`gpu/opencl_energy_evaluator.h`):
`evaluate()` scores a batch of candidate scanline sets, `commit()` blends an accepted shape into the
device-resident current image, `resetCurrent()` re-uploads it, and `isAvailable()`, `getDeviceName()`
and `getLastError()` report what happened.

## When the GPU is actually used

A dispatch has a fixed cost, so the work has to be big enough to pay for it. Several gates decide,
and failing any one of them is not an error — it just means that piece of work runs on the CPU.

**Per step** (`Model::getHillClimbState`), all of these must hold:

* `allowGpuBatching` is set — i.e. `ImageRunner` with a built-in shape creator, or an explicit
  `stepWithGpuBatching(…, true)`
* no custom energy function was supplied
* the batch has at least **64** candidates: `maxThreads × (shapeCount + 2) ≥ 64`
* there is enough total work: `estimated evaluations × image pixels ≳ 2×10⁸`, where the estimate is
  `maxThreads × (shapeCount + maxShapeMutations + 2)`
* OpenCL initialization has not already failed for this model

**Per batch** (`Model::evaluateCandidateBatch`): the candidates in a batch must cover at least
**250 000** pixels between them. Sparse masks — lines, thin polylines, tiny shapes — are dominated by
launch and readback latency, so they are scored on the CPU while keeping the exact same lane and RNG
schedule the GPU path would have used.

**Per accepted shape** (`Model::step`): when there is neither a custom energy function nor a custom
acceptance precondition, the winning state's score is already the exact post-draw score, so the shape
is committed without copying the whole current bitmap to recompute and possibly roll back.

If initialization or any dispatch fails, the evaluator is dropped, a single line is written to
`stderr` —

```
[Geometrize GPU] Falling back to CPU: <reason>
```

— and the step is redone on the CPU. Handing out a mutable bitmap through `Model::getTarget()` or
`Model::getCurrent()` also drops the evaluator and rescores from scratch, since the caller may change
those pixels behind the model's back.

## Determinism

"Same results as the CPU" is a design constraint here, not a happy accident, and it is enforced in
two ways.

**The random sequence is reproduced, not approximated.** The scalar hill climber mutates a state,
scores it, and either accepts or rejects — so a batch evaluator that scores several mutations ahead
of time would consume random numbers the scalar algorithm never would. Instead each speculated
mutation is scored alongside a checkpoint of the thread-local generator
(`commonutil::captureRandomGeneratorState`). When one is accepted, its checkpoint is restored
(`commonutil::restoreRandomGeneratorState`), so the subsequent sequence is bit-identical to the
scalar path. `core::hillClimbState` is the entry point that lets a batch evaluator supply the initial
candidate while keeping the existing mutation and acceptance behavior.

**The arithmetic is integer, on both sides.** The kernels use the same 16-bit blend and the same
squared-error accumulation as `core.cpp`, summed in `ulong`. Nothing is reduced in floating point, so
the totals are exact rather than merely close — which is what makes the scores comparable
bit-for-bit instead of within a tolerance.

The commit that introduced the backend reports agreement with the CPU energy function verified across
**657 cases spanning all nine shape types**.

## Device selection

All platforms are enumerated and only GPU devices are considered. A device is skipped unless it is
available, has a compiler, and supports a work-group size of at least 256. Remaining devices are
ranked by compute units, multiplied by 1000 when the device does *not* report unified host memory —
so a discrete GPU is preferred over an integrated one. The program is built with `-cl-std=CL1.2`.
`getDeviceName()` reports the chosen device and its platform.

## Kernels

| Kernel                                     | Role                                                                                                                             |
| ------------------------------------------ | -------------------------------------------------------------------------------------------------------------------------------- |
| `compute_scores`                            | One work group per candidate. 256 work items stride along its scanlines, accumulate the color sums, blend, and reduce the squared error in a single pass. |
| `compute_color_partials` / `finalize_colors` | Split color reduction, for candidates large enough to spread across several work groups.                                          |
| `compute_error_partials`                    | Split error reduction for the same path.                                                                                          |
| `commit_lines`                              | Blends an accepted shape into the device-resident current image, so it never has to be re-uploaded between steps.                  |

Every kernel but `finalize_colors` is compiled with a required work-group size of 256;
`compute_scores` reduces through exactly 8 KiB of local memory. A candidate covering a large area can
be split across up to 32 work groups; the split is chosen automatically from the candidate count and
the covered pixel counts, and can be overridden.

## Environment variables

| Variable                    | Values                                     | Effect                                                                                                                       |
| --------------------------- | ------------------------------------------ | ---------------------------------------------------------------------------------------------------------------------------- |
| `GEOMETRIZE_GPU`            | `0`, `off`, `false`, `cpu`                 | Disables the backend entirely — everything runs on the CPU.                                                                    |
| `GEOMETRIZE_GPU_DEVICE`     | any substring                              | Case-insensitive substring matched against `"<platform name> <device name>"`. Only matching devices are considered.            |
| `GEOMETRIZE_GPU_MULTIGROUP` | `0`/`off`/`single`, `1`/`on`/`force`, `2`–`32` | Forces the multi-work-group split off, on with automatic sizing, or to that exact number of groups per candidate. Default: automatic. |
| `GEOMETRIZE_GPU_SPECULATION`| `1`–`16`                                   | Caps how many rejection-path mutations may be scored ahead per lane. **Default `1`, i.e. speculation is off**; raising it lets the age-based window (2 from age 8, 4 from age 24) take effect. |

Unrecognized values fall back to the default.

## CPU path improvements

Three changes that landed with the backend speed up the CPU path as well, and apply whether or not OpenCL is compiled in. All
three produce the same scanlines and the same scores as before.

* `core::defaultEnergyFunction` takes a fused path when a candidate's scanlines do not overlap:
  color and error are computed in one pass over the covered pixels, with no buffer bitmap copy.
  Overlapping scanlines — possible with custom shapes — keep the original buffered behavior, since
  those pixels must be blended repeatedly.
* Polygon rasterization accumulates the minimum and maximum outline x per row, instead of inserting
  every outline pixel into a per-row `std::set` (which allocated a tree node per pixel).
* Circle rasterization walks the radius once, O(r), instead of testing every pixel of the enclosing
  square, O(r²), and uses 64-bit products so large radii cannot overflow.

## Performance

The commit that introduced the backend measured **~19× over the CPU path at 736×1104 on an
RTX 4070 SUPER**. Expect the ratio to move a lot with image size, shape type and device: small
images, small batches and sparse shapes do not clear the thresholds above and stay on the CPU by
design.

## Limitations

* Windows only.
* Custom energy functions always run on the CPU.
* Custom shape creators are not batched through `ImageRunner`. Drive `Model::stepWithGpuBatching`
  directly if you need that, and only if your scanlines stay inside the target and do not overlap.
* The backend is an optimization of the built-in energy function, not a general compute path — there
  is no way to run arbitrary scoring code on the device.

## Credit

The OpenCL backend was contributed by [GleamEyes02](https://github.com/GleamEyes02).
