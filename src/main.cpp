#include "nova/logger.hpp"
#include "nova/renderer.hpp"
#include "nova/framebuffer.hpp"
#include "nova/recovery_ui.hpp"

#include <iostream>

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

    nova::FramebufferBackend backend;
    std::cout << "NOVA Recovery UI smoke test: PASS\n";
    std::cout << "Menu entries: " << ui.menuItems().size() << "\n";
    std::cout << "Canvas: " << canvas.width() << "x" << canvas.height() << "\n";
#if defined(__linux__)
    std::cout << "Framebuffer backend compiled: Linux fbdev\n";
#else
    std::cout << "Framebuffer backend compiled: unavailable on this host\n";
#endif
    std::cout << "Hardware framebuffer intentionally not accessed in CI.\n";
    return 0;
}
