#ifndef _BUTTON_
#define _BUTTON_

#include <SFML/Graphics.hpp>

#include "ECS/Entities/Entity.hpp"

#include "ResourceAllocator.hpp"

class ButtonEntity : public Entity {
public:

    ButtonEntity() {
        this->addComponent(std::make_shared<PositionComponent>());
        this->addComponent(std::make_shared<SpriteComponent>());
    }

    inline void update(float deltaTime) {
        this->getComponent<SpriteComponent>()->sprite.setPosition(this->getComponent<PositionComponent>()->x, this->getComponent<PositionComponent>()->y);
    }

};

#endif