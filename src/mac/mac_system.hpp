#ifndef THE_STATIONS_MAC_SYSTEM_HPP
#define THE_STATIONS_MAC_SYSTEM_HPP

#include "shared/platform_interface.hpp"

namespace the_stations {

class MacPlatformSystem : public IPlatformSystem {
public:
    MacPlatformSystem() = default;

    PlatformType getPlatformType() const override { return PlatformType::Mac; }
    std::string getAccountUserName() override;
    PlatformInputState pollInput() override;
    bool requestBiometricPermissions() override;
};

} // namespace the_stations

#endif // THE_STATIONS_MAC_SYSTEM_HPP
