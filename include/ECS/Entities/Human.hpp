#ifndef _HUMAN_
#define _HUMAN_

#include "Entity.hpp"
#include "ECS/Components/Components.hpp"


class HumanMageEntity: public Entity {
public:
    HumanMageEntity() : Entity() {
        this->addComponent(std::make_shared<PositionComponent>());
        this->addComponent(std::make_shared<SpriteComponent>());
        this->addComponent(std::make_shared<HealthComponent>());
        this->addComponent(std::make_shared<MageStateComponent>());
        this->addComponent(std::make_shared<AnimationComponent>());
        this->addComponent(std::make_shared<CollisionComponent>());
        this->addComponent(std::make_shared<SelectableComponent>());
    }

    void transitionState(MageStateComponent::State state) {
        switch (state) {
            case MageStateComponent::IDLE:
                this->getComponent<MageStateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 3;
                this->getComponent<AnimationComponent>()->frameTime = 6.4f;
                break;

            case MageStateComponent::WALK:
                this->getComponent<MageStateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 5;
                this->getComponent<AnimationComponent>()->frameTime = 2.0f;
                break;

            case MageStateComponent::REST:
                this->getComponent<MageStateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 2;
                this->getComponent<AnimationComponent>()->frameTime = 5.0f;
                break;

            case MageStateComponent::ATTACK:
                this->getComponent<MageStateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 10;
                this->getComponent<AnimationComponent>()->frameTime = 2.0f;
                break;

            case MageStateComponent::ATTACK2:
                this->getComponent<MageStateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 8;
                this->getComponent<AnimationComponent>()->frameTime = 2.0f;
                break;

            case MageStateComponent::ATTACK3:
                this->getComponent<MageStateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 8;
                this->getComponent<AnimationComponent>()->frameTime = 2.0f;
                break;
            
            case MageStateComponent::TEMP:
                this->getComponent<MageStateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 1;
                this->getComponent<AnimationComponent>()->frameTime = 3.0f;
                break;

            case MageStateComponent::DEATH:
                this->getComponent<MageStateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 8;
                this->getComponent<AnimationComponent>()->frameTime = 2.0f;
                break;

        }
    }

    void update(float deltatime) {

    }
};

#endif