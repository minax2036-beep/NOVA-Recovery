# NOVA Recovery
Experimental recovery development project for Galaxian (Nothing Phone (3a) Lite).

> Current code is a host-buildable rendering core and display adapter. This is not a flashable recovery image.

## Recovery graphics investigation
The collected Galaxian Recovery files indicate:
- The Recovery executable dynamically links `librecovery_ui.so`.
- The vendor Recovery UI library exports `ScreenRecoveryUI` methods for screen update, drawing, menu, text, and surfaces.
- Recovery ueventd config creates `/dev/graphics`, `/dev/dri`, and `/dev/input` device nodes.
- Recovery module list includes MediaTek DRM and panel/display modules, including `mediatek-drm.ko`, `mediatek-drm-panel-drv.ko`, and `mtk_dpc.ko`.
- The UI uses AOSP's minui-facing layer. Exact active panel/DRM node and runtime mode still require device verification.

NOVA uses its own Canvas and a small adapter to AOSP minui. It does not reuse or link Nothing's proprietary `librecovery_ui.so`.

## Architecture
- NOVA Canvas: RGB pixels, clipping, rectangles, 5x7 ASCII text.
- `RecoveryDisplay`: display abstraction.
- Android Recovery build: `recovery_display_minui.cpp`, using minui `gr_init`, framebuffer dimensions, `gr_color`, `gr_fill`, and `gr_flip`.
- Host CMake: stub backend; CI never accesses hardware.
- Linux fbdev backend remains an experimental separate backend and is not assumed to be Galaxian's active path.

## Host build
```sh
cmake -S . -B build
cmake --build build
./build/nova-core
```

## Android integration
`Android.bp` defines recovery-available static library `libnova_recovery_display` with minui adapter. Build it in a compatible Android Recovery source tree where `libminui` is available.

This is a library component only. It is not yet connected to AOSP recovery's UI loop, touch/input handling, device configuration, ramdisk packaging, or boot image build.

## Safety/status
No partitions are written. Do not treat current sources as a bootable image.
