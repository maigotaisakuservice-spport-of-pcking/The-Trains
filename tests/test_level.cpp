#include <iostream>
#include <cassert>
#include "shared/level/level_generator.hpp"

using namespace the_stations;

int main() {
    LevelGenerator gen;

    // Verify template catalog initialization (50 templates)
    assert(gen.getCatalogSize() == 50);

    // Verify root station generation
    uint64_t p1 = 1001;
    auto node1 = gen.generateRootStation(p1, 0.2f);
    assert(node1 != nullptr);
    assert(node1->ownerPlayerId == p1);
    assert(!node1->isDestroyed);

    uint64_t initialNodeId = node1->nodeId;

    // Transition through doorway (3D spatial transition & memory destruction)
    auto p1_next = gen.transitionThroughDoorway(p1, initialNodeId, 2, 0.5f);
    assert(p1_next != nullptr);
    assert(p1_next->nodeId != initialNodeId);
    assert(gen.isNodeDestroyed(initialNodeId)); // Old node memory destroyed and unlinked from active graph

    // Verify individual non-reversible station paths per player in multiplayer
    uint64_t p2 = 1002;
    auto node2 = gen.generateRootStation(p2, 0.1f);
    auto p2_next = gen.transitionThroughDoorway(p2, node2->nodeId, 2, 0.8f);

    assert(p1_next->nodeId != p2_next->nodeId);
    assert(p2_next->stationTemplate.category == StationCategory::CentralTerminal ||
           p2_next->stationTemplate.category == StationCategory::OldForeignStation ||
           p2_next->stationTemplate.category == StationCategory::UnderConstruction);

    std::cout << "[PASS] Production Procedural Level Generator & Spatial Irreversibility Test" << std::endl;
    return 0;
}
