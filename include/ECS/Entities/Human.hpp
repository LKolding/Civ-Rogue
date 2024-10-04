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
        this->addComponent(std::make_shared<StateComponent>());
        this->addComponent(std::make_shared<AnimationComponent>());
        this->addComponent(std::make_shared<CollisionComponent>());
        this->addComponent(std::make_shared<SelectableComponent>());
    }

    void transitionState(StateComponent::State state) {
        switch (state) {
            case StateComponent::IDLE:
                this->getComponent<StateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 3;
                this->getComponent<AnimationComponent>()->frameTime = 6.4f;
                break;

            case StateComponent::WALK:
                this->getComponent<StateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 5;
                this->getComponent<AnimationComponent>()->frameTime = 2.0f;
                break;

            case StateComponent::REST:
                this->getComponent<StateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 2;
                this->getComponent<AnimationComponent>()->frameTime = 5.0f;
                break;

            case StateComponent::ATTACK:
                this->getComponent<StateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 10;
                this->getComponent<AnimationComponent>()->frameTime = 2.0f;
                break;

            case StateComponent::ATTACK2:
                this->getComponent<StateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 8;
                this->getComponent<AnimationComponent>()->frameTime = 2.0f;
                break;

            case StateComponent::ATTACK3:
                this->getComponent<StateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 8;
                this->getComponent<AnimationComponent>()->frameTime = 2.0f;
                break;
            
            case StateComponent::TEMP:
                this->getComponent<StateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 1;
                this->getComponent<AnimationComponent>()->frameTime = 3.0f;
                break;

            case StateComponent::DEATH:
                this->getComponent<StateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 8;
                this->getComponent<AnimationComponent>()->frameTime = 2.0f;
                break;

        }
    }

    void update(float deltatime) {
        // all of the code in this function updating the animation
        // component that then in turn updates the textRect of the sprite
        // is pretty hardcoded and should be refactored to support
        // all the different states of the sprites which all have
        // variable duration of their animations

        //  update sprite position
        float x = this->getComponent<PositionComponent>()->x;
        float y = this->getComponent<PositionComponent>()->y;
        this->getComponent<SpriteComponent>()->sprite.setPosition(x, y);
        //  update sprite texture rectangle to match up with the animation index of AnimationComponent
        sf::IntRect textRect = this->getComponent<SpriteComponent>()->sprite.getTextureRect(); // get
        textRect.left = 32 * this->getComponent<AnimationComponent>()->animationIndex;         // update frame index     (x)
        textRect.top = 32 * this->getComponent<StateComponent>()->state;                       // update animation index (y)
        this->getComponent<SpriteComponent>()->sprite.setTextureRect(textRect);                // set
        //  update animation component's elapsed time
        this->getComponent<AnimationComponent>()->elapsedTime += deltatime;
        //  update animation component animationIndex (is enough time has passed)
        if (this->getComponent<AnimationComponent>()->elapsedTime >= this->getComponent<AnimationComponent>()->frameTime) {
            this->getComponent<AnimationComponent>()->elapsedTime = 0.0f;

            const int animationSheetWidth = this->getComponent<SpriteComponent>()->sprite.getTexture()->getSize().x;
            const int animationIndex = this->getComponent<AnimationComponent>()->animationIndex;
            if (
                (animationIndex >= (animationSheetWidth/32)-1) | (animationIndex >= this->getComponent<AnimationComponent>()->animationIndexMax)
                ) {
                this->getComponent<AnimationComponent>()->animationIndex = 0;  //  reset
            }
            else {
                this->getComponent<AnimationComponent>()->animationIndex += 1; //  increment
            }
        }

        // update collision component
        this->getComponent<CollisionComponent>()->bounds = this->getComponent<SpriteComponent>()->sprite.getGlobalBounds();
    }
};

#endif