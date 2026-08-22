[![Geometrize Logo](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/logo.png?raw=true "Geometrize logo")](https://www.geometrize.co.uk/)

[![License](https://img.shields.io/:license-mit-blue.svg?style=flat-square)](https://github.com/Tw1ddle/geometrize-lib/blob/master/LICENSE)
[![Build Status Badge](https://ci.appveyor.com/api/projects/status/github/Tw1ddle/geometrize-lib)](https://ci.appveyor.com/project/Tw1ddle/geometrize-lib)

[Geometrize](https://www.geometrize.co.uk/) is a C++ library based on [primitive](https://github.com/fogleman/primitive). It recreates images as geometric primitives.

[![Geometrized Trees 210 Ellipses](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/tree_under_clouds.png?raw=true "Tree Under Clouds - 210 Ellipses")](https://www.geometrize.co.uk/)

## Features

 * Geometrize images into shapes.
 * Export the results as SVG, JSON and more.
 * Optional [OpenCL backend](docs/opencl-backend.md) that scores candidate shapes on the GPU, producing the same shapes as the CPU path.

## Usage

Refer to the minimal [example](https://github.com/Tw1ddle/geometrize-lib-example) project and read the [documentation](https://tw1ddle.github.io/geometrize-lib-docs/). These projects may also be useful references:

| Project                                                            |
|--------------------------------------------------------------------|
| [Geometrize App](https://github.com/Tw1ddle/geometrize)            |
| [Example](https://github.com/Tw1ddle/geometrize-lib-example)       |
| [Fuzz Tests](https://github.com/Tw1ddle/geometrize-lib-fuzzing)    |
| [Unit Tests](https://github.com/Tw1ddle/geometrize-lib-unit-tests) |
| [Documentation](https://github.com/Tw1ddle/geometrize-lib-docs)    |

See the [top level repo](https://github.com/Tw1ddle/geometrize-top-level-repo) for a listing of all the repositories included in the Geometrize project.

## OpenCL backend

Scoring candidate shapes is where almost all of the running time goes. This library ships an optional OpenCL backend that batches a whole hill-climbing round onto the GPU, and measured ~19x over the CPU path at 736x1104 on an RTX 4070 SUPER.

It is **compiled out by default** — a stock build is CPU-only and behaves exactly as it did before. To enable it, build on Windows with `GEOMETRIZE_OPENCL` defined:

```qmake
DEFINES += GEOMETRIZE_OPENCL
```

No OpenCL SDK is needed to build; the runtime is loaded from the installed GPU driver. With the same seed and settings the backend picks the same shapes in the same order as the CPU path, and anything it cannot handle — a custom energy function, a small image, a missing or failing OpenCL runtime — silently falls back to the CPU.

Read [docs/opencl-backend.md](docs/opencl-backend.md) for the gating rules, the determinism guarantees, device selection and the tuning environment variables.

## Shape Comparison

The matrix below shows typical results for a combination of circles, triangles, rotated rectangles, rotated ellipses and all supported shapes at 50, 200 and 500 total shapes:

| -                  | 50 Shapes     | 200 Shapes    | 500 Shapes   |
| ------------------ | ------------- | ------------- | ------------ |
| Circles            | [![50 Circles](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_50_circles.png?raw=true)](https://www.geometrize.co.uk/) | [![200 Circles](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_200_circles.png?raw=true)](https://www.geometrize.co.uk/) | [![500 Circles](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_500_circles.png?raw=true)](https://www.geometrize.co.uk/) |
| Triangles          | [![50 Triangles](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_50_triangles.png?raw=true)](https://www.geometrize.co.uk/) | [![200 Triangles](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_200_triangles.png?raw=true)](https://www.geometrize.co.uk/) | [![500 Triangles](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_500_triangles.png?raw=true)](https://www.geometrize.co.uk/) |
| Rotated Rectangles | [![50 Rotated Rectangles](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_50_rotated_rectangles.png?raw=true)](https://www.geometrize.co.uk/) | [![200 Rotated Rectangles](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_200_rotated_rectangles.png?raw=true)](https://www.geometrize.co.uk/) | [![500 Rotated Rectangles](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_500_rotated_rectangles.png?raw=true)](https://www.geometrize.co.uk/) |
| Rotated Ellipses   | [![50 Rotated Ellipses](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_50_rotated_ellipses.png?raw=true)](https://www.geometrize.co.uk/) | [![200 Rotated Ellipses](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_200_rotated_ellipses.png?raw=true)](https://www.geometrize.co.uk/) | [![500 Rotated Ellipses](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_500_rotated_ellipses.png?raw=true)](https://www.geometrize.co.uk/) |
| All Shapes         | [![50 All Shapes](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_50_all_shapes.png?raw=true)](https://www.geometrize.co.uk/) | [![200 All Shapes](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_200_all_shapes.png?raw=true)](https://www.geometrize.co.uk/) | [![500 All Shapes](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_500_all_shapes.png?raw=true)](https://www.geometrize.co.uk/) |

## Screenshots

See the [gallery](https://gallery.geometrize.co.uk/).

## Resources

See the Geometrize [resources](https://resources.geometrize.co.uk/) page.

## Notes
 * Got an idea or suggestion? Open an issue on GitHub, or send Sam a message on [Twitter](https://twitter.com/Sam_Twidale).
 * The OpenCL backend was contributed by [GleamEyes02](https://github.com/GleamEyes02).