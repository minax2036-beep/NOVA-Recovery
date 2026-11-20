#pragma once

#include "nova/renderer.hpp"
#include <cstddef>
#include <string>
#include <vector>

namespace nova {

enum class RecoveryKey {
    Up,
    Down,
    Select,
    Back
};

enum class RecoveryScreen {
    MainMenu,
    Information,
    Logs
};

struct MenuItem {
    std::string title;
    std::string description;
};

class RecoveryUI {
public:
    RecoveryUI();

    void render(Canvas& canvas) const;
    void handleKey(RecoveryKey key);

    RecoveryScreen screen() const noexcept { return screen_; }
    std::size_t selectedIndex() const noexcept { return selected_; }
    const std::string& message() const noexcept { return message_; }
    const std::vector<MenuItem>& menuItems() const noexcept { return menu_; }

private:
    void renderMain(Canvas& canvas) const;
    void renderInformation(Canvas& canvas) const;
    void renderLogs(Canvas& canvas) const;
    void moveSelection(int delta);
    void selectItem();
    void showInformation(const std::string& title, const std::string& body);
    static std::string clipped(const std::string& text, std::size_t maxChars);

    std::vector<MenuItem> menu_;
    std::size_t selected_ = 0;
    RecoveryScreen screen_ = RecoveryScreen::MainMenu;
    std::string infoTitle_;
    std::string infoBody_;
    std::string message_ = "UI initialized";
};

} // namespace nova
