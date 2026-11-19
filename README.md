# NOVA Recovery

Experimental custom recovery development for Nothing Phone (3a) Lite / Galaxian.

## Current stage

- C++17 core prototype
- Lightweight logger (INFO/WARN/ERROR)
- GitHub Actions host build
- No device-specific recovery image yet

## Build locally

```bash
cmake -S . -B build
cmake --build build
./build/nova-core
```

This currently builds a host-side core prototype. It is not a flashable or bootable recovery image.

## Planned milestones

1. Core logging and diagnostics
2. UI rendering foundation
3. Input handling
4. Device diagnostics
5. Recovery-platform integration and controlled device testing

## Safety

Early testing is intended to use temporary boot workflows and non-destructive diagnostics. Avoid partition writes or userdata operations during initial development.
