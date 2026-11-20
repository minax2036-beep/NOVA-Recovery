#include "nova/recovery_ui.hpp"
#include "nova/logger.hpp"

#include <algorithm>

namespace nova {
namespace {
constexpr Color Background{10, 14, 22};
constexpr Color Panel{20, 27, 39};
constexpr Color Accent{30, 205, 220};
constexpr Color Selected{27, 75, 105};
constexpr Color Text{235, 240, 246};
constexpr Color Muted{130, 150, 170};
constexpr Color Warning{240, 190, 70};

void drawHeader(Canvas& canvas, const std::string& title) {
    canvas.fillRect(0, 0, canvas.width(), 3, Accent);
    canvas.fillRect(0, 3, canvas.width(), 26, Panel);
    canvas.drawText(10, 10, "NOVA RECOVERY", Text);
    canvas.drawText(canvas.width() - static_cast<int>(title.size()) * 6 - 8, 10, title, Accent);
}
}

RecoveryUI::RecoveryUI() : menu_{
    {"REBOOT", "Restart the device"},
    {"APPLY UPDATE", "Install an update package"},
    {"WIPE", "Factory reset options"},
    {"MOUNT", "Storage mount status"},
    {"ADVANCED", "Additional tools"},
    {"LOGS", "View recovery log"}
} {
    Logger::info("RecoveryUI initialized");
}

void RecoveryUI::render(Canvas& canvas) const {
    canvas.clear(Background);
    switch (screen_) {
        case RecoveryScreen::MainMenu: renderMain(canvas); break;
        case RecoveryScreen::Information: renderInformation(canvas); break;
        case RecoveryScreen::Logs: renderLogs(canvas); break;
    }
}

void RecoveryUI::renderMain(Canvas& canvas) const {
    drawHeader(canvas, "GALAXIAN");
    canvas.drawText(10, 38, "DEVICE: GALAXIAN / MT6878", Muted);
    const int itemHeight = 25;
    const int startY = 54;
    for (std::size_t i = 0; i < menu_.size(); ++i) {
        const int y = startY + static_cast<int>(i) * itemHeight;
        if (i == selected_) {
            canvas.fillRect(7, y - 2, canvas.width() - 14, itemHeight - 2, Selected);
            canvas.fillRect(7, y - 2, 3, itemHeight - 2, Accent);
        }
        canvas.drawText(16, y + 3, menu_[i].title, i == selected_ ? Accent : Text);
    }
    canvas.fillRect(0, canvas.height() - 22, canvas.width(), 1, Panel);
    canvas.drawText(8, canvas.height() - 14, "UP/DOWN: MOVE  POWER: SELECT", Muted);
}

void RecoveryUI::renderInformation(Canvas& canvas) const {
    drawHeader(canvas, "INFO");
    canvas.fillRect(8, 38, canvas.width() - 16, canvas.height() - 72, Panel);
    canvas.drawText(16, 48, infoTitle_, Accent);
    canvas.drawText(16, 68, clipped(infoBody_, 48), Text);
    canvas.drawText(16, 92, "NO DEVICE ACTION WAS EXECUTED", Warning);
    canvas.drawText(8, canvas.height() - 14, "BACK: RETURN", Muted);
}

void RecoveryUI::renderLogs(Canvas& canvas) const {
    drawHeader(canvas, "LOGS");
    const auto entries = Logger::snapshot();
    const std::size_t maxLines = 15;
    const std::size_t begin = entries.size() > maxLines ? entries.size() - maxLines : 0;
    int y = 38;
    for (std::size_t i = begin; i < entries.size(); ++i) {
        Color color = Muted;
        if (entries[i].level == LogLevel::Warning) color = Warning;
        if (entries[i].level == LogLevel::Error) color = colors::Red;
        canvas.drawText(8, y, clipped(entries[i].message, 50), color);
        y += 12;
        if (y > canvas.height() - 20) break;
    }
    canvas.drawText(8, canvas.height() - 14, "BACK: RETURN", Muted);
}

void RecoveryUI::handleKey(RecoveryKey key) {
    if (screen_ != RecoveryScreen::MainMenu) {
        if (key == RecoveryKey::Back || key == RecoveryKey::Select) {
            screen_ = RecoveryScreen::MainMenu;
            message_ = "Returned to main menu";
        }
        return;
    }

    switch (key) {
        case RecoveryKey::Up: moveSelection(-1); break;
        case RecoveryKey::Down: moveSelection(1); break;
        case RecoveryKey::Select: selectItem(); break;
        case RecoveryKey::Back: message_ = "Already at main menu"; break;
    }
}

void RecoveryUI::moveSelection(int delta) {
    const auto count = static_cast<int>(menu_.size());
    if (count == 0) return;
    int next = static_cast<int>(selected_) + delta;
    if (next < 0) next = count - 1;
    if (next >= count) next = 0;
    selected_ = static_cast<std::size_t>(next);
}

void RecoveryUI::selectItem() {
    const auto& item = menu_[selected_];
    Logger::info("Menu selected: " + item.title);
    if (item.title == "LOGS") {
        screen_ = RecoveryScreen::Logs;
        return;
    }

    const std::string body = item.title == "REBOOT"
        ? "Reboot is a UI placeholder in this build."
        : item.title == "WIPE"
        ? "Wipe actions are not implemented. No data will be changed."
        : item.title == "APPLY UPDATE"
        ? "Update installation is not implemented."
        : item.title == "MOUNT"
        ? "Mount controls are not implemented."
        : "Advanced tools are not implemented.";
    showInformation(item.title, body);
}

void RecoveryUI::showInformation(const std::string& title, const std::string& body) {
    infoTitle_ = title;
    infoBody_ = body;
    message_ = title + " opened (placeholder)";
    screen_ = RecoveryScreen::Information;
}

std::string RecoveryUI::clipped(const std::string& text, std::size_t maxChars) {
    if (text.size() <= maxChars) return text;
    if (maxChars <= 3) return text.substr(0, maxChars);
    return text.substr(0, maxChars - 3) + "...";
}

} // namespace nova
