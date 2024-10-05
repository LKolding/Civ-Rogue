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
                // Update position based on direction, speed and deltatime
                if (posPtr && velPtr) {
                    float x = velPtr->xDir;
                    float y = velPtr->yDir;

                    float length = std::sqrt(x * x + y * y);

                    if (length != 0.0f) {
                        x /= length;
                        y /= length;
                    }

                    posPtr->x += (velPtr->xSpeed * deltaTime) * x;
                    posPtr->y += (velPtr->ySpeed * deltaTime) * y;

                }
                // Mark the texture as flipped if traveling left or right
                if (velPtr->xDir != 0.0f) {
                    if (velPtr->xDir == 1.0f) {
                        entity->getComponent<FlipComponent>()->isFlipped = false;
                    } else 
                    if (velPtr->xDir == -1.0f){
                        entity->getComponent<FlipComponent>()->isFlipped = true;
                    }
                }
            }
        }
    }
};

#endif