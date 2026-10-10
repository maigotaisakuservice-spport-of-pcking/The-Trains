#include <iostream>
#include <cassert>
#include "shared/ui/ui_manager.hpp"

using namespace the_stations;

int main() {
    UIManager ui;
    assert(ui.getCurrentState() == ScreenState::Title);

    // Navigate to mode select
    ui.navigateTo(ScreenState::ModeSelect);
    assert(ui.getCurrentState() == ScreenState::ModeSelect);

    // Select singleplayer -> transitions to cutscene
    ui.selectGameMode(GameMode::SinglePlayer);
    assert(ui.getCurrentState() == ScreenState::GameplayCutscene);

    // Test private lobby creation
    ui.createPrivateLobby();
    assert(ui.getCurrentState() == ScreenState::PrivateLobby);
    assert(ui.getRoomCode().length() == 5);
    assert(ui.getLobbyPlayers().size() == 1);

    // Test joining private lobby
    UIManager guestUI;
    bool joined = guestUI.joinPrivateLobby(ui.getRoomCode());
    assert(joined);
    assert(guestUI.getCurrentState() == ScreenState::PrivateLobby);
    assert(guestUI.getRoomCode() == ui.getRoomCode());

    // Test ASCII HUD rendering contains no HUD (0%) during gameplay
    guestUI.navigateTo(ScreenState::Gameplay);
    std::string renderOutput = guestUI.renderCurrentScreen();
    assert(renderOutput.find("HUD 0%") != std::string::npos);

    std::cout << "[PASS] UI State Machine Test" << std::endl;
    return 0;
}
