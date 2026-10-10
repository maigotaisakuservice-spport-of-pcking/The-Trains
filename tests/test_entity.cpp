#include <iostream>
#include <cassert>
#include "shared/entity/entity_ai.hpp"

using namespace the_stations;

int main() {
    EntityAI ai;
    assert(ai.getCurrentState() == EntityState::Wander);

    // Test noise investigation
    ai.update(0.1f, false, true, 0.2f);
    assert(ai.getCurrentState() == EntityState::Investigate);

    // Test player chase
    ai.update(0.1f, true, false, 0.2f);
    assert(ai.getCurrentState() == EntityState::Chase);

    // Test announcement gimmick
    ai.triggerLostPlayerAnnouncement("Guest_02");
    assert(ai.isAnnouncementActive());
    assert(ai.getAnnouncementMessage().find("Guest_02") != std::string::npos);

    // Test 10-minute station timer warnings and forced kill
    std::string warning0 = ai.checkStationTimerAnnouncements();
    assert(!warning0.empty());

    ai.updateStationTimer(300.0f); // 5 mins
    std::string warning5 = ai.checkStationTimerAnnouncements();
    assert(warning5.find("あと5分") != std::string::npos);

    ai.updateStationTimer(300.0f); // 10 mins reached
    std::string warningExpired = ai.checkStationTimerAnnouncements();
    assert(ai.isTimeExpiredForForcedKill());

    std::cout << "[PASS] Entity AI & Station Timer Test" << std::endl;
    return 0;
}
