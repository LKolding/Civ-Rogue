#ifndef _LIFETIME_SYSTEM_
#define _LIFETIME_SYSTEM_

#include <vector>

#include "ECS/Entities/Entity.hpp"
#include "ECS/Systems/System.hpp"

// This system  updates the data members of LifetimeComponent and DeletableComponent.
//
// Whenever lifetime is spent this system marks its deletableComponent 
// as true and EntityManager takes care of proper deletion of entity

class LifetimeSystem: public System {
public:
    inline void update(float deltaTime, std::vector<std::weak_ptr<Entity>> entities) {
        for (auto& entity_p : entities) {
            if (auto entity = entity_p.lock()) {
                if (!entity->hasComponent<LifetimeComponent>())
                    continue;
                if (!entity->hasComponent<DeletableComponent>())
                    continue;
                // update
                if (auto lft_ptr = entity->getComponent<LifetimeComponent>()) {
                    if (lft_ptr->timeAlive >= lft_ptr->deathTime) {
                        entity->getComponent<DeletableComponent>()->markedForDeletion = true;
                        
                    } else {
                        entity->getComponent<LifetimeComponent>()->timeAlive += deltaTime;
                    }
                }
            }
        }
    }
};

#endif