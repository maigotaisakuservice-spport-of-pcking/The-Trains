#include "level_generator.hpp"
#include <random>
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace the_stations {

std::optional<Doorway> StationNode::getDoorway(uint32_t doorId) const {
    for (const auto& door : activeDoorways) {
        if (door.doorId == doorId) {
            return door;
        }
    }
    return std::nullopt;
}

bool StationNode::connectDoorway(uint32_t doorId, uint64_t targetNodeId, uint32_t targetDoorId) {
    for (auto& door : activeDoorways) {
        if (door.doorId == doorId) {
            door.isConnected = true;
            door.targetNodeId = targetNodeId;
            door.targetDoorId = targetDoorId;
            return true;
        }
    }
    return false;
}

LevelGenerator::LevelGenerator() {
    initializeTemplateCatalog();
}

void LevelGenerator::initializeTemplateCatalog() {
    m_templateCatalog.clear();
    m_templateCatalog.reserve(50);

    // 1 to 15: Rural Unmanned Stations
    for (int i = 1; i <= 15; ++i) {
        StationTemplate tmpl;
        tmpl.templateId = i;
        tmpl.category = StationCategory::RuralUnmanned;
        tmpl.displayName = "地方無人駅 #" + std::to_string(i) + " (木造ホーム)";
        tmpl.assetName = "SM_Station_Rural_" + std::to_string(i);
        tmpl.dimensions = {15.0f, 4.0f, 40.0f};
        tmpl.horrorFlickerRate = 0.1f * i;
        tmpl.ambientFogDensity = 0.02f * i;

        // Door 1 (Entrance), Door 2 (Exit)
        Doorway doorIn{1, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, false, 0, 0};
        Doorway doorOut{2, {0.0f, 0.0f, 40.0f}, {0.0f, 0.0f, 1.0f}, false, 0, 0};
        tmpl.defaultDoorways = {doorIn, doorOut};

        m_templateCatalog.push_back(tmpl);
    }

    // 16 to 30: Metro Subway Stations
    for (int i = 16; i <= 30; ++i) {
        StationTemplate tmpl;
        tmpl.templateId = i;
        tmpl.category = StationCategory::MetroSubway;
        tmpl.displayName = "大都市地下鉄 #" + std::to_string(i - 15) + " (第" + std::to_string(i - 15) + "ホーム)";
        tmpl.assetName = "SM_Station_Metro_" + std::to_string(i);
        tmpl.dimensions = {20.0f, 5.0f, 60.0f};
        tmpl.horrorFlickerRate = 0.15f * (i - 15);
        tmpl.ambientFogDensity = 0.03f * (i - 15);

        Doorway doorIn{1, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, false, 0, 0};
        Doorway doorOut{2, {0.0f, 0.0f, 60.0f}, {0.0f, 0.0f, 1.0f}, false, 0, 0};
        tmpl.defaultDoorways = {doorIn, doorOut};

        m_templateCatalog.push_back(tmpl);
    }

    // 31 to 42: Central Terminal Stations
    for (int i = 31; i <= 42; ++i) {
        StationTemplate tmpl;
        tmpl.templateId = i;
        tmpl.category = StationCategory::CentralTerminal;
        tmpl.displayName = "ターミナル駅 #" + std::to_string(i - 30) + " (中央コンコース)";
        tmpl.assetName = "SM_Station_Terminal_" + std::to_string(i);
        tmpl.dimensions = {35.0f, 8.0f, 80.0f};
        tmpl.horrorFlickerRate = 0.2f;
        tmpl.ambientFogDensity = 0.04f;

        Doorway doorIn{1, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, false, 0, 0};
        Doorway doorOut{2, {0.0f, 0.0f, 80.0f}, {0.0f, 0.0f, 1.0f}, false, 0, 0};
        tmpl.defaultDoorways = {doorIn, doorOut};

        m_templateCatalog.push_back(tmpl);
    }

    // 43 to 49: Old Foreign Stations
    for (int i = 43; i <= 49; ++i) {
        StationTemplate tmpl;
        tmpl.templateId = i;
        tmpl.category = StationCategory::OldForeignStation;
        tmpl.displayName = "海外の古びた駅 #" + std::to_string(i - 42) + " (廃墟通路)";
        tmpl.assetName = "SM_Station_Foreign_" + std::to_string(i);
        tmpl.dimensions = {18.0f, 4.5f, 50.0f};
        tmpl.horrorFlickerRate = 0.4f;
        tmpl.ambientFogDensity = 0.06f;

        Doorway doorIn{1, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, false, 0, 0};
        Doorway doorOut{2, {0.0f, 0.0f, 50.0f}, {0.0f, 0.0f, 1.0f}, false, 0, 0};
        tmpl.defaultDoorways = {doorIn, doorOut};

        m_templateCatalog.push_back(tmpl);
    }

    // 50: Under Construction
    {
        StationTemplate tmpl;
        tmpl.templateId = 50;
        tmpl.category = StationCategory::UnderConstruction;
        tmpl.displayName = "工事中の駅 (工事用坑道)";
        tmpl.assetName = "SM_Station_Construction_50";
        tmpl.dimensions = {12.0f, 3.5f, 45.0f};
        tmpl.horrorFlickerRate = 0.8f;
        tmpl.ambientFogDensity = 0.1f;

        Doorway doorIn{1, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, false, 0, 0};
        Doorway doorOut{2, {0.0f, 0.0f, 45.0f}, {0.0f, 0.0f, 1.0f}, false, 0, 0};
        tmpl.defaultDoorways = {doorIn, doorOut};

        m_templateCatalog.push_back(tmpl);
    }
}

const StationTemplate& LevelGenerator::getTemplate(int templateId) const {
    if (templateId >= 1 && templateId <= 50) {
        return m_templateCatalog[templateId - 1];
    }
    static StationTemplate fallbackSpecial{
        999, "SM_Station_Special_Escape", "ある駅 (特殊脱出レベル)",
        StationCategory::SpecialEscape, {25.0f, 6.0f, 70.0f},
        {Doorway{1, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, false, 0, 0},
         Doorway{2, {0.0f, 0.0f, 70.0f}, {0.0f, 0.0f, 1.0f}, false, 0, 0}},
        0.0f, 0.01f
    };
    return fallbackSpecial;
}

int LevelGenerator::selectTemplateIdProcedural(uint64_t seed, float fearScore, bool forceSpecialEscape) {
    if (forceSpecialEscape) return 999;

    std::mt19937_64 rng(seed);

    // High fear score increases chance of selecting creepier templates (Old Foreign / Construction)
    if (fearScore > 0.7f) {
        std::uniform_int_distribution<int> dist(40, 50);
        return dist(rng);
    } else if (fearScore > 0.4f) {
        std::uniform_int_distribution<int> dist(20, 45);
        return dist(rng);
    } else {
        std::uniform_int_distribution<int> dist(1, 35);
        return dist(rng);
    }
}

Transform LevelGenerator::calculateAlignedTransform(
    const Transform& sourceNodeTransform,
    const Doorway& sourceDoor,
    const Doorway& targetDoor
) {
    // 3D vector alignment calculation to connect targetDoor to sourceDoor seamlessly
    Transform aligned;
    Vector3 sourceWorldDoorPos = sourceNodeTransform.position + sourceDoor.localPosition;

    // Align target node origin such that target door touches source door
    aligned.position = sourceWorldDoorPos - targetDoor.localPosition;
    aligned.rotation = sourceNodeTransform.rotation;
    aligned.scale = {1.0f, 1.0f, 1.0f};
    return aligned;
}

std::shared_ptr<StationNode> LevelGenerator::generateRootStation(uint64_t playerId, float fearScore) {
    uint64_t nodeId = m_nextNodeSequence++;
    int templateId = selectTemplateIdProcedural(nodeId ^ playerId, fearScore, false);

    auto node = std::make_shared<StationNode>();
    node->nodeId = nodeId;
    node->ownerPlayerId = playerId;
    node->stationTemplate = getTemplate(templateId);
    node->worldTransform = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}};
    node->activeDoorways = node->stationTemplate.defaultDoorways;
    node->isDestroyed = false;

    m_activeSpatialGraph[nodeId] = node;
    m_playerCurrentNodes[playerId] = nodeId;
    return node;
}

