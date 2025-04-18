#ifndef _CHUNKGEN_SYSTEM_
#define _CHUNKGEN_SYSTEM_

// Responsible for generating chunks around the player

#include "World.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"
#include "ECS/Components/Components.hpp"

#include "coordinate_calculators.hpp"

inline int floorDiv(int a, int b) {
    return (a >= 0) ? (a / b) : ((a - b + 1) / b);
}

class ChunkgenSystem {
public:
    inline void update(EntityManager& em, ComponentManager& cm, WorldManager& wm) {
        for (auto& ent : em.getAllEntities()) {
            auto pcontrol = cm.getComponent<ControlComponent>(ent);
            if (!pcontrol)
                continue;

            PositionComponent* entityPosition = cm.getComponent<PositionComponent>(ent);

            auto pos = getChunkCoords({entityPosition->x, entityPosition->y});

            if (wm.world.getChunk(pos))
                continue;//<-- skip if chunk at location already exists

            wm.generateRandomChunk({
                (int32_t)floorDiv(entityPosition->x, (TILE_WIDTH * CHUNK_WIDTH)),
                (int32_t)floorDiv(entityPosition->y, (TILE_HEIGHT * CHUNK_HEIGHT))
                });
            
        }
    }
};

#endif