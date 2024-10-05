#ifndef _MOVEMENT_SYSTEM_
#define _MOVEMENT_SYSTEM_

#include "ECS/Entities/Entity.hpp"
#include "ECS/Components/Components.hpp"

class MovementSystem {
public:
    void update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities) {
        for (auto& entity : entities) {
            // Check if the entity has both components
            if (entity->hasComponent<PositionComponent>() && entity->hasComponent<VelocityComponent>()) {
                // Use getComponent and dereference the shared_ptr to access the underlying component
                auto posPtr = entity->getComponent<PositionComponent>();
                auto velPtr = entity->getComponent<VelocityComponent>();

                // Make sure the pointers are valid
                if (posPtr && velPtr) {
                    // Update position based on velocity
                    posPtr->x += (velPtr->xSpeed * deltaTime) * velPtr->xDir;
                    posPtr->y += (velPtr->ySpeed * deltaTime) * velPtr->yDir;

                }
            }
        }
    }
};

#endif