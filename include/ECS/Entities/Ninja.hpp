#ifndef _NINJA_ENTITY_
#define _NINJA_ENTITY_

#include "ECS/Entities/Entity.hpp"
#include "ECS/Entities/Weapon.hpp"

#include "ECS/Components/Components.hpp"


class NinjaEntity: public Entity {
public:

    NinjaEntity() {
        this->addComponent(std::make_shared<PositionComponent>());
        this->addComponent(std::make_shared<SpriteComponent>());
        this->addComponent(std::make_shared<AnimationComponent>());
        this->addComponent(std::make_shared<HealthComponent>());
        this->addComponent(std::make_shared<NinjaStateComponent>());
        this->addComponent(std::make_shared<CollisionComponent>());
        this->addComponent(std::make_shared<VelocityComponent>());
        this->addComponent(std::make_shared<UUIDComponent>());
        this->addComponent(std::make_shared<FlipComponent>());
    }

    // behavior
    inline void update(float deltatime) { }

    inline void transitionState(NinjaStateComponent::State state) {
        switch (state) {
            case NinjaStateComponent::IDLE:
                this->getComponent<NinjaStateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 5;
                this->getComponent<AnimationComponent>()->frameTime = 0.1f;
                break;

            case NinjaStateComponent::WALK:
                this->getComponent<NinjaStateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 3;
                this->getComponent<AnimationComponent>()->frameTime = 0.03f;
                break;

            case NinjaStateComponent::ACTIVE:
                this->getComponent<NinjaStateComponent>()->state = state;
                this->getComponent<AnimationComponent>()->animationIndexMax = 0;
                this->getComponent<AnimationComponent>()->frameTime = 1.0f;
                break;

            default: // doesn't update internal state if not recognized
                break;

        }
    }

    //  Performs the default attack with the equipped weapon
    inline void weapon_attack(sf::Vector2f mouse_pos) {
        if (auto weaponp = this->equippedWeapon.lock()) {
            weaponp->perform_attack(this->getComponent<PositionComponent>(), mouse_pos);
        } 
    }

    inline void setEquippedWeapon(std::shared_ptr<WeaponEntity> weapon) {
        this->equippedWeapon = weapon;
    }

private:
    std::weak_ptr<WeaponEntity> equippedWeapon;
};

#endif