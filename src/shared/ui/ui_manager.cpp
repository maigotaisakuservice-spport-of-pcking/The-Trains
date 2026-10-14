#include "ui_manager.hpp"
#include <sstream>
#include <random>
#include <algorithm>

namespace the_stations {

UIManager::UIManager() = default;

void UIManager::navigateTo(ScreenState state) {
    m_currentState = state;
}

void UIManager::selectGameMode(GameMode mode) {
    m_gameMode = mode;
    if (mode == GameMode::SinglePlayer) {
        navigateTo(ScreenState::GameplayCutscene);
    } else {
        navigateTo(ScreenState::LobbySelect);
    }
}

std::string UIManager::generateRandomRoomCode() {
    static const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<size_t> dist(0, sizeof(chars) - 2);

    std::string code;
    for (int i = 0; i < 5; ++i) {
        code += chars[dist(rng)];
    }
    return code;
}

void UIManager::createPrivateLobby() {
    m_roomCode = generateRandomRoomCode();
    m_lobbyPlayers.clear();
    m_lobbyPlayers.push_back({1, "HostPlayer", "Steam", true, true});
    navigateTo(ScreenState::PrivateLobby);
}

bool UIManager::joinPrivateLobby(const std::string& code) {
    if (code.length() != 5) return false;
    m_roomCode = code;
    m_lobbyPlayers.clear();
    m_lobbyPlayers.push_back({1, "HostPlayer", "Steam", true, true});
    m_lobbyPlayers.push_back({2, "Guest_02", "Android", false, true});
    navigateTo(ScreenState::PrivateLobby);
    return true;
}

void UIManager::quickMatchPublicLobby() {
    m_roomCode = generateRandomRoomCode();
    m_lobbyPlayers.clear();
    m_lobbyPlayers.push_back({1, "HostPlayer", "Steam", true, true});
    m_lobbyPlayers.push_back({2, "Guest_02", "Android", false, true});
    m_lobbyPlayers.push_back({3, "Guest_03", "Mac", false, true});
    navigateTo(ScreenState::GameplayCutscene);
}

void UIManager::startLobbyGame() {
    if (m_currentState == ScreenState::PrivateLobby) {
        navigateTo(ScreenState::GameplayCutscene);
    }
}

void UIManager::leaveLobby() {
    m_lobbyPlayers.clear();
    m_roomCode.clear();
    navigateTo(ScreenState::LobbySelect);
}

std::string UIManager::renderCurrentScreen() const {
    std::ostringstream ss;
    switch (m_currentState) {
        case ScreenState::Title:
            ss << "================================================================================\n"
               << "[Background: 3D Underground Platform Loop Scene]\n\n"
               << "                   T h e   S t a t i o n s\n"
               << "             〜そこへの入口はどこにだってある〜\n\n"
               << "                    > [ START GAME ] <\n"
               << "================================================================================";
            break;

        case ScreenState::ModeSelect:
            ss << "================================================================================\n"
               << "[Background: 3D Train Interior Scene]\n\n"
               << "                   T h e   S t a t i o n s\n\n"
               << "                 >  シングルプレイ  <\n"
               << "                 >  マルチプレイ    <\n"
               << "================================================================================";
            break;

        case ScreenState::LobbySelect:
            ss << "================================================================================\n"
               << "【 マルチプレイモード 】\n\n"
               << "  ┌──────────────────────────────────────────────────────────┐\n"
               << "  │ [ プライベート (フレンドと遊ぶ) ]                        │\n"
               << "  │                                                          │\n"
               << "  │   > 部屋を作る (ルームコードを発行)                      │\n"
               << "  │   > 部屋に入る (5桁の合言葉を入力)                       │\n"
               << "  │                                                          │\n"
               << "  └──────────────────────────────────────────────────────────┘\n\n"
               << "  ┌──────────────────────────────────────────────────────────┐\n"
               << "  │ [ パブリック (世界中のだれかと遊ぶ) ]                    │\n"
               << "  │                                                          │\n"
               << "  │   > 今すぐ参加 (ランダムマッチング)                      │\n"
               << "  │                                                          │\n"
               << "  └──────────────────────────────────────────────────────────┘\n"
               << "================================================================================";
            break;

        case ScreenState::PrivateLobby:
            ss << "================================================================================\n"
               << "【 ロビー待機中 】  Room Code: [ " << m_roomCode << " ]  (LINE等で共有)\n\n"
               << "  参加プレイヤー (最大8名):\n"
               << "  --------------------------------------------------------------------------\n";
            for (int i = 1; i <= 8; ++i) {
                auto it = std::find_if(m_lobbyPlayers.begin(), m_lobbyPlayers.end(),
                                       [i](const LobbyPlayer& p) { return p.slotIndex == i; });
                if (it != m_lobbyPlayers.end()) {
                    ss << "  [P" << i << "] " << it->name << " (" << it->platform << ")\t";
                } else {
                    ss << "  [P" << i << "] --- 待機中 ---\t";
                }
                if (i % 2 == 0) ss << "\n";
            }
            ss << "  --------------------------------------------------------------------------\n\n"
               << "                   > [ ゲーム開始 ] < (ホストのみ操作可能)\n"
               << "                   > [ 退出する ]   <\n"
               << "================================================================================";
            break;

        case ScreenState::GameplayCutscene:
            ss << "[Opening Cutscene: Non-controllable Train Interior Scene. Train stops and doors open.]";
            break;

        case ScreenState::Gameplay:
            ss << "[HUD 0% - First Person Immersive Gameplay Active]";
            break;

        case ScreenState::GameOver:
            ss << "[GAME OVER - Distortion & Fallen Perspective]";
            break;

        case ScreenState::GameClear:
            ss << "[GAME CLEAR - Escaped to Rain Street via Manhole]";
            break;
    }
    return ss.str();
}

} // namespace the_stations
