#ifndef _NINJA_ENTITY_
#define _NINJA_ENTITY_

#include "ECS/Entities/Entity.hpp"

#include "ECS/Components/Components.hpp"



class NinjaEntity: public Entity {
public:

    NinjaEntity() {
        this->addComponent(std::make_shared<PositionComponent>());
        this->addComponent(std::make_shared<SpriteComponent>());
        this->addComponent(std::make_shared<AnimationComponent>());
        this->addComponent(std::make_shared<HealthComponent>());
        this->addComponent(std::make_shared<NinjaStateComponent>());
        this->addComponent(std::make_shared<CollisionComponent>());
        this->addComponent(std::make_shared<SelectableComponent>());
        this->addComponent(std::make_shared<VelocityComponent>());
        this->addComponent(std::make_shared<ObjectiveComponent>());
        this->addComponent(std::make_shared<UUIDComponent>());
        this->addComponent(std::make_shared<FlipComponent>());
    }

    void update(float deltatime) {
        if (this->hasComponent<ObjectiveComponent>() && this->getComponent<ObjectiveComponent>()->location){
            if (this->getComponent<ObjectiveComponent>()->location->x != 0 || (int)this->getComponent<ObjectiveComponent>()->location->y != 0) {
                this->transitionState(NinjaStateComponent::WALK);
                //  x
                if (this->getComponent<ObjectiveComponent>()->location->x < (int)this->getComponent<PositionComponent>()->x) {
                    this->getComponent<VelocityComponent>()->xDir = -1.0f;
                }
                if (this->getComponent<ObjectiveComponent>()->location->x > (int)this->getComponent<PositionComponent>()->x) {
                    this->getComponent<VelocityComponent>()->xDir = 1.0f;
                }
                //  y
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
        
    }

    void transitionState(NinjaStateComponent::State state) {
        switch (state) {
            case NinjaStateComponent::IDLE:
                this->getComponent<NinjaStateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 5;
                this->getComponent<AnimationComponent>()->frameTime = 5.4f;
                break;

            case NinjaStateComponent::WALK:
                this->getComponent<NinjaStateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 3;
                this->getComponent<AnimationComponent>()->frameTime = 3.4f;
                break;

            default: // doesn't update internal state if not recognized
                break;

        }
    }
};

#endif