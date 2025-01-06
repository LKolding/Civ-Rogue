#ifndef _ICON_ENTITY_
#define _ICON_ENTITY_

#include "ECS/Entities/Entity.hpp"

class IconEntity: public Entity {
public:
    // Texture name will be fx. "axe.png". The entire path will be added automatically
    IconEntity() {
        this->addComponent(std::make_shared<SpriteComponent>());
        this->addComponent(std::make_shared<PositionComponent>());
        this->addComponent(std::make_shared<UUIDComponent>());
    }
    void update(float dt) { return; };
};

#endif