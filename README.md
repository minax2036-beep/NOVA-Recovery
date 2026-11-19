# NOVA Recovery

Experimental recovery development project for Galaxian (Nothing Phone (3a) Lite).

> This repository is currently a host-buildable diagnostic core. It does not produce a flashable recovery image.

## Current features
- C++17 core
- Logger levels: INFO / WARN / ERROR
- Timestamped log entries
- Thread-safe in-memory log snapshot
- Bounded log buffer (256 entries)
- Console output
- GitHub Actions CI

## Host build

```sh
cmake -S . -B build
cmake --build build
./build/nova-core
```

## Layout
- `include/nova/logger.hpp` — logger API
- `src/logger.cpp` — implementation
- `src/main.cpp` — test/demo
- `.github/workflows/build.yml` — CI build

Planned: recovery display integration, input handling, diagnostic UI, device-specific development.
