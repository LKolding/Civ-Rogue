#ifndef _INVENTORY_ENTITY_
#define _INVENTORY_ENTITY_

#include "ECS/Entities/Entity.hpp"
#include "ECS/Components/Components.hpp"

class InventoryEntity: public Entity {
public:
    InventoryEntity() {
        this->addComponent(std::make_shared<PositionComponent>());
        this->addComponent(std::make_shared<SpriteComponent>());
        this->addComponent(std::make_shared<UUIDComponent>());
        this->addComponent(std::make_shared<VelocityComponent>());
    }

    void update(float dt) {return;};
};

#endif