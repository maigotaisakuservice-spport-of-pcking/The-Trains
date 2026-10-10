#include "mac_system.hpp"
#include <cstdlib>
#include <unistd.h>

namespace the_stations {

std::string MacPlatformSystem::getAccountUserName() {
    const char* userEnv = std::getenv("USER");
    if (userEnv) {
        return std::string(userEnv);
    }
    char loginName[256];
    if (getlogin_r(loginName, sizeof(loginName)) == 0) {
        return std::string(loginName);
    }
    return "Mac_User";
}

PlatformInputState MacPlatformSystem::pollInput() {
    PlatformInputState state{};
    state.moveForward = 1.0f;
    return state;
}

bool MacPlatformSystem::requestBiometricPermissions() {
    return true;
}

} // namespace the_stations
