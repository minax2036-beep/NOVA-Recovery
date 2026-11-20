#include "nova/renderer.hpp"

#include <fstream>
#include <iostream>
#include <string>

namespace {
using nova::Canvas;
using nova::Color;

constexpr Color Background{12, 18, 28};
constexpr Color Accent{38, 210, 225};
constexpr Color Text{225, 235, 245};
constexpr Color Muted{105, 130, 150};
constexpr Color Error{245, 85, 90};

bool writePpm(const char* path, const Canvas& canvas) {
    std::ofstream out(path, std::ios::binary);
    if (!out) return false;
    out << "P6\n" << canvas.width() << " " << canvas.height() << "\n255\n";
    for (const auto& pixel : canvas.pixels()) {
        const char rgb[] = {static_cast<char>(pixel.r),
                            static_cast<char>(pixel.g),
                            static_cast<char>(pixel.b)};
        out.write(rgb, sizeof(rgb));
    }
    return out.good();
}

void drawBootScreen(Canvas& canvas, bool failed, const char* failedStep) {
    canvas.clear(Background);
    canvas.fillRect(0, 0, canvas.width(), 4, Accent);
    canvas.fillRect(18, 28, 284, 1, Color{35, 55, 72});

    canvas.drawText(24, 42, "NOVA", Accent, 4);
    canvas.drawText(26, 78, "RECOVERY ALPHA 0", Text, 1);
    canvas.fillRect(24, 98, 272, 2, Accent);

    canvas.drawText(24, 122, "BOOT SEQUENCE", Muted, 1);
    canvas.drawText(24, 142, "DISPLAY INIT", failed && std::string(failedStep) == "display" ? Error : Text, 1);
    canvas.drawText(24, 158, "UI INIT", failed && std::string(failedStep) == "ui" ? Error : Text, 1);
    canvas.drawText(24, 174, "SYSTEM READY", failed ? Error : Accent, 1);

    canvas.fillRect(24, 196, 272, 8, Color{30, 45, 58});
    canvas.fillRect(24, 196, failed ? 72 : 272, 8, failed ? Error : Accent);

    canvas.drawText(24, 218, failed ? "BOOT ERROR - SIMULATED" : "HOST BOOT SIMULATION", failed ? Error : Muted, 1);
}
}

int main(int argc, char** argv) {
    bool failDisplay = false;
    bool failUI = false;
    for (int i = 1; i < argc; ++i) {
        const std::string arg(argv[i]);
        if (arg == "--fail-display") failDisplay = true;
        else if (arg == "--fail-ui") failUI = true;
        else {
            std::cerr << "Usage: " << argv[0] << " [--fail-display|--fail-ui]\n";
            return 2;
        }
    }

    std::cout << "[NOVA] Recovery Alpha 0 boot simulation\n";
    std::cout << "[NOVA] Display initialization...\n";
    if (failDisplay) {
        std::cerr << "[NOVA] ERROR: simulated display initialization failure\n";
        Canvas canvas(320, 240);
        drawBootScreen(canvas, true, "display");
        if (!writePpm("boot-preview.ppm", canvas)) {
            std::cerr << "[NOVA] ERROR: could not write preview\n";
        }
        return 1;
    }

    std::cout << "[NOVA] Display: simulated OK\n";
    std::cout << "[NOVA] UI initialization...\n";
    if (failUI) {
        std::cerr << "[NOVA] ERROR: simulated UI initialization failure\n";
        Canvas canvas(320, 240);
        drawBootScreen(canvas, true, "ui");
        if (!writePpm("boot-preview.ppm", canvas)) {
            std::cerr << "[NOVA] ERROR: could not write preview\n";
        }
        return 1;
    }

    std::cout << "[NOVA] UI: simulated OK\n";
    std::cout << "[NOVA] Status: READY\n";
    Canvas canvas(320, 240);
    drawBootScreen(canvas, false, "");
    if (!writePpm("boot-preview.ppm", canvas)) {
        std::cerr << "[NOVA] ERROR: could not write boot-preview.ppm\n";
        return 1;
    }

    std::cout << "[NOVA] Preview written: boot-preview.ppm (PPM, host-only)\n";
    std::cout << "NOVA boot simulation: PASS\n";
    return 0;
}
