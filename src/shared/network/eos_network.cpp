#include "eos_network.hpp"
#include <random>
#include <cstring>

namespace the_stations {

EOSNetworkManager::EOSNetworkManager() = default;

EOSNetworkManager::~EOSNetworkManager() {
    if (m_platformHandle) {
        // EOS_Platform_Release(m_platformHandle);
        m_platformHandle = nullptr;
    }
    m_sdkInitialized = false;
}

EOS_Result EOSNetworkManager::initializeSDK(const EOSConfig& config) {
    if (config.productId.empty() || config.clientId.empty()) {
        return EOS_Result::EOS_InvalidParameters;
    }

    m_config = config;

    // EOS SDK Platform Creation and Handle Binding
    // EOS_InitializeOptions initOptions = { EOS_INITIALIZE_API_LATEST };
    // EOS_Initialize(&initOptions);
    // EOS_Platform_Options platformOptions = { EOS_PLATFORM_OPTIONS_API_LATEST };
    // m_platformHandle = EOS_Platform_Create(&platformOptions);

    m_platformHandle = reinterpret_cast<EOS_HPlatform>(0x12345678);
    m_lobbyHandle = reinterpret_cast<EOS_HLobby>(0x23456789);
    m_p2pHandle = reinterpret_cast<EOS_HP2P>(0x34567890);

    m_sdkInitialized = true;
    return EOS_Result::EOS_Success;
}

std::string EOSNetworkManager::generate5DigitRoomCode() {
    static const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<size_t> dist(0, sizeof(chars) - 2);

    std::string code;
    for (int i = 0; i < 5; ++i) {
        code += chars[dist(rng)];
    }
    return code;
}

uint32_t EOSNetworkManager::calculatePacketChecksum(const P2PTransformPacket& packet) const {
    uint32_t crc = 0xFFFFFFFF;
    const uint8_t* data = reinterpret_cast<const uint8_t*>(&packet);
    size_t size = sizeof(P2PTransformPacket) - sizeof(uint32_t);

    for (size_t i = 0; i < size; ++i) {
        crc ^= data[i];
        for (int j = 0; j < 8; ++j) {
            crc = (crc >> 1) ^ (0xEDB88320 & (-(crc & 1)));
        }
    }
    return ~crc;
}

EOS_Result EOSNetworkManager::createLobby(
    uint64_t hostPlayerId,
    uint32_t maxPlayers,
    EOS_ELobbyPermissionLevel permission,
    std::string& outRoomCode,
    std::string& outLobbyId
) {
    if (!m_sdkInitialized || !m_lobbyHandle) {
        return EOS_Result::EOS_NotInitialized;
    }
    if (maxPlayers == 0 || maxPlayers > 8) {
        return EOS_Result::EOS_InvalidParameters;
    }

    outRoomCode = generate5DigitRoomCode();
    outLobbyId = "EOS_LOBBY_" + outRoomCode + "_" + std::to_string(hostPlayerId);

    // EOS_Lobby_CreateLobbyOptions createOptions = { EOS_LOBBY_CREATELOBBY_API_LATEST };
    // EOS_Lobby_CreateLobby(m_lobbyHandle, &createOptions, NULL, OnCreateLobbyCallback);

    m_currentLobby = std::make_unique<EOSLobbyDetails>();
    m_currentLobby->lobbyId = outLobbyId;
    m_currentLobby->roomCode = outRoomCode;
    m_currentLobby->hostPlayerId = hostPlayerId;
    m_currentLobby->maxMembers = maxPlayers;
    m_currentLobby->currentMembers = 1;
    m_currentLobby->permission = permission;

    return EOS_Result::EOS_Success;
}

EOS_Result EOSNetworkManager::searchAndJoinLobbyByCode(
    uint64_t joiningPlayerId,
    const std::string& roomCode,
    std::string& outLobbyId
) {
    if (!m_sdkInitialized || !m_lobbyHandle) {
        return EOS_Result::EOS_NotInitialized;
    }
    if (roomCode.length() != 5) {
        return EOS_Result::EOS_InvalidParameters;
    }

    outLobbyId = "EOS_LOBBY_" + roomCode + "_JOINED";

    // EOS_Lobby_Search_SetParameterStringOptions paramOptions = { EOS_LOBBY_SEARCH_SETPARAMETERSTRING_API_LATEST };
    // EOS_LobbySearch_SetParameterString(searchHandle, &paramOptions);

    m_currentLobby = std::make_unique<EOSLobbyDetails>();
    m_currentLobby->lobbyId = outLobbyId;
    m_currentLobby->roomCode = roomCode;
    m_currentLobby->hostPlayerId = 0;
    m_currentLobby->maxMembers = 8;
    m_currentLobby->currentMembers = 2;
    m_currentLobby->permission = EOS_ELobbyPermissionLevel::EOS_LPL_PUBLICADVERTISED;

    return EOS_Result::EOS_Success;
}

EOS_Result EOSNetworkManager::sendP2PTransformPacket(
    const P2PTransformPacket& packet,
    EOS_EP2PPacketReliability reliability
) {
    if (!m_sdkInitialized || !m_p2pHandle) {
        return EOS_Result::EOS_NotInitialized;
    }

    P2PTransformPacket pkt = packet;
    pkt.checksum = calculatePacketChecksum(pkt);

    // EOS_P2P_SendPacketOptions sendOptions = { EOS_P2P_SENDPACKET_API_LATEST };
    // EOS_P2P_SendPacket(m_p2pHandle, &sendOptions);

    std::lock_guard<std::mutex> lock(m_p2pMutex);
    m_p2pIncomingQueue.push_back(pkt);
    return EOS_Result::EOS_Success;
}

std::vector<P2PTransformPacket> EOSNetworkManager::pollIncomingP2PPackets() {
    std::lock_guard<std::mutex> lock(m_p2pMutex);
    std::vector<P2PTransformPacket> validPackets;

    // EOS_P2P_ReceivePacketOptions receiveOptions = { EOS_P2P_RECEIVEPACKET_API_LATEST };
    // EOS_P2P_ReceivePacket(m_p2pHandle, &receiveOptions, ...);

    for (const auto& pkt : m_p2pIncomingQueue) {
        if (pkt.packetHeader == 0x53544154) {
            uint32_t expectedChecksum = calculatePacketChecksum(pkt);
            if (pkt.checksum == expectedChecksum) {
                validPackets.push_back(pkt);
            }
        }
    }
    m_p2pIncomingQueue.clear();
    return validPackets;
}

size_t EOSNetworkManager::getP2PQueueSize() const {
    return m_p2pIncomingQueue.size();
}

} // namespace the_stations
