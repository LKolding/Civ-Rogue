#ifndef _HUMAN_
#define _HUMAN_

#include "Entity.hpp"
#include "ECS/Components/Components.hpp"

struct Human {  // temp 
public:
    Human(int intelligence, int strength, int speed) {
        this->intelligence = intelligence;
        this->strength = strength;
        this->speed = speed;
    };
    int getIntelligence() { return this->intelligence; };
    int getStrength() { return this->strength; };
    int getSpeed() { return this->speed; }

private:
    int intelligence;  // Gained by spending time in educational facility
    int strength;      // Gained by spending time in gym facility
    int speed;
};


class HumanEntity: public Entity {
public:
    HumanEntity() : Entity() {
        this->addComponent(std::make_shared<PositionComponent>());
        this->addComponent(std::make_shared<SpriteComponent>());
        this->addComponent(std::make_shared<HealthComponent>());
        this->addComponent(std::make_shared<StateComponent>());
        this->addComponent(std::make_shared<AnimationComponent>());
        this->addComponent(std::make_shared<CollisionComponent>());
        this->addComponent(std::make_shared<SelectableComponent>());
    }

    void update(float deltatime) {
        // all of the code in this function updating the animation
        // component that then in turn updates the textRect of the sprite
        // is pretty hardcoded and should be refactored to support
        // all the different states of the sprites which all have
        // variable duration of their animations

        // update sprite position
        float x = this->getComponent<PositionComponent>()->x;
        float y = this->getComponent<PositionComponent>()->y;
        this->getComponent<SpriteComponent>()->sprite.setPosition(x, y);
        // update animation component's elapsed time
        this->getComponent<AnimationComponent>()->elapsedTime += deltatime;
        // update sprite texture rectangle to match up with the animation index of AnimationComponent
        sf::IntRect textRect = this->getComponent<SpriteComponent>()->sprite.getTextureRect(); // get
        textRect.left = 32 * this->getComponent<AnimationComponent>()->animationIndex;         // update frame index     (x)
        textRect.top = 32 * this->getComponent<StateComponent>()->state;                       // update animation index (y)
        this->getComponent<SpriteComponent>()->sprite.setTextureRect(textRect);                // set
        // update animation component animationIndex (is enough time has passed)
        if (this->getComponent<AnimationComponent>()->elapsedTime >= 2.0f) {
            this->getComponent<AnimationComponent>()->elapsedTime = 0.0f;

            const int animationSheetWidth = this->getComponent<SpriteComponent>()->sprite.getTexture()->getSize().x;
            if (this->getComponent<AnimationComponent>()->animationIndex >= (animationSheetWidth/32)-1) {
                this->getComponent<AnimationComponent>()->animationIndex = 0;
            }
            else {
                this->getComponent<AnimationComponent>()->animationIndex += 1;
            }
        }

        // update collision component
        this->getComponent<CollisionComponent>()->bounds = this->getComponent<SpriteComponent>()->sprite.getGlobalBounds();
    }
};

#endif