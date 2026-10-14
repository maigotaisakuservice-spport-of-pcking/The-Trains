#ifndef THE_STATIONS_LEVEL_GENERATOR_HPP
#define THE_STATIONS_LEVEL_GENERATOR_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cstdint>
#include <array>
#include <optional>

namespace the_stations {

struct Vector3 {
    float x{0.0f};
    float y{0.0f};
    float z{0.0f};

    Vector3 operator+(const Vector3& other) const { return {x + other.x, y + other.y, z + other.z}; }
    Vector3 operator-(const Vector3& other) const { return {x - other.x, y - other.y, z - other.z}; }
    bool operator==(const Vector3& other) const { return x == other.x && y == other.y && z == other.z; }
};

struct Quaternion {
    float x{0.0f};
    float y{0.0f};
    float z{0.0f};
    float w{1.0f};
};

struct Transform {
    Vector3 position{};
    Quaternion rotation{};
    Vector3 scale{1.0f, 1.0f, 1.0f};
};

struct Doorway {
    uint32_t doorId;
    Vector3 localPosition;
    Vector3 localForwardDirection;
    bool isConnected{false};
    uint64_t targetNodeId{0};
    uint32_t targetDoorId{0};
};

enum class StationCategory {
    RuralUnmanned,       // 1-15
    MetroSubway,         // 16-30
    CentralTerminal,     // 31-42
    OldForeignStation,   // 43-49
    UnderConstruction,   // 50
    SpecialEscape       // "ある駅"
};

struct StationTemplate {
    int templateId; // 1 to 50
    std::string assetName;
    std::string displayName;
    StationCategory category;
    Vector3 dimensions; // Width, Height, Length
    std::vector<Doorway> defaultDoorways;
    float horrorFlickerRate;
    float ambientFogDensity;
};

struct StationNode {
    uint64_t nodeId;
    uint64_t ownerPlayerId;
    StationTemplate stationTemplate;
    Transform worldTransform;
    std::vector<Doorway> activeDoorways;
    bool isDestroyed{false};
    bool isSpecialEscapeStation{false};

    std::optional<Doorway> getDoorway(uint32_t doorId) const;
    bool connectDoorway(uint32_t doorId, uint64_t targetNodeId, uint32_t targetDoorId);
};

class LevelGenerator {
public:
    LevelGenerator();

    // Initialize template catalog with 50 station types
    void initializeTemplateCatalog();

    // Generate root station for player session
    std::shared_ptr<StationNode> generateRootStation(uint64_t playerId, float fearScore);

    // Seamless spatial transition algorithm (Space Irreversibility & Distortion)
    // Destroys previous node, calculates continuous 3D doorway alignment, and spawns player-isolated station node
    std::shared_ptr<StationNode> transitionThroughDoorway(
        uint64_t playerId,
        uint64_t currentNodeId,
        uint32_t exitDoorId,
        float playerFearScore,
        bool forceSpecialEscape = false
    );

    // Get active station node for player
    std::shared_ptr<StationNode> getPlayerCurrentStation(uint64_t playerId) const;

    // Check if node has been destroyed (irreversible space)
    bool isNodeDestroyed(uint64_t nodeId) const;

    // Retrieve full template catalog size
    size_t getCatalogSize() const { return m_templateCatalog.size(); }
    const StationTemplate& getTemplate(int templateId) const;

private:
    uint64_t m_nextNodeSequence{1000};
    std::vector<StationTemplate> m_templateCatalog;

    // Map nodeId -> StationNode
    std::unordered_map<uint64_t, std::shared_ptr<StationNode>> m_activeSpatialGraph;

    // Map playerId -> current nodeId
    std::unordered_map<uint64_t, uint64_t> m_playerCurrentNodes;

    // Set of destroyed node IDs
    std::unordered_map<uint64_t, bool> m_destroyedNodes;

    // Helper to calculate seamless doorway alignment matrix
    Transform calculateAlignedTransform(
        const Transform& sourceNodeTransform,
        const Doorway& sourceDoor,
        const Doorway& targetDoor
    );

    // Select template ID procedurally based on PRNG seed and fear score DDA
    int selectTemplateIdProcedural(uint64_t seed, float fearScore, bool forceSpecialEscape);
};

} // namespace the_stations

#endif // THE_STATIONS_LEVEL_GENERATOR_HPP
