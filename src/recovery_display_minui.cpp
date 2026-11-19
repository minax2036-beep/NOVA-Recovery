#include "nova/recovery_display.hpp"
#include "nova/renderer.hpp"
#include <cstdint>
#include <minui/minui.h>
namespace nova {
RecoveryDisplay::~RecoveryDisplay() { shutdown(); }
bool RecoveryDisplay::initialize() {
    shutdown();
    if (gr_init() != 0) {
        lastError_ = "minui gr_init() failed";
        return false;
    }
    width_ = gr_fb_width();
    height_ = gr_fb_height();
    if (width_ <= 0 || height_ <= 0) {
        gr_exit();
        width_ = height_ = 0;
        lastError_ = "minui reported invalid framebuffer dimensions";
        return false;
    }
    initialized_ = true;
    lastError_.clear();
    return true;
}
bool RecoveryDisplay::present(const Canvas& canvas) {
    if (!initialized_) {
        lastError_ = "display is not initialized";
        return false;
    }
    if (canvas.width() <= 0 || canvas.height() <= 0) {
        lastError_ = "canvas has invalid dimensions";
        return false;
    }
    // minui's gr_fill takes exclusive bottom-right coordinates, not width/height.
    // Convert Canvas into scaled horizontal color runs, then submit rectangles.
    const auto& pixels = canvas.pixels();
    for (int sy = 0; sy < canvas.height(); ++sy) {
        const int y0 = static_cast<int>((static_cast<int64_t>(sy) * height_) / canvas.height());
        const int y1 = static_cast<int>((static_cast<int64_t>(sy + 1) * height_) / canvas.height());
        if (y1 <= y0) continue;
        int sx = 0;
        while (sx < canvas.width()) {
            const Color color = pixels[static_cast<std::size_t>(sy) * canvas.width() + sx];
            int end = sx + 1;
            while (end < canvas.width() &&
                   pixels[static_cast<std::size_t>(sy) * canvas.width() + end] == color) ++end;
            const int x0 = static_cast<int>((static_cast<int64_t>(sx) * width_) / canvas.width());
            const int x1 = static_cast<int>((static_cast<int64_t>(end) * width_) / canvas.width());
            if (x1 > x0) {
                gr_color(color.r, color.g, color.b, 255);
                gr_fill(x0, y0, x1, y1);
            }
            sx = end;
        }
    }
    gr_flip();
    lastError_.clear();
    return true;
}
void RecoveryDisplay::shutdown() noexcept {
    if (initialized_) gr_exit();
    initialized_ = false;
    width_ = height_ = 0;
}
bool RecoveryDisplay::isInitialized() const noexcept { return initialized_; }
int RecoveryDisplay::width() const noexcept { return width_; }
int RecoveryDisplay::height() const noexcept { return height_; }
const std::string& RecoveryDisplay::lastError() const noexcept { return lastError_; }
}
