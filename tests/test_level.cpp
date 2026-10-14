#include <iostream>
#include <cassert>
#include "shared/level/level_generator.hpp"

using namespace the_stations;

int main() {
    LevelGenerator gen1;
    LevelGenerator gen2;

    // 1. Level Generation Determinism Test
    uint64_t seed = 987654321;
    float fearScore = 0.45f;

    auto nodeA1 = gen1.generateRootStation(seed, fearScore);
    auto nodeB1 = gen2.generateRootStation(seed, fearScore);

    // Assert complete deterministic template selection
    assert(nodeA1->stationTemplate.templateId == nodeB1->stationTemplate.templateId);
    assert(nodeA1->stationTemplate.category == nodeB1->stationTemplate.category);
    assert(nodeA1->stationTemplate.displayName == nodeB1->stationTemplate.displayName);

    // Transition door determinism
    auto nodeA2 = gen1.transitionThroughDoorway(seed, nodeA1->nodeId, 2, fearScore);
    auto nodeB2 = gen2.transitionThroughDoorway(seed, nodeB1->nodeId, 2, fearScore);

    assert(nodeA2->stationTemplate.templateId == nodeB2->stationTemplate.templateId);
    assert(gen1.isNodeDestroyed(nodeA1->nodeId));
    assert(gen2.isNodeDestroyed(nodeB1->nodeId));

    std::cout << "[PASS] Level Generator Determinism & Irreversibility Test" << std::endl;
    return 0;
}
