#ifndef _BLUE_POINTER_
#define _BLUE_POINTER_

#include "ECS/Entities/Entity.hpp"
#include "ECS/Components/Components.hpp"

class BluePointerEntity: public Entity {
    public:
    BluePointerEntity() {
        this->addComponent(std::make_shared<SpriteComponent>());
        this->addComponent(std::make_shared<UUIDComponent>());
        this->addComponent(std::make_shared<LifetimeComponent>());
        this->addComponent(std::make_shared<DeletableComponent>());
        this->addComponent(std::make_shared<AnimationComponent>());
    }

    void update(float dt) {
        return;
    }
};

#endif