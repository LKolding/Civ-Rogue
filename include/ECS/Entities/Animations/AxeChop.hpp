#ifndef _AXE_ANIMATION_
#define _AXE_ANIMATION_

#include "ECS/Entities/Entity.hpp"
#include "ECS/Components/Components.hpp"

class AxeChopEntity: public Entity {
public:
    AxeChopEntity(std::shared_ptr<Entity> ownerEntity) {
        if (ownerEntity) { this->ownerEntity = ownerEntity; }
        this->addComponent(std::make_shared<SpriteComponent>());
        this->addComponent(std::make_shared<AnimationComponent>());
        this->addComponent(std::make_shared<UUIDComponent>());
    }

    void update(float dt) {
        if (auto ent_p = this->ownerEntity.lock()) {
            this->getComponent<SpriteComponent>()->sprite.setPosition(ent_p->getComponent<PositionComponent>()->x, ent_p->getComponent<PositionComponent>()->y-12);
        }
    }

private:
    std::weak_ptr<Entity> ownerEntity;
};

#endif