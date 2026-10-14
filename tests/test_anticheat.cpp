#include <iostream>
#include <fstream>
#include <cassert>
#include "shared/anticheat/escape_anticheat.hpp"

using namespace the_stations;

int main() {
    EscapeAndAntiCheatEngine engine;

    // 1. Create mock binary file
    std::string binPath = "mock_game_binary.exe";
    {
        std::ofstream file(binPath, std::ios::binary);
        file << "ORIGINAL_GAME_BINARY_PAYLOAD_3D_ENGINE_2026";
    }

    // Compute legitimate SHA256
    std::string originalHash = engine.computeFileSHA256(binPath);
    assert(!originalHash.empty());

    // Verify legitimate check passes
    bool passVal = engine.verifyExecutableFileSHA256(binPath, originalHash);
    assert(passVal);
    assert(!engine.isTrappedUser());

    // 2. Intentional binary tampering test
    {
        std::ofstream file(binPath, std::ios::binary);
        file << "CRACKED_TAMPERED_GAME_BINARY_PAYLOAD_3D_ENGINE_2026";
    }

    // Verify tampered check fails and sets g_IsTrappedUser = true
    bool failVal = engine.verifyExecutableFileSHA256(binPath, originalHash);
    assert(!failVal);
    assert(engine.isTrappedUser());

    // Verify random Trap B or Trap C selection
    AntiCheatTrapType trap = engine.triggerTrappedUserTrap(777);
    assert(trap == AntiCheatTrapType::TrapB_InstantDeathLoop || trap == AntiCheatTrapType::TrapC_InfiniteLoop);

    std::remove(binPath.c_str());

    std::cout << "[PASS] Anti-Cheat Tampering & Trap Trigger Verification Test" << std::endl;
    return 0;
}
