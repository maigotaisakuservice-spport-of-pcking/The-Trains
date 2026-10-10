#ifndef THE_STATIONS_ESCAPE_ANTICHEAT_HPP
#define THE_STATIONS_ESCAPE_ANTICHEAT_HPP

#include <string>
#include <random>
#include <vector>

namespace the_stations {

enum class AntiCheatTrapType {
    None,
    TrapB_InstantDeathLoop,
    TrapC_InfiniteLoop
};

class EscapeAndAntiCheatEngine {
public:
    EscapeAndAntiCheatEngine() = default;

    // Check escape probability on passing doors/holes
    // Normal Escape: 1 / 2560
    // Special Escape Gimmick: 1 / 5000
    bool checkNormalEscapeChance(uint32_t seed);
    bool checkSpecialEscapeChance(uint32_t seed);

    // Production Binary File System SHA256 Verification
    // Reads executable file bytes directly from file system path and verifies against expected SHA256 hex string
    bool verifyExecutableFileSHA256(const std::string& executableFilePath, const std::string& expectedSHA256Hex);

    // Compute raw SHA256 string from file bytes
    std::string computeFileSHA256(const std::string& filePath);

    bool isTrappedUser() const { return m_isTrappedUser; }
    void setTrappedUserFlag(bool trapped) { m_isTrappedUser = trapped; }

    // Evaluates 50% random trigger for trapped users when stepping off train
    AntiCheatTrapType triggerTrappedUserTrap(uint32_t seed);

private:
    bool m_isTrappedUser{false};
};

} // namespace the_stations

#endif // THE_STATIONS_ESCAPE_ANTICHEAT_HPP
