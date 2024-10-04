#ifndef _COLLISION_SYSTEM_
#define _COLLISION_SYSTEM_

#include "ECS/Entities/Entity.hpp"
#include "ECS/Components/Components.hpp"

class CollisionSystem {
public:
    void update(float deltaTime, std::vector<Entity>& entities) {
        for (auto& entity : entities) {
            // Check if the entity has both components
            if (entity.hasComponent<CollisionComponent>()) {
                // Use getComponent and dereference the shared_ptr to access the underlying component
                auto colPtr = entity.getComponent<CollisionComponent>();

                // Make sure the pointers are valid
                if (colPtr) {
                    continue;
                }
            }
        }
    }
};
#endif