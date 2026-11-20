#include "nova/recovery_ui.hpp"
#include "nova/logger.hpp"
#include <cassert>
#include <iostream>

int main() {
    nova::RecoveryUI ui;
    nova::Canvas canvas(320, 240);

    assert(ui.screen() == nova::RecoveryScreen::MainMenu);
    assert(ui.selectedIndex() == 0);
    assert(ui.menuItems().size() == 6);

    ui.render(canvas);
    assert((canvas.getPixel(0, 0) == nova::Color{30, 205, 220}));
    assert((canvas.getPixel(10, 52) == nova::Color{27, 75, 105}));

    ui.handleKey(nova::RecoveryKey::Up);
    assert(ui.selectedIndex() == 5); // Wrap around.
    ui.handleKey(nova::RecoveryKey::Down);
    assert(ui.selectedIndex() == 0);

    ui.handleKey(nova::RecoveryKey::Down);
    ui.handleKey(nova::RecoveryKey::Select);
    assert(ui.screen() == nova::RecoveryScreen::Information);
    ui.render(canvas);
    ui.handleKey(nova::RecoveryKey::Back);
    assert(ui.screen() == nova::RecoveryScreen::MainMenu);

    for (int i = 0; i < 5; ++i) ui.handleKey(nova::RecoveryKey::Down);
    assert(ui.selectedIndex() == 5);
    ui.handleKey(nova::RecoveryKey::Select);
    assert(ui.screen() == nova::RecoveryScreen::Logs);
    ui.render(canvas);
    ui.handleKey(nova::RecoveryKey::Back);
    assert(ui.screen() == nova::RecoveryScreen::MainMenu);

    std::cout << "NOVA RecoveryUI tests: PASS\n";
    return 0;
}
