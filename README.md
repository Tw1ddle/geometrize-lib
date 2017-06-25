[![Geometrize Logo](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/geometrize_logo.gif?raw=true "Geometrize library logo animation")](https://github.com/Tw1ddle/geometrize-lib)

[![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](https://github.com/Tw1ddle/geometrize-lib/blob/master/LICENSE)
[![Travis Unit Test Build Status](https://img.shields.io/travis/Tw1ddle/geometrize-lib-unit-tests.svg?style=flat-square)](https://travis-ci.org/Tw1ddle/geometrize-lib-unit-tests)
[![AppVeyor Unit Test Build Status](https://ci.appveyor.com/api/projects/status/github/Tw1ddle/geometrize-lib-unit-tests?branch=master&svg=true)](https://ci.appveyor.com/project/Tw1ddle/geometrize-lib-unit-tests)
[![Travis Fuzzing Build Status](https://img.shields.io/travis/Tw1ddle/geometrize-lib-fuzzing.svg?style=flat-square)](https://travis-ci.org/Tw1ddle/geometrize-lib-fuzzing)
[![AppVeyor Fuzzing Build Status](https://ci.appveyor.com/api/projects/status/ebc5hbfu0mtofdom?svg=true)](https://ci.appveyor.com/project/Tw1ddle/geometrize-lib-fuzzing)
[![Travis Documentation Build Status](https://img.shields.io/travis/Tw1ddle/geometrize-lib-docs.svg?style=flat-square)](https://travis-ci.org/Tw1ddle/geometrize-lib-docs)
[![Codecov Code Coverage Build Status](https://codecov.io/github/Tw1ddle/geometrize-lib-unit-tests/coverage.svg?branch=master)](https://codecov.io/github/Tw1ddle/geometrize-lib-unit-tests?branch=master)
[![Coverity Scan Status](https://scan.coverity.com/projects/12991/badge.svg)](https://scan.coverity.com/projects/geometrize)

Geometrize is a C++ library for geometrizing images into geometric primitives. It is a work in progress - for now, I recommend using the [Haxe version](https://github.com/Tw1ddle/geometrize-haxe) instead.

## Features

 * Geometrize images into shapes.
 * Export as SVG, JSON, bitmap and others.

## How It Works

The algorithm finds shapes to approximate a target image. It first generates a large number of random candidate shapes, and then repeatedly improves the fit of the best shapes using a hillclimbing optimization approach, eventually picking the best fitting shape. Shapes are added one by one.

## Usage

More detail coming soon.

Read the documentation [here](http://tw1ddle.github.io/geometrize-lib-docs/).

## Resources

* See the Geometrize [resources](https://github.com/Tw1ddle/geometrize-resources) and [template](https://github.com/Tw1ddle/geometrize-templates) repositories.
* See the Geometrize [haxelib code](https://github.com/Tw1ddle/geometrize-haxe) and library [documentation](http://tw1ddle.github.io/geometrize-haxe/).
* See the Geometrize Haxe [web demo](http://www.samcodes.co.uk/project/geometrize-haxe-web/) and web [demo code](https://github.com/Tw1ddle/geometrize-haxe-web/).
* See the HaxeFlixel Geometrize [demo](http://samcodes.co.uk/project/geometrize-haxe-flixel/) and the [demo code](https://github.com/Tw1ddle/geometrize-haxe-demo/).
* See the Primitive Go [repository](https://github.com/fogleman/primitive).

## Examples and Screenshots

More coming soon.

[![Geometrized Stanczyk](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/stanczyk.jpg?raw=true "Stanczyk - 250 rects and ellipses")](http://i.imgur.com/y2OJTYf.jpg)

## Notes
 * Got an idea or suggestion? Open an issue on GitHub, or send Sam a message on [Twitter](https://twitter.com/Sam_Twidale).
 * Geometrize was originally based on [primitive](https://github.com/fogleman/primitive), a Go library created by [Michael Fogleman](https://github.com/fogleman).