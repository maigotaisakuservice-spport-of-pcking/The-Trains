#ifndef THE_STATIONS_WINDOWS_SYSTEM_HPP
#define THE_STATIONS_WINDOWS_SYSTEM_HPP

#include "shared/platform_interface.hpp"

namespace the_stations {

class WindowsPlatformSystem : public IPlatformSystem {
public:
    WindowsPlatformSystem() = default;

    PlatformType getPlatformType() const override { return PlatformType::Windows; }
    std::string getAccountUserName() override;
    PlatformInputState pollInput() override;
    bool requestBiometricPermissions() override;
};

} // namespace the_stations

#endif // THE_STATIONS_WINDOWS_SYSTEM_HPP
