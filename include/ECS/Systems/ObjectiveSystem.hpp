#ifndef _OBJECTIVE_SYSTEM_
#define _OBJECTIVE_SYSTEM_

#include <vector>

#include "ECS/Systems/System.hpp"
#include "ECS/Entities/Entity.hpp"

class ObjectiveSystem: public System {
public:
    inline void update(float deltaTime, std::vector<std::weak_ptr<Entity>> entities) {
        for (auto& entity_p : entities) {
            if ( auto entity = entity_p.lock()) {
                if (entity->hasComponent<ObjectiveComponent>() && entity->hasComponent<PositionComponent>() && entity->getComponent<ObjectiveComponent>()->location) {
                    auto objective = entity->getComponent<ObjectiveComponent>();
                    auto position  = entity->getComponent<PositionComponent>();

                    if (objective->location->x != 0 || (int)objective->location->y != 0) {
                        //  x direction
                        if (objective->location->x < (int)position->x) {
                            entity->getComponent<VelocityComponent>()->xDir = -1.0f;
                        }
                        if (objective->location->x > (int)position->x) {
                            entity->getComponent<VelocityComponent>()->xDir = 1.0f;
                        }
                        //  y direction
                        if (objective->location->y < (int)position->y) {
                            entity->getComponent<VelocityComponent>()->yDir = -1.0f;
                        }
                        if (objective->location->y > (int)position->y) {
                            entity->getComponent<VelocityComponent>()->yDir = 1.0f;
                        }
                        
                    }
                    //  reset direction
                    if (objective->location->x == (int)position->x) {
                        entity->getComponent<VelocityComponent>()->xDir = 0.0f;
                    }
                    if (objective->location->y == (int)position->y) {
                        entity->getComponent<VelocityComponent>()->yDir = 0.0f;
                    }
                    //  reset objective
                    if (objective->location->x == (int)position->x && objective->location->y == (int)position->y) {
                        objective->addObjective(sf::Vector2i(0,0));
                    }
                }
            }
            
        }
    }
};
#endif
/*

if (this->hasComponent<ObjectiveComponent>() && this->getComponent<ObjectiveComponent>()->location){
            if (this->getComponent<ObjectiveComponent>()->location->x != 0 || (int)this->getComponent<ObjectiveComponent>()->location->y != 0) {
                this->transitionState(NinjaStateComponent::WALK);
                //  x direction
                if (this->getComponent<ObjectiveComponent>()->location->x < (int)this->getComponent<PositionComponent>()->x) {
                    this->getComponent<VelocityComponent>()->xDir = -1.0f;
                }
                if (this->getComponent<ObjectiveComponent>()->location->x > (int)this->getComponent<PositionComponent>()->x) {
                    this->getComponent<VelocityComponent>()->xDir = 1.0f;
                }
                //  y direction
                if (this->getComponent<ObjectiveComponent>()->location->y < (int)this->getComponent<PositionComponent>()->y) {
                    this->getComponent<VelocityComponent>()->yDir = -1.0f;
                }
                if (this->getComponent<ObjectiveComponent>()->location->y > (int)this->getComponent<PositionComponent>()->y) {
                    this->getComponent<VelocityComponent>()->yDir = 1.0f;
                }
                
            }
            //  reset direction
            if (this->getComponent<ObjectiveComponent>()->location->x == (int)this->getComponent<PositionComponent>()->x) {
                this->getComponent<VelocityComponent>()->xDir = 0.0f;
            }
            if (this->getComponent<ObjectiveComponent>()->location->y == (int)this->getComponent<PositionComponent>()->y) {
                this->getComponent<VelocityComponent>()->yDir = 0.0f;
            }
            //  reset objective
            if (this->getComponent<ObjectiveComponent>()->location->x == (int)this->getComponent<PositionComponent>()->x && this->getComponent<ObjectiveComponent>()->location->y == (int)this->getComponent<PositionComponent>()->y) {
                this->getComponent<ObjectiveComponent>()->addObjective(sf::Vector2i(0,0));
                this->transitionState(NinjaStateComponent::IDLE);
            }
        }

*/