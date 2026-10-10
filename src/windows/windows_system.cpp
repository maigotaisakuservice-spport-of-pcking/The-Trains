#include "windows_system.hpp"
#include <cstdlib>

#if defined(_WIN32)
#include <windows.h>
#include <lmcons.h>
#endif

namespace the_stations {

std::string WindowsPlatformSystem::getAccountUserName() {
#if defined(_WIN32)
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    if (GetUserNameA(username, &username_len)) {
        return std::string(username);
    }
#endif
    const char* userEnv = std::getenv("USERNAME");
    if (userEnv) {
        return std::string(userEnv);
    }
    return "Windows_User";
}

PlatformInputState WindowsPlatformSystem::pollInput() {
    PlatformInputState state{};
    state.moveForward = 1.0f;
    return state;
}

bool WindowsPlatformSystem::requestBiometricPermissions() {
    return true;
}

} // namespace the_stations
