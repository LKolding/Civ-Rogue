#ifndef _COLLISION_SYSTEM_
#define _COLLISION_SYSTEM_

#include <iostream>

#include "ECS/Entities/Entity.hpp"
#include "ECS/Components/Components.hpp"

bool checkCollision(const CollisionComponent& a, const CollisionComponent& b) {
    // Example: Assuming AABB (axis-aligned bounding box) collision detection
    return (a.bounds.left < b.bounds.left + b.bounds.width &&
            a.bounds.left + a.bounds.width > b.bounds.left &&
            a.bounds.top < b.bounds.top + b.bounds.height &&
            a.bounds.top + a.bounds.height > b.bounds.top);
}

void resolveCollision(std::shared_ptr<Entity> entityA, std::shared_ptr<Entity> entityB) {
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


class CollisionSystem {
public:
    void update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities) {
        for (auto& entity : entities) {
            // Check if the entity has both components
            if (entity->hasComponent<CollisionComponent>()) {
                if (entity->hasComponent<SpriteComponent>()) {
                    // update collision component
                    entity->getComponent<CollisionComponent>()->bounds = entity->getComponent<SpriteComponent>()->sprite.getGlobalBounds();
                }
                // Use getComponent and dereference the shared_ptr to access the underlying component
                auto colPtr = entity->getComponent<CollisionComponent>();

                // Make sure the pointers are valid
                if (colPtr) {
                    for (auto& entity_check : entities) {
                        if (entity_check->getComponent<UUIDComponent>()->ID == entity->getComponent<UUIDComponent>()->ID) {
                            continue; // skip own collision component
                        }
                        if (checkCollision(*colPtr, *entity_check->getComponent<CollisionComponent>())) {
                            std::cout << "Collision detected!\n"; 
                            resolveCollision(entity, entity_check);
                        }
                    }
                }
            }
        }
    }
};
#endif