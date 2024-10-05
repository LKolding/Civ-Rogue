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
    }

    void update(float deltatime) {
        if (this->getComponent<ObjectiveComponent>()->location->x != 0.0f || this->getComponent<ObjectiveComponent>()->location->y != 0.0f) {
             //  x
            if ((int)this->getComponent<ObjectiveComponent>()->location->x < (int)this->getComponent<PositionComponent>()->x) {
                this->getComponent<VelocityComponent>()->xDir = -1.0f;
            }
            if ((int)this->getComponent<ObjectiveComponent>()->location->x > (int)this->getComponent<PositionComponent>()->x) {
                this->getComponent<VelocityComponent>()->xDir = 1.0f;
            }
            //  y
            if ((int)this->getComponent<ObjectiveComponent>()->location->y < (int)this->getComponent<PositionComponent>()->y) {
                this->getComponent<VelocityComponent>()->yDir = -1.0f;
            }
            if ((int)this->getComponent<ObjectiveComponent>()->location->y > (int)this->getComponent<PositionComponent>()->y) {
                this->getComponent<VelocityComponent>()->yDir = 1.0f;
            }
            
        }
        //  reset direction
        if ((int)this->getComponent<ObjectiveComponent>()->location->x == (int)this->getComponent<PositionComponent>()->x) {
            this->getComponent<VelocityComponent>()->xDir = 0.0f;
        }
        if ((int)this->getComponent<ObjectiveComponent>()->location->y == (int)this->getComponent<PositionComponent>()->y) {
            this->getComponent<VelocityComponent>()->yDir = 0.0f;
        }
        //  reset objective
        if ((int) this->getComponent<ObjectiveComponent>()->location->x == (int)this->getComponent<PositionComponent>()->x && (int)this->getComponent<ObjectiveComponent>()->location->y == (int)this->getComponent<PositionComponent>()->y) {
            this->addObjective(sf::Vector2f(0.0f,0.0f));
            this->transitionState(NinjaStateComponent::IDLE);
        }
        
    }

    void addObjective(sf::Vector2f pos) {
        this->getComponent<ObjectiveComponent>()->location = std::make_unique<sf::Vector2f>(pos);
    }

    void transitionState(NinjaStateComponent::State state) {

        switch (state) {
            case NinjaStateComponent::IDLE:
                this->getComponent<NinjaStateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 5;
                this->getComponent<AnimationComponent>()->frameTime = 2.4f;
                break;

            case NinjaStateComponent::WALK:
                this->getComponent<NinjaStateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 3;
                this->getComponent<AnimationComponent>()->frameTime = 1.4f;
                break;

            default: // doesn't update internal state if not recognized
                break;

        }
    }
};

#endif