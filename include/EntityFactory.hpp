#ifndef _ENTITY_FACTORY_
#define _ENTITY_FACTORY_
// system
#include <memory>
// sfml
#include <SFML/Graphics.hpp>
// my stuff
#include "ResourceAllocator.hpp"
// ecs
#include "ECS/Entities/Entity.hpp"
#include "ECS/Entities/Human.hpp"
#include "ECS/Components/Components.hpp"


std::unique_ptr<HumanMageEntity> buildHumanMage(ResourceAllocator &allocator, const float &x = 100, const float &y = 100) {
    auto humanptr = std::make_unique<HumanMageEntity>();
    //  Health
    if (auto health_ptr = humanptr->getComponent<HealthComponent>()) {
        health_ptr->currentHealth = 100;
        health_ptr->maxHealth = 100;
    }
    //  Position
    if (auto pos_ptr = humanptr->getComponent<PositionComponent>()) {
        pos_ptr->x = x;
        pos_ptr->y = y;
    }
    //  Texture
    if (auto sprite_ptr = humanptr->getComponent<SpriteComponent>()) {
        sprite_ptr->sprite.setTexture(*allocator.loadTexture("../assets/textures/characters/MiniMage.png"));
        sprite_ptr->sprite.setTextureRect(sf::IntRect(0,0,32,32));
        sprite_ptr->sprite.setScale(sf::Vector2f(1.2,1.2));
    }
    //  State
    if (auto state_ptr = humanptr->getComponent<StateComponent>()) {
        state_ptr->state = state_ptr->IDLE;
    }
    //  Animation
    if (auto anime_ptr = humanptr->getComponent<AnimationComponent>()) {
        anime_ptr->animationIndex = 0;
        anime_ptr->elapsedTime = 0.0f;
        //  ensures animationIndexMax get's set up properly
        humanptr->transitionState(StateComponent::IDLE); 
    }
    //  Collision
    if (auto colli_ptr = humanptr->getComponent<CollisionComponent>()) {
        colli_ptr->bounds = humanptr->getComponent<SpriteComponent>()->sprite.getGlobalBounds();
    }
    //  Selectable
    if (auto selec_ptr = humanptr->getComponent<SelectableComponent>()) {
        selec_ptr->isSelected = false;
    }

    return humanptr;
}

#endif