#ifndef _ARROW_
#define _ARROW_

#include "ECS/Entities/Entity.hpp"
#include "ECS/Components/Components.hpp"


class ArrowEntity: public Entity {
public:
    ArrowEntity() {
        this->addComponent(std::make_shared<SpriteComponent>());
        this->addComponent(std::make_shared<UUIDComponent>());
        this->addComponent(std::make_shared<LifetimeComponent>());
        this->addComponent(std::make_shared<DeletableComponent>());
    }

    void update(float dt) {
        return;
    }

};

#endif