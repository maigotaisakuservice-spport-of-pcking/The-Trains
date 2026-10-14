#ifndef THE_STATIONS_PLATFORM_INTERFACE_HPP
#define THE_STATIONS_PLATFORM_INTERFACE_HPP

#include <string>

namespace the_stations {

enum class PlatformType {
    Windows,
    Mac,
    Android
};

struct PlatformInputState {
    float moveForward{0.0f};
    float moveRight{0.0f};
    float cameraPitch{0.0f};
    float cameraYaw{0.0f};
    bool isCrouching{false};
    bool isGrabbing{false};
    bool isThrowing{false};
};

class IPlatformSystem {
public:
    virtual ~IPlatformSystem() = default;

    virtual PlatformType getPlatformType() const = 0;
    virtual std::string getAccountUserName() = 0;
    virtual PlatformInputState pollInput() = 0;
    virtual bool requestBiometricPermissions() = 0;
};

} // namespace the_stations

#endif // THE_STATIONS_PLATFORM_INTERFACE_HPP
