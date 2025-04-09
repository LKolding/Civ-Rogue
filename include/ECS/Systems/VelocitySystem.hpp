#ifndef _VELOCITY_SYSTEM_
#define _VELOCITY_SYSTEM_
#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"
#include "ECS/Components/Components.hpp"


class VelocitySystem {
public:
    inline void update(float dt, EntityManager& em, ComponentManager& cm) {
        for (auto& ent : em.getAllEntities()) {
            if ( !cm.getComponent<PositionComponent>(ent) | !cm.getComponent<MovementComponent>(ent) ) {
                continue;
            }
            // Declarations
            auto p_movecomponent = cm.getComponent<MovementComponent>(ent);
            auto p_poscomponent  = cm.getComponent<PositionComponent>(ent);
            float x = p_movecomponent->xDir;
            float y = p_movecomponent->yDir;
            float speed = p_movecomponent->speed;
            // Calculation
            float length = std::sqrt(x * x + y * y);

            if (length != 0.0f) {
                x /= length;
                y /= length;
            }
            // Apply
            p_poscomponent->x += (speed * dt) * x;
            p_poscomponent->y += (speed * dt) * y;

        }

    }

    // inline void update2(float deltaTime, EntityManager& em, ComponentManager& cm) {
    //     for (auto& entity_p : entities) {
    //         if (auto entity = entity_p.lock()) {
    //             if (!entity->hasComponent<PositionComponent>() || !entity->hasComponent<MovementComponent>())
    //                 return;

    //             auto velPtr = entity->getComponent<MovementComponent>();
    //             auto posPtr = entity->getComponent<PositionComponent>();

    //             float x = velPtr->xDir;
    //             float y = velPtr->yDir;

    //             float length = std::sqrt(x * x + y * y);

    //             if (length != 0.0f) {
    //                 x /= length;
    //                 y /= length;
    //             }
    //             //  Update position based on direction, speed and deltatime
    //             posPtr->x += (velPtr->speed * deltaTime) * x;
    //             posPtr->y += (velPtr->speed * deltaTime) * y;
                
    //             //  Mark the entity texture as flipped if traveling left (and vice versa)
    //             if (velPtr->xDir != 0.0f && entity->hasComponent<FlipComponent>()) {
    //                 if (velPtr->xDir > 0.0f) {
    //                     entity->getComponent<FlipComponent>()->isFlipped = false;
    //                 } else 
    //                 if (velPtr->xDir < 0.0f){
    //                     entity->getComponent<FlipComponent>()->isFlipped = true;
    //                 }
    //             }
                
    //             //  Syncronize sprite position
    //             if (entity->hasComponent<PositionComponent>() && entity->hasComponent<SpriteComponent>()) {
    //                 entity->getComponent<SpriteComponent>()->sprite.setPosition(
    //                     posPtr->x, 
    //                     posPtr->y
    //                     );
    //             }
    //         }
    //     }
    // }
};

#endif