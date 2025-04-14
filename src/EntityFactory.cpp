#include "EntityFactory.hpp"

// Ninja
/*
std::shared_ptr<NinjaEntity> buildNinja(std::shared_ptr<ResourceAllocator> allocator, const float &x, const float &y) {
    auto ninja_ptr = std::make_shared<NinjaEntity>();
    //  Health
    if (auto health_ptr = ninja_ptr->getComponent<HealthComponent>()) {
        health_ptr->currentHealth = 90;
        health_ptr->maxHealth = 100;
    }
    //  Position
    if (auto pos_ptr = ninja_ptr->getComponent<PositionComponent>()) {
        pos_ptr->x = x;
        pos_ptr->y = y;
    }
    //  Sprite
    if (auto sprite_ptr = ninja_ptr->getComponent<SpriteComponent>()) {
        sprite_ptr->sprite.setTexture(*allocator->loadTexture("../assets/textures/characters/ninja_sheet.png"));
        sprite_ptr->sprite.setTextureRect(sf::IntRect(0,0,32,32));
        sprite_ptr->sprite.setOrigin(sf::Vector2f(sprite_ptr->sprite.getTextureRect().width/2, sprite_ptr->sprite.getTextureRect().height/2));
        sprite_ptr->hasBeenFlipped = false;
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
        ninja_ptr->transitionState(NinjaStateComponent::IDLE); 
    }
    //  Collision
    if (auto colli_ptr = ninja_ptr->getComponent<CollisionComponent>()) {
        colli_ptr->bounds = ninja_ptr->getComponent<SpriteComponent>()->sprite.getGlobalBounds();
        colli_ptr->bounds.width  -= 8;
        colli_ptr->bounds.height -= 8;
        colli_ptr->bounds.top  = ninja_ptr->getComponent<PositionComponent>()->y - (colli_ptr->bounds.height/2);
        colli_ptr->bounds.left = ninja_ptr->getComponent<PositionComponent>()->x - (colli_ptr->bounds.width/2);
    }
    //  Velocity
    if (auto velo_ptr = ninja_ptr->getComponent<MovementComponent>()) {
        velo_ptr->speed = 180.0f;
        velo_ptr->xDir = 0.0f;
        velo_ptr->yDir = 0.0f;
    }
    // UUID
    if (auto uuid_ptr = ninja_ptr->getComponent<UUIDComponent>()) {
        uuid_ptr->ID = reinterpret_cast<uint64_t>(&ninja_ptr);
    }
    // FlipComponent
    if (auto flip_ptr = ninja_ptr->getComponent<FlipComponent>()) {
        flip_ptr->isFlipped = false;
    }
    if (auto follow_ptr = ninja_ptr->getComponent<FollowComponent>()) {
        follow_ptr->isFollowing = false;
    }
    return ninja_ptr;
}
*/

EntityID buildNinja(ComponentManager& cm, EntityManager& em, float x, float y) {
    EntityID id = em.createEntity();

    const float XPOSITION = x;
    const float YPOSITION = y;
    const int MAX_HEALTH = 100;
    const int TEX_SIZE = 32;
    const std::string TEX_PATH = "../assets/textures/characters/ninja_sheet.png";
    
    cm.addComponent<PositionComponent>(id, {XPOSITION, YPOSITION});
    // cm.addComponent<HealthComponent>(id, {MAX_HEALTH, MAX_HEALTH});
    cm.addComponent<SpriteComponent>(id, {{{0,0},{TEX_SIZE,TEX_SIZE}}, {TEX_SIZE/2, TEX_SIZE/2}, TEX_PATH});
    cm.addComponent<StateComponent>(id, {"IDLE"});

    cm.addComponent<AnimationComponent>(id, {0, 0, 5});
    // construct animation map component (hopefully temporary solution)
    AnimationMapComponent animMap;
    animMap.textureHeightPerRow = TEX_SIZE;
    animMap.stateToAnimation["IDLE"] = { 0, 5, 0.22f };
    animMap.stateToAnimation["WALK"] = { 1, 5, 0.08f };
    animMap.stateToAnimation["ATCK"] = { 2, 0 };
    
    cm.addComponent<AnimationMapComponent>(id, animMap); //apply constructed component
    cm.addComponent<FlipComponent>(id, {});

    // Hitbox gets declared here
    cm.addComponent<BoundsComponent>(id, {{{XPOSITION, YPOSITION},{(float)TEX_SIZE-12, (float)TEX_SIZE-12}}});
    cm.addComponent<CollisionComponent>(id, {});
    
    cm.addComponent<MovementComponent>(id, {120.0f, 0.0f, 0.0f});
    cm.addComponent<HoverComponent>(id, {});
    cm.addComponent<SelectComponent>(id, {});
    cm.addComponent<ObjectiveComponent>(id, {});

    return id;
}