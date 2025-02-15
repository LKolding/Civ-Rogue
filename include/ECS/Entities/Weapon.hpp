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

    inline void update(float dt) {
        auto swordPosition   = this->getComponent<PositionComponent>(); // for syncing with sprite.pos
        auto weaponComponent = this->getComponent<WeaponComponent>();

        if (!this->getComponent<WeaponComponent>()->isAttacking) { 
            this->getComponent<WeaponComponent>()->elapsedTime += dt;
            return; 
        }

        
        if (auto weaPtr = this->getComponent<WeaponComponent>()) {
            if ( weaPtr->elapsedTime >= weaPtr->useTime) {
                weaPtr->elapsedTime = 0.0f;
                weaPtr->isAttacking = false;
                this->getComponent<SpriteComponent>()->isVisible = false;

            } else {
                weaPtr->elapsedTime += dt;
            }

        }
    }

    void perform_attack(std::shared_ptr<PositionComponent> entityPosition, sf::Vector2i mouse_pos) {
        //  Perform the sword swing (transform the sword sprite)
        auto weaponSprite    = this->getComponent<SpriteComponent>();
        auto swordPosition   = this->getComponent<PositionComponent>(); // for syncing with sprite.pos
        auto weaponComponent = this->getComponent<WeaponComponent>();

        if (!weaponSprite->isVisible) {
            weaponSprite->isVisible = true;
        }
        if (weaponComponent->isAttacking) {
            return;
        }
        else {
            weaponComponent->isAttacking = true;
        }

        

        if (weaponSprite->sprite.getRotation() < 250)
            weaponSprite->sprite.setRotation(0);

        weaponSprite->sprite.rotate(-10.0f);  // Example rotation value for the swing
    }
    

};

#endif