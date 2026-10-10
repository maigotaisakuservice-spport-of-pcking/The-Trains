#include <iostream>
#include <cassert>
#include "shared/network/eos_network.hpp"

using namespace the_stations;

int main() {
    EOSNetworkManager eos;
    EOSConfig config{"prod_product", "prod_sandbox", "prod_deploy", "prod_client", "prod_secret"};

    // SDK Init
    assert(eos.initializeSDK(config) == EOS_Result::EOS_Success);
    assert(eos.isSDKInitialized());

    // Create Lobby
    std::string roomCode, lobbyId;
    assert(eos.createLobby(1001, 8, EOS_ELobbyPermissionLevel::EOS_LPL_PUBLICADVERTISED, roomCode, lobbyId) == EOS_Result::EOS_Success);
    assert(roomCode.length() == 5);
    assert(eos.getCurrentLobbyDetails() != nullptr);

    // Search and Join Lobby by 5-digit code
    EOSNetworkManager guestEOS;
    guestEOS.initializeSDK(config);
    std::string guestJoinedLobbyId;
    assert(guestEOS.searchAndJoinLobbyByCode(1002, roomCode, guestJoinedLobbyId) == EOS_Result::EOS_Success);

    // P2P Binary Transform Serialization & CRC Validation
    P2PTransformPacket pkt{};
    pkt.senderPlayerId = 1001;
    pkt.nodeLocationId = 500;
    pkt.posX = 12.5f;
    pkt.posY = 0.0f;
    pkt.posZ = -45.0f;
    pkt.isCrouching = 1;

    assert(eos.sendP2PTransformPacket(pkt) == EOS_Result::EOS_Success);

    auto received = eos.pollIncomingP2PPackets();
    assert(received.size() == 1);
    assert(received[0].senderPlayerId == 1001);
    assert(received[0].posX == 12.5f);

    std::cout << "[PASS] Production EOS Network & P2P Test" << std::endl;
    return 0;
}
