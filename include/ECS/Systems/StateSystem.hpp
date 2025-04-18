#ifndef _STATE_SYSTEM_
#define _STATE_SYSTEM_

    // SYSTEM RESPONSIBLE FOR:
    // managing the StateComponent of all entities
    // and make sure to switch between
    // states when applicable. 
    // This is important for the animatation system.

// Let this system evaluate the current situation of the entity 
// (based on its components like MovementComponent, HealthComponent, ObjectiveComponent, etc.)
// and set the appropriate state in StateComponent.

#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"
#include "ECS/Components/Components.hpp"


class StateSystem {
public:
    inline void update(EntityManager& em, ComponentManager& cm) {
        for (auto ent : em.getAllEntities()) {
            if (!cm.getComponent<StateComponent>(ent))
                continue;

            auto state = cm.getComponent<StateComponent>(ent);

            if (auto health = cm.getComponent<HealthComponent>(ent); health && health->currentHealth <= 0) {
                state->currentState = "DEAD";

            } 
            else if (auto move = cm.getComponent<MovementComponent>(ent); move && (move->xDir != 0.0f || move->yDir != 0.0)){
                state->currentState = "WALK";
            }
            else {
                state->currentState = "IDLE";
            }
        }
    }
};

#endif