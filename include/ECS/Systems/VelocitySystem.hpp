#ifndef _VELOCITY_SYSTEM_
#define _VELOCITY_SYSTEM_

#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"
#include "ECS/Components/Components.hpp"


class VelocitySystem {
public:
    inline void update(float dt, EntityManager& em, ComponentManager& cm) {
        for (auto& ent : em.getAllEntities()) {
            if ( !cm.getComponent<PositionComponent>(ent) | !cm.getComponent<MovementComponent>(ent) )
                continue;//<-- skip
            // Pointers to components
            auto p_movecomponent = cm.getComponent<MovementComponent>(ent);
            auto p_poscomponent  = cm.getComponent<PositionComponent>(ent);
            // Flip behavior
            if (auto p_flipcomponent = cm.getComponent<FlipComponent>(ent)) {
                // Moving right
                if (p_movecomponent->xDir < 0.0f && !p_flipcomponent->isFlipped) {
                    p_flipcomponent->isFlipped = true;
                } // left
                else if (p_movecomponent->xDir > 0.0f && p_flipcomponent->isFlipped) {
                    p_flipcomponent->isFlipped = false;
                } 
            }
            // Declarations
            float x = p_movecomponent->xDir;
            float y = p_movecomponent->yDir;
            float speed = p_movecomponent->speed * p_movecomponent->speedMult;
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
};

#endif