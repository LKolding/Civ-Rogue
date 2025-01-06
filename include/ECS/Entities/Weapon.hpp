#ifndef _WEAPON_ENTITY_
#define _WEAPON_ENTITY_

#include "ECS/Entities/Entity.hpp"
#include "ECS/Components/Components.hpp"

class WeaponEntity: public Entity {
public:
    WeaponEntity() {
        this->addComponent(std::make_shared<PositionComponent>());
        this->addComponent(std::make_shared<SpriteComponent>());
        //this->addComponent(std::make_shared<CollisionComponent>());
        this->addComponent(std::make_shared<WeaponComponent>());
        this->addComponent(std::make_shared<UUIDComponent>());
    }

    void update(float dt) { 
        this->getComponent<SpriteComponent>()->sprite.setPosition(this->getComponent<PositionComponent>()->x, this->getComponent<PositionComponent>()->y);
    
    }

    void perform_attack(std::shared_ptr<PositionComponent> entityPosition) {
        //  Perform the sword swing (transform the sword sprite)
        auto weaponSprite  = this->getComponent<SpriteComponent>();
        auto swordPosition = this->getComponent<PositionComponent>(); // for syncing with sprite.pos
        auto weaponComponent = this->getComponent<WeaponComponent>();

        if (!weaponSprite->isVisible)
            weaponSprite->isVisible = true;

        //  Set sword's position to player's position and rotate around the handle
        swordPosition->x = entityPosition->x;
        swordPosition->y = entityPosition->y;
        if (weaponSprite->sprite.getRotation() < 250)
            weaponSprite->sprite.setRotation(0);

        weaponSprite->sprite.rotate(-10.0f);  // Example rotation value for the swing
    }
    

};

#endif