std::shared_ptr<StationNode> LevelGenerator::transitionThroughDoorway(
    uint64_t playerId,
    uint64_t currentNodeId,
    uint32_t exitDoorId,
    float playerFearScore,
    bool forceSpecialEscape
) {
    // 1. Locate current node and exit door
    auto currentIt = m_activeSpatialGraph.find(currentNodeId);
    if (currentIt == m_activeSpatialGraph.end()) {
        return generateRootStation(playerId, playerFearScore);
    }

    auto currentNode = currentIt->second;
    auto exitDoorOpt = currentNode->getDoorway(exitDoorId);
    if (!exitDoorOpt.has_value()) {
        return generateRootStation(playerId, playerFearScore);
    }

    // 2. Mark previous node as destroyed & deallocate from active graph (Space Irreversibility)
    currentNode->isDestroyed = true;
    m_destroyedNodes[currentNodeId] = true;
    m_activeSpatialGraph.erase(currentNodeId); // Memory deallocation of previous space

    // 3. Generate new player-isolated station node
    uint64_t newNodeId = m_nextNodeSequence++;
    int templateId = selectTemplateIdProcedural(newNodeId ^ playerId, playerFearScore, forceSpecialEscape);

    auto newNode = std::make_shared<StationNode>();
    nodeId_t:
    newNode->nodeId = newNodeId;
    newNode->ownerPlayerId = playerId;
    newNode->stationTemplate = getTemplate(templateId);
    newNode->activeDoorways = newNode->stationTemplate.defaultDoorways;
    newNode->isSpecialEscapeStation = forceSpecialEscape;
    newNode->isDestroyed = false;

    // 4. Calculate continuous 3D spatial alignment based on doorway positions
    auto entranceDoorOpt = newNode->getDoorway(1); // Door 1 is entrance
    Doorway entranceDoor = entranceDoorOpt.value_or(Doorway{1, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, false, 0, 0});

    newNode->worldTransform = calculateAlignedTransform(
        currentNode->worldTransform,
        exitDoorOpt.value(),
        entranceDoor
    );

    // 5. Connect doorways logically
    newNode->connectDoorway(1, currentNodeId, exitDoorId);

    // 6. Update graph & active player node tracking
    m_activeSpatialGraph[newNodeId] = newNode;
    m_playerCurrentNodes[playerId] = newNodeId;

    return newNode;
}

std::shared_ptr<StationNode> LevelGenerator::getPlayerCurrentStation(uint64_t playerId) const {
    auto it = m_playerCurrentNodes.find(playerId);
    if (it != m_playerCurrentNodes.end()) {
        auto graphIt = m_activeSpatialGraph.find(it->second);
        if (graphIt != m_activeSpatialGraph.end()) {
            return graphIt->second;
        }
    }
    return nullptr;
}

bool LevelGenerator::isNodeDestroyed(uint64_t nodeId) const {
    auto it = m_destroyedNodes.find(nodeId);
    return it != m_destroyedNodes.end() && it->second;
}

} // namespace the_stations
