#pragma once
#include <string>
namespace nova {
class Canvas;
class RecoveryDisplay {
public:
    RecoveryDisplay() = default;
    ~RecoveryDisplay();
    RecoveryDisplay(const RecoveryDisplay&) = delete;
    RecoveryDisplay& operator=(const RecoveryDisplay&) = delete;
    bool initialize();
    bool present(const Canvas& canvas);
    void shutdown() noexcept;
    bool isInitialized() const noexcept;
    int width() const noexcept;
    int height() const noexcept;
    const std::string& lastError() const noexcept;
private:
    bool initialized_ = false;
    int width_ = 0;
    int height_ = 0;
    std::string lastError_;
};
}
