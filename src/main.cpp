#include <iostream>
#include "shared/ui/ui_manager.hpp"

int main() {
    std::cout << "The Stations Game Engine Starting..." << std::endl;
    the_stations::UIManager uiManager;
    uiManager.renderCurrentScreen();
    return 0;
}
