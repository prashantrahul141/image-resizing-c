# Image resizing in C.

Resize images using [Nearest Neighbor Interpolation](https://en.wikipedia.org/wiki/Nearest_neighbor_value_interpolation) algorithm.


### Building

This uses a simple Makefile, just run (with [make](https://www.gnu.org/software/make/) installed):

```sh
make
```

### Usage

```sh
Usage:
    main [scaling factor] [file path]
```

example:

```sh
./main 2 ./images/pixel_art.png
./main 4 ./images/arch.png
./main 20 ./images/2x2.png
```

### Images

[./images/pixel_art.png](https://github.com/prashantrahul141/image-resizing-c/blob/main/images/pixel_art.png) - Scene from KatanaZero.
