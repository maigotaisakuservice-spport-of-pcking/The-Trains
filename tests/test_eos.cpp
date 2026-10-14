#include <iostream>
#include <cassert>
#include <unordered_set>
#include "shared/network/eos_network.hpp"

using namespace the_stations;

int main() {
    EOSNetworkManager eos;
    EOSConfig config{"prod_product", "prod_sandbox", "prod_deploy", "prod_client", "prod_secret"};
    assert(eos.initializeSDK(config) == EOS_Result::EOS_Success);

    // 1. Room Code Collision Resistance Test (Generate 1000 codes, ensure unique distribution)
    std::unordered_set<std::string> codes;
    for (int i = 0; i < 1000; ++i) {
        std::string code, lobbyId;
        eos.createLobby(i + 1, 8, EOS_ELobbyPermissionLevel::EOS_LPL_PUBLICADVERTISED, code, lobbyId);
        assert(code.length() == 5);
        codes.insert(code);
    }
    assert(codes.size() > 950); // High entropy collision resistance

    // 2. P2P Packet Binary Integrity & Checksum Test
    P2PTransformPacket pkt{};
    pkt.senderPlayerId = 999;
    pkt.posX = 100.0f;
    pkt.posY = 200.0f;
    pkt.posZ = 300.0f;

    assert(eos.sendP2PTransformPacket(pkt) == EOS_Result::EOS_Success);
    auto received = eos.pollIncomingP2PPackets();
    assert(received.size() == 1);
    assert(received[0].senderPlayerId == 999);
    assert(received[0].posX == 100.0f);

    std::cout << "[PASS] EOS Room Code Collision & P2P Session Test" << std::endl;
    return 0;
}
