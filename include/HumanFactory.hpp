#ifndef _HUMAN_FACTORY_
#define _HUMAN_FACTORY_

#include <memory>

#include <SFML/Graphics.hpp>

#include "ResourceAllocator.hpp"

#include "ECS/Entities/Entity.hpp"
#include "ECS/Entities/Human.hpp"
#include "ECS/Components/Components.hpp"

// if (this->hasComponent<PositionComponent>()) {
//             auto posptr = this->getComponent<PositionComponent>();
//             posptr->x = 20;
//             posptr->y = 20;
//         }

//         if (this->hasComponent<HealthComponent>()) {
//             auto healthptr = this->getComponent<HealthComponent>();
//             healthptr->maxHealth = 100;
//             healthptr->currentHealth = 100;
//         }

//         if (this->hasComponent<SpriteComponent>()) {
//             auto spritePtr = this->getComponent<SpriteComponent>();
//             spritePtr->sprite.setTexture();
//         }


std::unique_ptr<HumanEntity> buildHuman(ResourceAllocator &allocator) {
    auto humanptr = std::make_unique<HumanEntity>();

    if (auto health_ptr = humanptr->getComponent<HealthComponent>()) {
        health_ptr->currentHealth = 100;
        health_ptr->maxHealth = 100;
    }

    if (auto pos_ptr = humanptr->getComponent<PositionComponent>()) {
        pos_ptr->x = 30;
        pos_ptr->y = 50;
    }

    if (auto sprite_ptr = humanptr->getComponent<SpriteComponent>()) {
        sprite_ptr->sprite.setTexture(*allocator.loadTexture("../assets/textures/characters/MiniMage.png"));
        sprite_ptr->sprite.setTextureRect(sf::IntRect(0,0,32,32));
    }
    
    return humanptr;
}

#endif