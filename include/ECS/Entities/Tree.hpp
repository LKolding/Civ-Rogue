#ifndef _TREE_
#define _TREE_

#include "ECS/Entities/Entity.hpp"
#include "ECS/Components/Components.hpp"

class TreeEntity: public Entity {
public:
    TreeEntity() {
        this->addComponent(std::make_shared<PositionComponent>());
        this->addComponent(std::make_shared<SpriteComponent>());
        this->addComponent(std::make_shared<HealthComponent>());
        this->addComponent(std::make_shared<UUIDComponent>());
    }
    void update(float dt) {
        return;
    }

};

#endif