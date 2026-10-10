#ifndef THE_STATIONS_ENTITY_AI_HPP
#define THE_STATIONS_ENTITY_AI_HPP

#include <string>
#include <vector>

namespace the_stations {

enum class EntityState {
    Wander,
    Investigate,
    Chase
};

class EntityAI {
public:
    EntityAI() = default;

    EntityState getCurrentState() const { return m_state; }

    void update(float deltaTimeSec, bool playerInLineOfSight, bool noiseDetected, float fearScore);

    // Announcement gimmick logic
    void triggerLostPlayerAnnouncement(const std::string& playerName);
    bool isAnnouncementActive() const { return m_announcementActive; }
    std::string getAnnouncementMessage() const { return m_announcementMessage; }

    // Station Timer logic (10 min hidden background timer)
    void updateStationTimer(float deltaTimeSec);
    float getStationTimerSec() const { return m_stationTimerSec; }
    std::string checkStationTimerAnnouncements();
    bool isTimeExpiredForForcedKill() const { return m_stationTimerSec >= 600.0f; }

    float getEntitySpawnDensityMultiplier() const { return m_spawnDensityMultiplier; }

private:
    EntityState m_state{EntityState::Wander};
    float m_spawnDensityMultiplier{1.0f};

    // Announcement state
    bool m_announcementActive{false};
    std::string m_announcementMessage;

    // Station timer state
    float m_stationTimerSec{0.0f};
    bool m_announced10MinWarning{false};
    bool m_announced5MinWarning{false};
    bool m_announced1MinWarning{false};
    bool m_announcedExpired{false};
};

} // namespace the_stations

#endif // THE_STATIONS_ENTITY_AI_HPP
