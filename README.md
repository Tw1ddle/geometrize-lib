[![Geometrize Logo](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/logo.png?raw=true "Geometrize logo")](http://www.geometrize.co.uk/)

[![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](https://github.com/Tw1ddle/geometrize-lib/blob/master/LICENSE)
[![Coverity Scan Status](https://scan.coverity.com/projects/12991/badge.svg)](https://scan.coverity.com/projects/geometrize)

[Geometrize](http://www.geometrize.co.uk/) is a C++ library based on [primitive](https://github.com/fogleman/primitive). It recreates images as geometric primitives.

[![Geometrized Trees 210 Ellipses](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/tree_under_clouds.png?raw=true "Tree Under Clouds - 210 Ellipses")](http://www.geometrize.co.uk/)

## Features

 * Geometrize images into shapes.
 * Export the results to SVG, JSON, bitmaps and more.

## Usage

Examine the minimal [example](https://github.com/Tw1ddle/geometrize-lib-example) project code and read the documentation [here](http://tw1ddle.github.io/geometrize-lib-docs/). Also examine projects that use it below.

## Build Status

| Component                                                          | Build Status
|--------------------------------------------------------------------|-------------------------------------------|
| [Geometrize App](https://github.com/Tw1ddle/geometrize)            | [![Travis Geometrize App Build Status](https://img.shields.io/travis/Tw1ddle/geometrize.svg?style=flat-square)](https://travis-ci.org/Tw1ddle/geometrize) [![AppVeyor Geometrize App Build Status](https://ci.appveyor.com/api/projects/status/09l5nquksmev8ta4?svg=true)]
| [Example](https://github.com/Tw1ddle/geometrize-lib-example)       | [![Travis Geometrize Example Build Status](https://img.shields.io/travis/Tw1ddle/geometrize-lib-example.svg?style=flat-square)](https://travis-ci.org/Tw1ddle/geometrize-lib-example) [![AppVeyor Geometrize Example Build Status](https://ci.appveyor.com/api/projects/status/tav5nu3isxvdjkbh?svg=true)](https://ci.appveyor.com/project/Tw1ddle/geometrize-lib-example)
| [Fuzz Tests](https://github.com/Tw1ddle/geometrize-lib-fuzzing)    | [![Travis Fuzzing Build Status](https://img.shields.io/travis/Tw1ddle/geometrize-lib-fuzzing.svg?style=flat-square)](https://travis-ci.org/Tw1ddle/geometrize-lib-fuzzing) [![AppVeyor Fuzzing Build Status](https://ci.appveyor.com/api/projects/status/ebc5hbfu0mtofdom?svg=true)](https://ci.appveyor.com/project/Tw1ddle/geometrize-lib-fuzzing)
| [Unit Tests](https://github.com/Tw1ddle/geometrize-lib-unit-tests) | [![Travis Unit Test Build Status](https://img.shields.io/travis/Tw1ddle/geometrize-lib-unit-tests.svg?style=flat-square)](https://travis-ci.org/Tw1ddle/geometrize-lib-unit-tests) [![AppVeyor Unit Test Build Status](https://ci.appveyor.com/api/projects/status/github/Tw1ddle/geometrize-lib-unit-tests?branch=master&svg=true)](https://ci.appveyor.com/project/Tw1ddle/geometrize-lib-unit-tests)
| [Documentation](https://github.com/Tw1ddle/geometrize-lib-docs)    | [![Travis Documentation Build Status](https://img.shields.io/travis/Tw1ddle/geometrize-lib-docs.svg?style=flat-square)](https://travis-ci.org/Tw1ddle/geometrize-lib-docs)

## Shape Comparison

Geometrize supports rectangles, rotated rectangles, triangles, circles, ellipses, rotated ellipses, lines, polylines and quadratic beziers.

The matrix below shows typical results for a combination of circles, triangles, rotated rectangles, rotated ellipses and all supported shapes at 50, 200 and 500 total shapes:

| -                  | 50 Shapes     | 200 Shapes    | 500 Shapes   |
| ------------------ | ------------- | ------------- | ------------ |
| Circles            | [![50 Circles](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_50_circles.png?raw=true)](http://www.geometrize.co.uk/) | [![200 Circles](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_200_circles.png?raw=true)](http://www.geometrize.co.uk/) | [![500 Circles](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_500_circles.png?raw=true)](http://www.geometrize.co.uk/) |
| Triangles          | [![50 Triangles](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_50_triangles.png?raw=true)](http://www.geometrize.co.uk/) | [![200 Triangles](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_200_triangles.png?raw=true)](http://www.geometrize.co.uk/) | [![500 Triangles](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_500_triangles.png?raw=true)](http://www.geometrize.co.uk/) |
| Rotated Rectangles | [![50 Rotated Rectangles](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_50_rotated_rectangles.png?raw=true)](http://www.geometrize.co.uk/) | [![200 Rotated Rectangles](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_200_rotated_rectangles.png?raw=true)](http://www.geometrize.co.uk/) | [![500 Rotated Rectangles](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_500_rotated_rectangles.png?raw=true)](http://www.geometrize.co.uk/) |
| Rotated Ellipses   | [![50 Rotated Ellipses](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_50_rotated_ellipses.png?raw=true)](http://www.geometrize.co.uk/) | [![200 Rotated Ellipses](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_200_rotated_ellipses.png?raw=true)](http://www.geometrize.co.uk/) | [![500 Rotated Ellipses](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_500_rotated_ellipses.png?raw=true)](http://www.geometrize.co.uk/) |
| All Shapes         | [![50 All Shapes](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_50_all_shapes.png?raw=true)](http://www.geometrize.co.uk/) | [![200 All Shapes](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_200_all_shapes.png?raw=true)](http://www.geometrize.co.uk/) | [![500 All Shapes](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/seagull_500_all_shapes.png?raw=true)](http://www.geometrize.co.uk/) |

## Screenshots

For examples of geometrized images, see the [gallery](http://gallery.geometrize.co.uk/).

## Resources

 * See the Geometrize [resources](http://resources.geometrize.co.uk/) page.

## Notes
 * Got an idea or suggestion? Open an issue on GitHub, or send Sam a message on [Twitter](https://twitter.com/Sam_Twidale).