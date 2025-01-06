#ifndef _MAIN_CHARACTER_
#define _MAIN_CHARACTER_

#include "ECS/Entities/Entity.hpp"
#include "ECS/Components/Components.hpp"

class MainCharacterEntity: public Entity {
public:
    MainCharacterEntity() { 
        this->addComponent(std::make_shared<PositionComponent>());
        this->addComponent(std::make_shared<SpriteComponent>());
        this->addComponent(std::make_shared<AnimationComponent>());
        this->addComponent(std::make_shared<VelocityComponent>());
        this->addComponent(std::make_shared<PositionComponent>());
        this->addComponent(std::make_shared<HealthComponent>());
        this->addComponent(std::make_shared<UUIDComponent>());
        this->addComponent(std::make_shared<FlipComponent>());
    }
    void update(float dt) { return; }
};

#endif