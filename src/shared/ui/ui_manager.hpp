#ifndef THE_STATIONS_UI_STATE_HPP
#define THE_STATIONS_UI_STATE_HPP

#include <string>
#include <vector>
#include <memory>

namespace the_stations {

enum class ScreenState {
    Title,
    ModeSelect,
    LobbySelect,
    PrivateLobby,
    GameplayCutscene,
    Gameplay,
    GameOver,
    GameClear
};

enum class GameMode {
    SinglePlayer,
    MultiPlayer
};

struct LobbyPlayer {
    int slotIndex;
    std::string name;
    std::string platform; // e.g. "Steam", "Android", "Mac"
    bool isHost;
    bool isReady;
};

class UIManager {
public:
    UIManager();

    ScreenState getCurrentState() const { return m_currentState; }
    GameMode getGameMode() const { return m_gameMode; }
    const std::string& getRoomCode() const { return m_roomCode; }
    const std::vector<LobbyPlayer>& getLobbyPlayers() const { return m_lobbyPlayers; }

    void navigateTo(ScreenState state);
    void selectGameMode(GameMode mode);

    // UI Interactions
    void createPrivateLobby();
    bool joinPrivateLobby(const std::string& code);
    void quickMatchPublicLobby();
    void startLobbyGame();
    void leaveLobby();

    // Render mockup ASCII representations
    std::string renderCurrentScreen() const;

private:
    ScreenState m_currentState{ScreenState::Title};
    GameMode m_gameMode{GameMode::SinglePlayer};
    std::string m_roomCode;
    std::vector<LobbyPlayer> m_lobbyPlayers;

    std::string generateRandomRoomCode();
};

} // namespace the_stations

#endif // THE_STATIONS_UI_STATE_HPP
