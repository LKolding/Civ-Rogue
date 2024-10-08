#ifndef _EYEBUG_
#define _EYEBUG_

#include "ECS/Entities/Entity.hpp"

class EyeBugEntity : public Entity {
public:
    EyeBugEntity() {
        this->addComponent(std::make_shared<PositionComponent>());
        this->addComponent(std::make_shared<SpriteComponent>());
        this->addComponent(std::make_shared<CollisionComponent>());
        this->addComponent(std::make_shared<VelocityComponent>());
        this->addComponent(std::make_shared<AnimationComponent>());
        this->addComponent(std::make_shared<UUIDComponent>());
    }

    void update(float deltaTime) override {
        return;
        /*
        static const float thinkInterval = 2.0f;
        static float thinkTimer = 0;
        
        if (thinkTimer >= thinkInterval) {
            thinkTimer = 0.0f;

            if (rand() % 2 == 1) {
                if ((!this->getComponent<VelocityComponent>()->xDir == 0.0f) && (!this->getComponent<VelocityComponent>()->yDir == 0.0f)) {
                    this->getComponent<VelocityComponent>()->xDir = 0.0f; // reset
                    this->getComponent<VelocityComponent>()->yDir = 0.0f;

                } else {
                    this->getComponent<VelocityComponent>()->xDir = 1.0f * (rand() % 2);
                    this->getComponent<VelocityComponent>()->yDir = 1.0f * (rand() % 2);
                }

            }
        } else {
            thinkTimer += deltaTime;
        }*/
    }
};

#endif