#include "entity_ai.hpp"

namespace the_stations {

void EntityAI::update(float deltaTimeSec, bool playerInLineOfSight, bool noiseDetected, float fearScore) {
    updateStationTimer(deltaTimeSec);

    // State machine transitions
    if (playerInLineOfSight) {
        m_state = EntityState::Chase;
    } else if (noiseDetected) {
        m_state = EntityState::Investigate;
    } else {
        m_state = EntityState::Wander;
    }

    // High fear score or active announcement increases spawn density
    m_spawnDensityMultiplier = 1.0f + fearScore * 2.0f;
    if (m_announcementActive) {
        m_spawnDensityMultiplier *= 3.0f;
    }
}

void EntityAI::triggerLostPlayerAnnouncement(const std::string& playerName) {
    m_announcementActive = true;
    m_announcementMessage = "ピンポンパンポン…… " + playerName + " さんがただいま迷子です〜。現在捜索しておりますので、その場でお待ちください……";
    m_spawnDensityMultiplier *= 3.0f;
}

void EntityAI::updateStationTimer(float deltaTimeSec) {
    m_stationTimerSec += deltaTimeSec;
}

std::string EntityAI::checkStationTimerAnnouncements() {
    if (m_stationTimerSec >= 600.0f && !m_announcedExpired) {
        m_announcedExpired = true;
        m_spawnDensityMultiplier = 100.0f; // Forced kill spawn rush
        return "本日の営業は終了しました";
    }
    if (m_stationTimerSec >= 540.0f && !m_announced1MinWarning) {
        m_announced1MinWarning = true;
        return "まもなく営業終了です。お急ぎください…";
    }
    if (m_stationTimerSec >= 300.0f && !m_announced5MinWarning) {
        m_announced5MinWarning = true;
        return "営業終了まで、あと5分です…";
    }
    if (m_stationTimerSec >= 0.0f && !m_announced10MinWarning) {
        m_announced10MinWarning = true;
        return "本日は当駅をご利用いただき… あと10分で営業終了いたします";
    }
    return "";
}

} // namespace the_stations
