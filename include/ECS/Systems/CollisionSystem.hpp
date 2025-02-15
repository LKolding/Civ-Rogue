#ifndef _COLLISION_SYSTEM_
#define _COLLISION_SYSTEM_

#include <iostream>

#include "ECS/Entities/Entity.hpp"
#include "ECS/Components/Components.hpp"


bool inline checkCollision(const sf::FloatRect& a, const sf::FloatRect& b) {
    //  AABB (axis-aligned bounding box) collision detection
    return (a.left < b.left  + b.width  &&
            a.left + a.width > b.left   &&
            a.top  < b.top   + b.height &&
            a.top  + a.height> b.top );
}

void inline resolveStaticCollision(std::shared_ptr<Entity> entityA, std::shared_ptr<Entity> entityB) {
    // First, determine which entity has the StaticCollisionComponent and which has the CollisionComponent
    std::shared_ptr<StaticCollisionComponent> staticCollision;
    std::shared_ptr<CollisionComponent> dynamicCollision;
    std::shared_ptr<PositionComponent> dynamicPosition;

    if (entityA->hasComponent<StaticCollisionComponent>()) {
        staticCollision = entityA->getComponent<StaticCollisionComponent>();
        dynamicCollision = entityB->getComponent<CollisionComponent>();
        dynamicPosition = entityB->getComponent<PositionComponent>();
    } else if (entityB->hasComponent<StaticCollisionComponent>()) {
        staticCollision = entityB->getComponent<StaticCollisionComponent>();
        dynamicCollision = entityA->getComponent<CollisionComponent>();
        dynamicPosition = entityA->getComponent<PositionComponent>();
    } else {
        // No static collision detected, return early
        return;
    }

    // Now resolve the collision using dynamicCollision and dynamicPosition
    // For example, calculate overlap and adjust position of the dynamic entity

    // Example logic (replace this with your actual collision resolution):
    float overlapX = std::min(staticCollision->bounds.left + staticCollision->bounds.width, dynamicCollision->bounds.left + dynamicCollision->bounds.width) -
                     std::max(staticCollision->bounds.left, dynamicCollision->bounds.left);

    float overlapY = std::min(staticCollision->bounds.top + staticCollision->bounds.height, dynamicCollision->bounds.top + dynamicCollision->bounds.height) -
                     std::max(staticCollision->bounds.top, dynamicCollision->bounds.top);

    if (overlapX < overlapY) {
        // Separate along the x-axis
        if (dynamicCollision->bounds.left < staticCollision->bounds.left) {
            dynamicPosition->x -= overlapX;
        } else {
            dynamicPosition->x += overlapX;
        }
    } else {
        // Separate along the y-axis
        if (dynamicCollision->bounds.top < staticCollision->bounds.top) {
            dynamicPosition->y -= overlapY;
        } else {
            dynamicPosition->y += overlapY;
        }
    }

    // Update collision box positions based on new entity positions
    dynamicCollision->bounds.left = dynamicPosition->x - (dynamicCollision->bounds.width /2);
    dynamicCollision->bounds.top  = dynamicPosition->y - (dynamicCollision->bounds.height/2);
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
    collisionA->bounds.left = posA->x - (collisionA->bounds.width /2);
    collisionA->bounds.top  = posA->y - (collisionA->bounds.height/2);
    collisionB->bounds.left = posB->x - (collisionB->bounds.width /2);
    collisionB->bounds.top  = posB->y - (collisionB->bounds.height/2);
}

const float COLLISION_SYSTEM_DELAY = 0.001; 

class CollisionSystem {
public:
    inline void update(float deltaTime, std::vector<std::weak_ptr<Entity>> entities) {
        // timer logic
        this->collisionDetectionTimer += deltaTime;
        if (collisionDetectionTimer < COLLISION_SYSTEM_DELAY) 
            return;
        else 
            this->collisionDetectionTimer = 0.0f;
        
        // system logic
        for (auto& entity_p : entities) {
            if (auto entity = entity_p.lock()) {
                if (auto colPtr = entity->getComponent<CollisionComponent>()) {
                    // update collision box to position component
                    colPtr->bounds.left = entity->getComponent<PositionComponent>()->x;
                    colPtr->bounds.top  = entity->getComponent<PositionComponent>()->y;
                    
                    for (auto& entity_check_p : entities) {
                        if (auto entity_check = entity_check_p.lock()) {
                            // Static collision box check and resolve
                            if (entity_check->hasComponent<StaticCollisionComponent>()) {
                                if (checkCollision(colPtr->bounds, entity_check->getComponent<StaticCollisionComponent>()->bounds))
                                    resolveStaticCollision(entity, entity_check);
                            } 
                            else // normal collision detection and resolve
                            if (entity_check->hasComponent<CollisionComponent>()) {
                                if (checkCollision(colPtr->bounds, entity_check->getComponent<CollisionComponent>()->bounds))
                                    resolveCollision(entity, entity_check);
                            }
                        }
                    }
                }
            }
        }
    }
private:
    float collisionDetectionTimer = 0.0f;
};
#endif