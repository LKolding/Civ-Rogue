#ifndef _ITEM_
#define _ITEM_

#include "ECS/Entities/Entity.hpp"
#include "ECS/Components/Components.hpp"

// BASE CLASS

class ItemEntity: public Entity {
public:
    ItemEntity() {
        this->addComponent(std::make_shared<SpriteComponent>());
        this->addComponent(std::make_shared<PositionComponent>());
    }

    void update(float dt) { return; };
};

#endif