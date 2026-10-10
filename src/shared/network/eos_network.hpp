#ifndef THE_STATIONS_EOS_NETWORK_HPP
#define THE_STATIONS_EOS_NETWORK_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace the_stations {

// Authenticate and EOS Native C API Structures
typedef void* EOS_HPlatform;
typedef void* EOS_HLobby;
typedef void* EOS_HP2P;
typedef void* EOS_ProductUserId;

enum class EOS_Result {
    EOS_Success = 0,
    EOS_InvalidParameters = 1,
    EOS_NotInitialized = 2,
    EOS_NotFound = 3,
    EOS_LimitExceeded = 4,
    EOS_Lobby_LobbyAlreadyExists = 100,
    EOS_Lobby_NoSuchLobby = 101,
    EOS_P2P_ConnectionFailed = 200
};

enum class EOS_ELobbyPermissionLevel {
    EOS_LPL_PUBLICADVERTISED = 0,
    EOS_LPL_JOINVIAPRESENCE = 1,
    EOS_LPL_INVITEONLY = 2
};

enum class EOS_EP2PPacketReliability {
    EOS_PR_UnreliableUnordered = 0,
    EOS_PR_ReliableUnordered = 1,
    EOS_PR_ReliableOrdered = 2
};

struct EOSConfig {
    std::string productId;
    std::string sandboxId;
    std::string deploymentId;
    std::string clientId;
    std::string clientSecret;
};

#pragma pack(push, 1)
struct P2PTransformPacket {
    uint32_t packetHeader{0x53544154}; // "STAT" magic header
    uint64_t senderPlayerId;
    uint64_t nodeLocationId;
    float posX, posY, posZ;
    float rotX, rotY, rotZ, rotW;
    uint8_t isCrouching;
    uint8_t isGrabbing;
    uint32_t checksum;
};
#pragma pack(pop)

struct EOSLobbyDetails {
    std::string lobbyId;
    std::string roomCode;
    uint64_t hostPlayerId;
    uint32_t maxMembers;
    uint32_t currentMembers;
    EOS_ELobbyPermissionLevel permission;
};

class EOSNetworkManager {
public:
    EOSNetworkManager();
    ~EOSNetworkManager();

    EOS_Result initializeSDK(const EOSConfig& config);
    bool isSDKInitialized() const { return m_sdkInitialized; }

    // Authentic EOS API call wrappers
    EOS_Result createLobby(
        uint64_t hostPlayerId,
        uint32_t maxPlayers,
        EOS_ELobbyPermissionLevel permission,
        std::string& outRoomCode,
        std::string& outLobbyId
    );

    EOS_Result searchAndJoinLobbyByCode(
        uint64_t joiningPlayerId,
        const std::string& roomCode,
        std::string& outLobbyId
    );

    EOS_Result sendP2PTransformPacket(
        const P2PTransformPacket& packet,
        EOS_EP2PPacketReliability reliability = EOS_EP2PPacketReliability::EOS_PR_ReliableOrdered
    );

    std::vector<P2PTransformPacket> pollIncomingP2PPackets();

    const EOSLobbyDetails* getCurrentLobbyDetails() const { return m_currentLobby.get(); }
    size_t getP2PQueueSize() const;

private:
    EOSConfig m_config;
    bool m_sdkInitialized{false};

    // Native EOS Handle Pointers
    EOS_HPlatform m_platformHandle{nullptr};
    EOS_HLobby m_lobbyHandle{nullptr};
    EOS_HP2P m_p2pHandle{nullptr};

    std::unique_ptr<EOSLobbyDetails> m_currentLobby;

    std::mutex m_p2pMutex;
    std::vector<P2PTransformPacket> m_p2pIncomingQueue;

    std::string generate5DigitRoomCode();
    uint32_t calculatePacketChecksum(const P2PTransformPacket& packet) const;
};

} // namespace the_stations

#endif // THE_STATIONS_EOS_NETWORK_HPP
