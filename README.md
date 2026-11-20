# NOVA Recovery
Experimental recovery development project for Galaxian (Nothing Phone (3a) Lite).

> Current code is a host-buildable rendering core and display adapter. This is not a flashable recovery image.

## Recovery graphics investigation
The collected Galaxian Recovery files indicate:
- The Recovery executable dynamically links `librecovery_ui.so`.
- The vendor Recovery UI library exports `ScreenRecoveryUI` methods for screen update, drawing, menu, text, and surfaces.
- Recovery ueventd config creates `/dev/graphics`, `/dev/dri`, and `/dev/input` device nodes.
- Recovery module list includes MediaTek DRM and panel/display modules, including `mediatek-drm.ko`, `mediatek-drm-panel-drv.ko`, and `mtk_dpc.ko`.
- AOSP minui is the display abstraction used by Recovery UI; the actual active panel/backend still needs device runtime verification.

NOVA uses its own Canvas and a small minui adapter. It does not reuse or link Nothing's proprietary `librecovery_ui.so`.

## Architecture
- NOVA Canvas: RGB pixels, clipping, rectangles, 5x7 ASCII text.
- `RecoveryUI`: initial main menu, selection wrap-around, placeholder information screens, and a log viewer.
- Main menu entries: Reboot, Apply Update, Wipe, Mount, Advanced, Logs. These are UI placeholders only; no device operation is implemented.
- `RecoveryDisplay`: display abstraction.
- Android Recovery build: `recovery_display_minui.cpp`, using minui `gr_init`, framebuffer dimensions, `gr_color`, `gr_fill`, and `gr_flip`.
- Host CMake: stub backend; CI never accesses hardware.
- Linux fbdev backend remains a separate experiment, not assumed to be Galaxian's active path.

## Host build and tests
```sh
cmake -S . -B build
cmake --build build --parallel 2
ctest --test-dir build --output-on-failure
# Tests include Canvas/core smoke, minui adapter, and RecoveryUI navigation.
```

## Android integration
`Android.bp` defines recovery-available static library `libnova_recovery_display` with minui adapter and static dependency on `libminui`.

Build it inside a compatible Android Recovery source tree. This is a library component only: it is not yet connected to AOSP recovery's UI loop, touch/input handling, device configuration, ramdisk packaging, or boot image build.

## Safety/status
No partitions are written. Do not treat current sources as a bootable image.
