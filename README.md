# NOVA Recovery

Experimental recovery development project for Galaxian (Nothing Phone (3a) Lite).

> Current project is a host-buildable diagnostic and rendering core. It is not a flashable recovery image and does not access the device framebuffer.

## Current features
- C++17 core
- Logger: INFO/WARN/ERROR, timestamps, bounded in-memory log
- Host Canvas: RGB color, pixel access, clear, clipped rectangles
- Compact 5x7 ASCII bitmap font, scalable text
- GitHub Actions CI

## Build
```sh
cmake -S . -B build
cmake --build build
./build/nova-core
```

## Layout
- include/nova/logger.hpp and src/logger.cpp — logger
- include/nova/renderer.hpp and src/renderer.cpp — canvas rendering
- src/main.cpp — smoke test/demo
- .github/workflows/build.yml — CI

Planned: device framebuffer backend, display initialization research, input handling, recovery UI.
