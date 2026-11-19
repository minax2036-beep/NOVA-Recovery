# NOVA Recovery

Experimental recovery development project for Galaxian (Nothing Phone (3a) Lite).

> Current code is a host-buildable rendering core. The Linux framebuffer backend is implemented but not yet verified on Galaxian; this is not a flashable recovery image.

## Features
- C++17 core and bounded logger
- Canvas RGB pixels, clipping, rectangles, scalable 5x7 ASCII text
- Linux fbdev backend: open device, query format/geometry, map framebuffer, convert RGB into bitfields, present canvas with bounds checks
- Host CI builds backend without touching hardware

## Host build
```sh
cmake -S . -B build
cmake --build build
./build/nova-core
```

## Files
- include/nova/renderer.hpp, src/renderer.cpp — Canvas and drawing
- include/nova/framebuffer.hpp, src/framebuffer.cpp — Linux framebuffer backend
- include/nova/logger.hpp, src/logger.cpp — logging
- src/main.cpp — host smoke test

## Notes
The framebuffer backend requires a Linux framebuffer device and permission to open it. The target recovery may use fbdev, DRM, or a vendor display stack; verify the device's display path before attempting integration. This backend is not used to write partitions or change device configuration.
