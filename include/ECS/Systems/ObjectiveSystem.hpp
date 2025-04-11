#ifndef _OBJECTIVE_SYSTEM_
#define _OBJECTIVE_SYSTEM_

#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"
#include "ECS/Components/Components.hpp"

    // SYSTEM RESPONSIBLE FOR:
    // Making the movement component of entity correspond to 
    // the angle/direction in which the objective is located
    // relative to the entity (if entity has objective ofc!)

class ObjectiveSystem {
public:
    inline void update(EntityManager& em, ComponentManager& cm) {
        auto& storage = cm.getStorage<ObjectiveComponent>();
        for (size_t i = 0; i < storage.components.size(); ++i) {
            auto& cobjective = storage.components[i];

            // If entity has objective
            if (cobjective.hasObjective) {
                // retrieve entityID
                EntityID entityWithObjective = storage.indexToEntity.at(i);
                // retrieve position component
                auto cposition = cm.getComponent<PositionComponent>(entityWithObjective);
                // retrieve movement component
                auto cmovement = cm.getComponent<MovementComponent>(entityWithObjective);

                // if not arrived
                if ((int)cposition->x != (int)cobjective.x || (int)cposition->y != (int)cobjective.y) {
                    // update movement component
                    if ((int)cposition->x != (int)cobjective.x)
                        cmovement->xDir = cposition->x < cobjective.x ? 1.0f : -1.0f;
                    else
                        cmovement->xDir = 0.0f;
                        
                    if ((int)cposition->y != (int)cobjective.y)
                        cmovement->yDir = cposition->y < cobjective.y ? 1.0f : -1.0f;
                    else
                        cmovement->yDir = 0.0f;

                // if arrived
                } else {
                    // reset
                    cobjective.hasObjective = false;
                    cmovement->xDir = 0.0f;
                    cmovement->yDir = 0.0f;
                }
            }

        }
    }
};

#endif