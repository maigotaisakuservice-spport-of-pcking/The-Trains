#include <iostream>
#include <fstream>
#include <cassert>
#include "shared/anticheat/escape_anticheat.hpp"

using namespace the_stations;

int main() {
    EscapeAndAntiCheatEngine engine;

    // Create a temporary test binary file on disk
    std::string testFilePath = "test_binary.bin";
    {
        std::ofstream outFile(testFilePath, std::ios::binary);
        outFile << "THE_STATIONS_PRODUCTION_BINARY_PAYLOAD_DATA_2026";
    }

    // Compute hash
    std::string calculatedHash = engine.computeFileSHA256(testFilePath);
    assert(!calculatedHash.empty());

    // Verify valid SHA256 file check
    bool isValid = engine.verifyExecutableFileSHA256(testFilePath, calculatedHash);
    assert(isValid);
    assert(!engine.isTrappedUser());

    // Verify tampered hash sets m_isTrappedUser flag
    bool isTampered = engine.verifyExecutableFileSHA256(testFilePath, "INCORRECT_HASH_9999");
    assert(!isTampered);
    assert(engine.isTrappedUser());

    // Verify Trap B / Trap C trigger selection
    AntiCheatTrapType trap = engine.triggerTrappedUserTrap(12345);
    assert(trap == AntiCheatTrapType::TrapB_InstantDeathLoop || trap == AntiCheatTrapType::TrapC_InfiniteLoop);

    // Cleanup
    std::remove(testFilePath.c_str());

    std::cout << "[PASS] Production Anti-Cheat File Binary Verification Test" << std::endl;
    return 0;
}
