#ifndef _EYEBUG_
#define _EYEBUG_

#include "ECS/Entities/Entity.hpp"

class EyeBugEntity : public Entity {
public:
    EyeBugEntity() {
        this->addComponent(std::make_shared<PositionComponent>());
        this->addComponent(std::make_shared<SpriteComponent>());
        this->addComponent(std::make_shared<CollisionComponent>());
        this->addComponent(std::make_shared<AnimationComponent>());
        this->addComponent(std::make_shared<UUIDComponent>());

    }

    void update(float deltaTime) override {

    }
};

#endif