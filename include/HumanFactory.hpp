#ifndef _HUMAN_FACTORY_
#define _HUMAN_FACTORY_

#include <memory>

#include <SFML/Graphics.hpp>

#include "ResourceAllocator.hpp"

#include "ECS/Entities/Entity.hpp"
#include "ECS/Entities/Human.hpp"
#include "ECS/Components/Components.hpp"

std::unique_ptr<HumanEntity> buildHumanMage(ResourceAllocator &allocator, const float &x = 100, const float &y = 100) {
    auto humanptr = std::make_unique<HumanEntity>();

    if (auto health_ptr = humanptr->getComponent<HealthComponent>()) {
        health_ptr->currentHealth = 100;
        health_ptr->maxHealth = 100;
    }

    if (auto pos_ptr = humanptr->getComponent<PositionComponent>()) {
        pos_ptr->x = x;
        pos_ptr->y = y;
    }

    if (auto sprite_ptr = humanptr->getComponent<SpriteComponent>()) {
        sprite_ptr->sprite.setTexture(*allocator.loadTexture("../assets/textures/characters/MiniMage.png"));
        sprite_ptr->sprite.setTextureRect(sf::IntRect(0,0,32,32));
    }

    if (auto state_ptr = humanptr->getComponent<StateComponent>()) {
        state_ptr->state = state_ptr->ATTACK;
    }

    if (auto anime_ptr = humanptr->getComponent<AnimationComponent>()) {
        anime_ptr->animationIndex = 0;
        anime_ptr->elapsedTime = 0.0f;
    }

    if (auto colli_ptr = humanptr->getComponent<CollisionComponent>()) {
        colli_ptr->bounds = humanptr->getComponent<SpriteComponent>()->sprite.getGlobalBounds();
    }
    
    if (auto selec_ptr = humanptr->getComponent<SelectableComponent>()) {
        selec_ptr->isSelected = false;
    }

    return humanptr;
}

#endif