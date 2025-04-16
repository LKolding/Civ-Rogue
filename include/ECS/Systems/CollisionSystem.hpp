#ifndef _COLLISION_SYSTEM_
#define _COLLISION_SYSTEM_

#include <iostream>

#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"
#include "ECS/Components/Components.hpp"

    // SYSTEM RESPONSIBLE FOR:
    // checking for _and_ handling collisions
    // It handles both static and non-static bodies, but checks only
    // for when moving bodies are colliding with other moving or static bodies
    // and not if static bodies are ever colliding with any other

const float COLLISION_SYSTEM_DELAY = 0.005;// <-- increase/decrease to have fun

// Calculation functions
bool inline checkCollision(const sf::FloatRect& a, const sf::FloatRect& b) {
    //  AABB (axis-aligned bounding box) collision detection
    return (a.position.x < b.position.x  + b.size.x  &&
            a.position.x + a.size.x > b.position.x   &&
            a.position.y  < b.position.y   + b.size.y &&
            a.position.y  + a.size.y > b.position.y );
}
/*
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
*/
void inline resolveCollision(PositionComponent& positionA, PositionComponent& positionB, BoundsComponent& collisionA, BoundsComponent& collisionB) {
    // Calculate the overlap on both the x and y axes
    float overlapX = std::min(collisionA.bounds.position.x + collisionA.bounds.size.x, collisionB.bounds.position.x + collisionB.bounds.size.x) -
                     std::max(collisionA.bounds.position.x, collisionB.bounds.position.x);

    float overlapY = std::min(collisionA.bounds.position.y + collisionA.bounds.size.y, collisionB.bounds.position.y + collisionB.bounds.size.y) -
                     std::max(collisionA.bounds.position.y, collisionB.bounds.position.y);

    // Move the entities apart along the axis with the smallest overlap
    if (overlapX < overlapY) {
        // Separate along the x-axis
        if (collisionA.bounds.position.x < collisionB.bounds.position.x) {
            // Move entityA to the left, entityB to the right
            positionA.x -= overlapX / 2;
            positionB.x += overlapX / 2;
        } else {
            // Move entityA to the right, entityB to the left
            positionA.x += overlapX / 2;
            positionB.x -= overlapX / 2;
        }
    } else {
        // Separate along the y-axis
        if (collisionA.bounds.position.y < collisionB.bounds.position.y) {
            // Move entityA up, entityB down
            positionA.y -= overlapY / 2;
            positionB.y += overlapY / 2;
        } else {
            // Move entityA down, entityB up
            positionA.y += overlapY / 2;
            positionB.y -= overlapY / 2;
        }
    }

    // Update collision box positions based on new entity positions
    collisionA.bounds.position.x = positionA.x - (collisionA.bounds.size.x/2);
    collisionA.bounds.position.y = positionA.y - (collisionA.bounds.size.y/2);
    collisionB.bounds.position.x = positionB.x - (collisionB.bounds.size.x/2);
    collisionB.bounds.position.y = positionB.y - (collisionB.bounds.size.y/2);
}


class CollisionSystem {
public:
    inline void update(float dt, EntityManager& em, ComponentManager& cm) {
        // timer logic
        this->collisionDetectionTimer += dt;
        if (collisionDetectionTimer < COLLISION_SYSTEM_DELAY) 
            return;
        else
            this->collisionDetectionTimer = 0.0f;

        for (auto ent : em.getAllEntities()) {
            if (!cm.getComponent<CollisionComponent>(ent) | cm.getComponent<CollisionComponent>(ent)->isStaticBody)
                continue; // skip if no collision component or static body
            
            for (auto ent2 : em.getAllEntities()) {
                if (ent == ent2) // skip if check against itself
                    continue;

                if (!cm.getComponent<CollisionComponent>(ent2))
                    continue; // skip if no collision component

                // pointers to the first and the other body
                BoundsComponent *pbounds1 = cm.getComponent<BoundsComponent>(ent);
                BoundsComponent *pbounds2 = cm.getComponent<BoundsComponent>(ent2);

                // Collision checks
                if (checkCollision(pbounds1->bounds, pbounds2->bounds)) {
                    // If bounds2 == static body
                    if ( cm.getComponent<CollisionComponent>(ent2)->isStaticBody) {
                        //resolveStaticCollision( );
                        continue;
                    }
                    // Else, resolve collision in normal manner
                    resolveCollision(
                        *cm.getComponent<PositionComponent>(ent),
                        *cm.getComponent<PositionComponent>(ent2),
                        *pbounds1,
                        *pbounds2
                    );       
                };
            }
        }
    }
private:
    float collisionDetectionTimer = 0.0f;
};
#endif