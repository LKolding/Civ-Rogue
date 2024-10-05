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
#include "ECS/Entities/Ninja.hpp"
#include "ECS/Components/Components.hpp"


std::shared_ptr<HumanMageEntity> buildHumanMage(ResourceAllocator &allocator, const float &x = 100, const float &y = 100) {
    auto humanptr = std::make_shared<HumanMageEntity>();
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
    if (auto state_ptr = humanptr->getComponent<MageStateComponent>()) {
        state_ptr->state = state_ptr->IDLE;
    }
    //  Animation
    if (auto anime_ptr = humanptr->getComponent<AnimationComponent>()) {
        anime_ptr->animationIndex = 0;
        anime_ptr->elapsedTime = 0.0f;
        //  ensures animationIndexMax get's set up properly
        humanptr->transitionState(MageStateComponent::IDLE); 
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


// Ninja
std::shared_ptr<NinjaEntity> buildNinja(ResourceAllocator &allocator, const float &x = 100 ,const float &y = 100) {
    auto ninja_ptr = std::make_shared<NinjaEntity>();

    //  Health
    if (auto health_ptr = ninja_ptr->getComponent<HealthComponent>()) {
        health_ptr->currentHealth = 100;
        health_ptr->maxHealth = 100;
    }
    //  Position
    if (auto pos_ptr = ninja_ptr->getComponent<PositionComponent>()) {
        pos_ptr->x = x;
        pos_ptr->y = y;
    }
    //  Texture
    if (auto sprite_ptr = ninja_ptr->getComponent<SpriteComponent>()) {
        sprite_ptr->sprite.setTexture(*allocator.loadTexture("../assets/textures/characters/ninja_sheet.png"));
        sprite_ptr->sprite.setTextureRect(sf::IntRect(0,0,32,32));
        sprite_ptr->sprite.setScale(sf::Vector2f(1.0,1.0));
    }
    //  State
    if (auto state_ptr = ninja_ptr->getComponent<NinjaStateComponent>()) {
        state_ptr->state = state_ptr->WALK;
    }
    //  Animation
    if (auto anime_ptr = ninja_ptr->getComponent<AnimationComponent>()) {
        anime_ptr->animationIndex = 0;
        anime_ptr->elapsedTime = 0.0f;
        //  ensures animationIndexMax get's set up properly
        ninja_ptr->transitionState(NinjaStateComponent::WALK); 
    }
    //  Collision
    if (auto colli_ptr = ninja_ptr->getComponent<CollisionComponent>()) {
        colli_ptr->bounds = ninja_ptr->getComponent<SpriteComponent>()->sprite.getGlobalBounds();
    }
    //  Velocity
    if (auto velo_ptr = ninja_ptr->getComponent<VelocityComponent>()) {
        velo_ptr->deceleration = 0.5f;
        velo_ptr->xSpeed = 4.0f;
        velo_ptr->ySpeed = 4.0f;
    }
    // UUID
    if (auto uuid_ptr = ninja_ptr->getComponent<UUIDComponent>()) {
        uuid_ptr->ID = reinterpret_cast<uint64_t>(&ninja_ptr);
    }
    // temp objective
    ninja_ptr->addObjective(sf::Vector2f(400,400));

    return ninja_ptr;
}

#endif