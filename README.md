[![Geometrize Logo](https://github.com/Tw1ddle/geometrize-lib/blob/master/screenshots/geometrize_logo.png?raw=true "Geometrize library - tool for geometrizing images into geometric primitives logo")](https://github.com/Tw1ddle/geometrize-lib)

[![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](https://github.com/Tw1ddle/geometrize-lib/blob/master/LICENSE)

Geometrize is a C++ library for geometrizing images into geometric primitives. It is a work in progress - for now, I recommend the [Haxe version](https://github.com/Tw1ddle/geometrize-haxe) instead.

## Features

 * Geometrize images into shapes.
 * Export as SVG, JSON, bitmap and others.

## How It Works

A user provides a target image, and the algorithm finds good shapes to approximate that image. To find a good shape, the algorithm generates a large number of random candidate shapes, repeatedly improving the fit of each using a hillclimbing optimization approach, eventually choosing the best fitting shape. Shapes are added one by one.

## Usage

Coming soon.

## Resources

* See the Geometrize [resources](https://github.com/Tw1ddle/geometrize-resources) and [template](https://github.com/Tw1ddle/geometrize-templates) repositories.
* See the Geometrize [haxelib code](https://github.com/Tw1ddle/geometrize-haxe) and library [documentation](http://tw1ddle.github.io/geometrize-haxe/).
* See the Geometrize [web demo](http://www.samcodes.co.uk/project/geometrize-haxe-web/) and web [demo code](https://github.com/Tw1ddle/geometrize-haxe-web/).
* See the HaxeFlixel Geometrize [demo](http://samcodes.co.uk/project/geometrize-haxe-flixel/) and the [demo code](https://github.com/Tw1ddle/geometrize-haxe-demo/).
* See the Primitive Go [repository](https://github.com/fogleman/primitive).

## Examples and Screenshots

Coming soon.

## Notes
 * Got an idea or suggestion? Open an issue on GitHub, or send Sam a message on [Twitter](https://twitter.com/Sam_Twidale).
 * Geometrize was originally based on [primitive](https://github.com/fogleman/primitive), a Go library created by [Michael Fogleman](https://github.com/fogleman).