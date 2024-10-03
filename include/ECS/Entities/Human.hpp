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
    }

    void update(float deltatime) {
        float x = this->getComponent<PositionComponent>()->x;
        float y = this->getComponent<PositionComponent>()->y;
        this->getComponent<SpriteComponent>()->sprite.setPosition(x, y);
    }
};

#endif