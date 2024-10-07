#include "EntityFactory.hpp"


std::shared_ptr<HumanMageEntity> buildHumanMage(ResourceAllocator &allocator, const float &x, const float &y) {
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
std::shared_ptr<NinjaEntity> buildNinja(std::shared_ptr<ResourceAllocator> allocator, const float &x, const float &y) {
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
    //  Sprite
    if (auto sprite_ptr = ninja_ptr->getComponent<SpriteComponent>()) {
        sprite_ptr->sprite.setTexture(*(allocator->loadTexture("../assets/textures/characters/ninja_sheet.png")));
        sprite_ptr->sprite.setTextureRect(sf::IntRect(0,0,32,32));
        sprite_ptr->sprite.setScale(sf::Vector2f(1.0,1.0));
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
    }
    //  Velocity
    if (auto velo_ptr = ninja_ptr->getComponent<VelocityComponent>()) {
        velo_ptr->xSpeed = 17.0f;
        velo_ptr->ySpeed = 17.0f;
        velo_ptr->moveAmount = 10.0f;
    }
    // Selectable
    if (auto select_ptr = ninja_ptr->getComponent<SelectableComponent>()) {
        select_ptr->isSelected = false;
    }
    // UUID
    if (auto uuid_ptr = ninja_ptr->getComponent<UUIDComponent>()) {
        uuid_ptr->ID = reinterpret_cast<uint64_t>(&ninja_ptr);
    }
    // FlipComponent
    if (auto flip_ptr = ninja_ptr->getComponent<FlipComponent>()) {
        flip_ptr->isFlipped = false;
    }
    // temp objective
    // ninja_ptr->addObjective(sf::Vector2f(400,400));

    return ninja_ptr;
}

// Healthbar
std::shared_ptr<HealthbarEntity> buildHealthbar(ResourceAllocator &allocator, std::shared_ptr<Entity> ownerEntity, const float &x, const float &y) {
    auto hp_ptr = std::make_shared<HealthbarEntity>(ownerEntity);
    // Setup components
    // Position
    if (auto pos_ptr = hp_ptr->getComponent<PositionComponent>()) {
        pos_ptr->x = x;
        pos_ptr->y = y;
    }
    // Sprite
    if (auto spr_ptr = hp_ptr->getComponent<SpriteComponent>()) {
        spr_ptr->sprite.setTexture(*allocator.loadTexture("../assets/textures/UI/health_bars.png"));
        tmx::Tileset tileset = allocator.getTileset("health_bars");
        spr_ptr->sprite.setTextureRect(sf::IntRect(0,0,tileset.getTileSize().x, tileset.getTileSize().y));
        spr_ptr->sprite.setOrigin(spr_ptr->sprite.getTextureRect().left/2, spr_ptr->sprite.getTextureRect().top/2);
    }
    // DeletableComponent
    if (auto del_ptr = hp_ptr->getComponent<DeletableComponent>()) {
        del_ptr->markedForDeletion = false;
    }
    // UUID
    if (auto uid_ptr = hp_ptr->getComponent<UUIDComponent>()) {
        uid_ptr->ID = reinterpret_cast<uint64_t>(&hp_ptr);
    }

    return hp_ptr;
}
