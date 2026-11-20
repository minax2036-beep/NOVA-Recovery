#include "nova/logger.hpp"
#include "nova/renderer.hpp"
#include "nova/framebuffer.hpp"
#include "nova/recovery_ui.hpp"

#include <fstream>
#include <iostream>

namespace {
bool writePpmImage(const char* path, const nova::Canvas& canvas) {
    std::ofstream out(path, std::ios::binary);
    if (!out) return false;

    // PPM P6 image payload. The filename intentionally uses .img for this
    // development artifact; it is NOT an Android boot/recovery image.
    out << "P6\n" << canvas.width() << " " << canvas.height() << "\n255\n";
    for (const auto& pixel : canvas.pixels()) {
        const char rgb[] = {
            static_cast<char>(pixel.r),
            static_cast<char>(pixel.g),
            static_cast<char>(pixel.b)
        };
        out.write(rgb, sizeof(rgb));
    }
    return out.good();
}
}

int main() {
    nova::Logger::info("NOVA Core starting");

    nova::Canvas canvas(320, 240);
    nova::RecoveryUI ui;
    ui.render(canvas);

    if (ui.screen() != nova::RecoveryScreen::MainMenu ||
        ui.selectedIndex() != 0 ||
        canvas.getPixel(0, 0) != nova::Color{30, 205, 220}) {
        nova::Logger::error("Recovery UI smoke test failed");
        return 1;
    }

    if (!writePpmImage("test.img", canvas)) {
        nova::Logger::error("Failed to write test.img");
        return 1;
    }

    nova::FramebufferBackend backend;
    std::cout << "NOVA Recovery UI smoke test: PASS\n";
    std::cout << "Menu entries: " << ui.menuItems().size() << "\n";
    std::cout << "Canvas: " << canvas.width() << "x" << canvas.height() << "\n";
    std::cout << "Output: test.img (PPM P6, RGB888; development preview only)\n";
#if defined(__linux__)
    std::cout << "Framebuffer backend compiled: Linux fbdev\n";
#else
    std::cout << "Framebuffer backend compiled: unavailable on this host\n";
#endif
    std::cout << "Hardware framebuffer intentionally not accessed in CI.\n";
    return 0;
}
