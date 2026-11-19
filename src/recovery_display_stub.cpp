#include "nova/recovery_display.hpp"
#include "nova/renderer.hpp"
namespace nova {
RecoveryDisplay::~RecoveryDisplay() { shutdown(); }
bool RecoveryDisplay::initialize() {
    shutdown();
    lastError_ = "NOVA was built without the Android minui display backend";
    return false;
}
bool RecoveryDisplay::present(const Canvas&) {
    lastError_ = "display backend is unavailable";
    return false;
}
void RecoveryDisplay::shutdown() noexcept {
    initialized_ = false;
    width_ = height_ = 0;
}
bool RecoveryDisplay::isInitialized() const noexcept { return initialized_; }
int RecoveryDisplay::width() const noexcept { return width_; }
int RecoveryDisplay::height() const noexcept { return height_; }
const std::string& RecoveryDisplay::lastError() const noexcept { return lastError_; }
}
