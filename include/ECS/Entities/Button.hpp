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
        this->addComponent(std::make_shared<CollisionComponent>());
        this->addComponent(std::make_shared<UUIDComponent>());
    }
    inline void click() {
        std::cout << "CLICKED\n";
    }
    inline void update(float deltaTime) {
        this->getComponent<SpriteComponent>()->sprite.setPosition(this->getComponent<PositionComponent>()->x, this->getComponent<PositionComponent>()->y);
    }

};

#endif