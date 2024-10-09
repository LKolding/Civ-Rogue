#ifndef _COLLISION_SYSTEM_
#define _COLLISION_SYSTEM_

#include <iostream>

#include "ECS/Systems/System.hpp"
#include "ECS/Entities/Entity.hpp"
#include "ECS/Components/Components.hpp"

bool inline checkCollision(const CollisionComponent& a, const CollisionComponent& b) {
    //  AABB (axis-aligned bounding box) collision detection
    return (a.bounds.left < b.bounds.left + b.bounds.width &&
            a.bounds.left + a.bounds.width > b.bounds.left &&
            a.bounds.top < b.bounds.top + b.bounds.height &&
            a.bounds.top + a.bounds.height > b.bounds.top);
}

void inline resolveCollision(std::shared_ptr<Entity> entityA, std::shared_ptr<Entity> entityB) {
    // Get shared_ptr to components
    auto collisionA = entityA->getComponent<CollisionComponent>();
    auto collisionB = entityB->getComponent<CollisionComponent>();
    auto posA = entityA->getComponent<PositionComponent>();
    auto posB = entityB->getComponent<PositionComponent>();

    // Calculate the overlap on both the x and y axes
    float overlapX = std::min(collisionA->bounds.left + collisionA->bounds.width, collisionB->bounds.left + collisionB->bounds.width) -
                     std::max(collisionA->bounds.left, collisionB->bounds.left);

    float overlapY = std::min(collisionA->bounds.top + collisionA->bounds.height, collisionB->bounds.top + collisionB->bounds.height) -
                     std::max(collisionA->bounds.top, collisionB->bounds.top);

    // Move the entities apart along the axis with the smallest overlap
    if (overlapX < overlapY) {
        // Separate along the x-axis
        if (collisionA->bounds.left < collisionB->bounds.left) {
            // Move entityA to the left, entityB to the right
            posA->x -= overlapX / 2;
            posB->x += overlapX / 2;
        } else {
            // Move entityA to the right, entityB to the left
            posA->x += overlapX / 2;
            posB->x -= overlapX / 2;
        }
    } else {
        // Separate along the y-axis
        if (collisionA->bounds.top < collisionB->bounds.top) {
            // Move entityA up, entityB down
            posA->y -= overlapY / 2;
            posB->y += overlapY / 2;
        } else {
            // Move entityA down, entityB up
            posA->y += overlapY / 2;
            posB->y -= overlapY / 2;
        }
    }

    // Update collision box positions based on new entity positions
    collisionA->bounds.left = posA->x;
    collisionA->bounds.top = posA->y;
    collisionB->bounds.left = posB->x;
    collisionB->bounds.top = posB->y;
}


class CollisionSystem: public System {
public:
    inline void update(float deltaTime, std::vector<std::weak_ptr<Entity>> entities) {
        for (auto& entity_p : entities) {
            if (auto entity = entity_p.lock()) {
                // Check if the entity has both components
                if (entity->hasComponent<CollisionComponent>()) {
                    // update position
                    auto posPtr = entity->getComponent<PositionComponent>();
                    // half of texture (COLLISION BOX) width
                    auto texWidth = entity->getComponent<CollisionComponent>()->bounds.width/2;
                    // half of texture (COLLISION BOX) height
                    auto texHeight = entity->getComponent<CollisionComponent>()->bounds.height/2;
                    entity->getComponent<CollisionComponent>()->bounds.left = posPtr->x - texWidth;
                    entity->getComponent<CollisionComponent>()->bounds.top  = posPtr->y - texHeight;
                    
                    // Use getComponent and dereference the shared_ptr to access the underlying component
                    auto colPtr = entity->getComponent<CollisionComponent>();

                    // Make sure the pointer is valid
                    if (colPtr) {
                        for (auto& entity_check_p : entities) {
                            if (auto entity_check = entity_check_p.lock()) {
                                if (entity_check->getComponent<UUIDComponent>()->ID == entity->getComponent<UUIDComponent>()->ID) {
                                continue; // skip own collision component
                                }
                                if (!entity_check->hasComponent<CollisionComponent>()) {
                                    continue; // skip entity if no CollisionComponent
                                }
                                if (checkCollision(*colPtr, *entity_check->getComponent<CollisionComponent>())) {
                                    resolveCollision(entity, entity_check);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
};
#endif