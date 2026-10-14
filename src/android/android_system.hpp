#ifndef THE_STATIONS_ANDROID_SYSTEM_HPP
#define THE_STATIONS_ANDROID_SYSTEM_HPP

#include "shared/platform_interface.hpp"

namespace the_stations {

class AndroidPlatformSystem : public IPlatformSystem {
public:
    AndroidPlatformSystem() = default;

    PlatformType getPlatformType() const override { return PlatformType::Android; }
    std::string getAccountUserName() override;
    void setAccountUserName(const std::string& name);
    PlatformInputState pollInput() override;
    bool requestBiometricPermissions() override;

private:
    std::string m_displayName{"AndroidUser_Guest"};
};

} // namespace the_stations

#endif // THE_STATIONS_ANDROID_SYSTEM_HPP
