#ifndef _WELL_ENTITY_
#define _WELL_ENTITY_

#include "ECS/Entities/Entity.hpp"

class MossyWellEntity : public Entity {
public:
    MossyWellEntity() {
        this->addComponent(std::make_shared<PositionComponent>());
        this->addComponent(std::make_shared<SpriteComponent>());
        this->addComponent(std::make_shared<StaticCollisionComponent>());
        this->addComponent(std::make_shared<UUIDComponent>());
        this->addComponent(std::make_shared<InteractableComponent>());
    }
    void update(float dt) override { return; }
};

#endif