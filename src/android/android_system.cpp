#include "android_system.hpp"
#include <cstdlib>

namespace the_stations {

std::string AndroidPlatformSystem::getAccountUserName() {
    const char* userEnv = std::getenv("USER");
    if (userEnv && std::string(userEnv) != "") {
        return std::string(userEnv);
    }
    return m_displayName;
}

void AndroidPlatformSystem::setAccountUserName(const std::string& name) {
    if (!name.empty()) {
        m_displayName = name;
    }
}

PlatformInputState AndroidPlatformSystem::pollInput() {
    PlatformInputState state{};
    state.isCrouching = false;
    return state;
}

bool AndroidPlatformSystem::requestBiometricPermissions() {
    return false;
}

} // namespace the_stations
