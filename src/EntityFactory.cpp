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

// Border
std::shared_ptr<BorderEntity> buildBorder(std::shared_ptr<ResourceAllocator> allocator, const float &x, const float &y) {
    auto bor_ptr = std::make_shared<BorderEntity>();

    if (auto pos_ptr = bor_ptr->getComponent<PositionComponent>()) {
        pos_ptr->x = x;
        pos_ptr->y = y;
    }
    if (auto spr_ptr = bor_ptr->getComponent<SpriteComponent>()) {
        spr_ptr->sprite.setTexture(*allocator->loadTexture("../assets/effects/shadow_realm_border.png"));
        spr_ptr->sprite.setTextureRect(sf::IntRect(sf::Vector2i(00,0), sf::Vector2i(256, 32)));
    }
    if (auto ani_ptr = bor_ptr->getComponent<AnimationComponent>()) {
        ani_ptr->frameTime = 0.1f;
        ani_ptr->animationIndex = 0;
        ani_ptr->elapsedTime = 0.0f;
        ani_ptr->animationIndexMax = 5;
        
    }
    if (auto uid_ptr = bor_ptr->getComponent<UUIDComponent>()) {
        uid_ptr->ID = reinterpret_cast<uint64_t>(&bor_ptr);
    }
    return bor_ptr;
}

// Button
std::shared_ptr<ButtonEntity> buildButton(std::shared_ptr<ResourceAllocator> allocator, const float &x, const float &y) {
    auto btn_ptr = std::make_shared<ButtonEntity>();

    if (auto pos_ptr = btn_ptr->getComponent<PositionComponent>()) {
        pos_ptr->x = x;
        pos_ptr->y = y;
    }

    if (auto spr_ptr = btn_ptr->getComponent<SpriteComponent>()) {
        spr_ptr->hasBeenFlipped = false;
        spr_ptr->sprite.setTexture(*allocator->loadTexture("../assets/textures/UI/button.png"));
        spr_ptr->sprite.setTextureRect(sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(spr_ptr->sprite.getTexture()->getSize().x, spr_ptr->sprite.getTexture()->getSize().y)));
        spr_ptr->sprite.scale(0.1,0.1);
        spr_ptr->sprite.setPosition(btn_ptr->getComponent<PositionComponent>()->x, btn_ptr->getComponent<PositionComponent>()->y);
    }
    if (auto col_ptr = btn_ptr->getComponent<CollisionComponent>()) {
        col_ptr->bounds = btn_ptr->getComponent<SpriteComponent>()->sprite.getGlobalBounds();
    }

    return btn_ptr;
}

// Eyebug entity
std::shared_ptr<EyeBugEntity> buildEyeBug(std::shared_ptr<ResourceAllocator> allocator, const float &x, const float &y) {
    auto eye_ptr = std::make_shared<EyeBugEntity>();

    if (auto pos_ptr = eye_ptr->getComponent<PositionComponent>()) {
        pos_ptr->x = x;
        pos_ptr->y = y;
    }
    if (auto spr_ptr = eye_ptr->getComponent<SpriteComponent>()) {
        spr_ptr->sprite.setTexture(*allocator->loadTexture("../assets/textures/characters/eyebug.png"));
        spr_ptr->sprite.setTextureRect(sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(32, 32)));
    }
    if (auto ani_ptr = eye_ptr->getComponent<AnimationComponent>()) {
        ani_ptr->animationIndex = 0;
        ani_ptr->elapsedTime = 0.0f;
        ani_ptr->frameTime = 0.1;
        ani_ptr->animationIndexMax = 96;
        
    }
    if (auto uid_ptr = eye_ptr->getComponent<UUIDComponent>()) {
        uid_ptr->ID = reinterpret_cast<uint64_t>(&eye_ptr);
    }
    if (auto vel_ptr = eye_ptr->getComponent<VelocityComponent>()) {
        vel_ptr->xSpeed = 17.0f;
        vel_ptr->ySpeed = 17.0f;
        vel_ptr->moveAmount = 10.0f;
    }
    return eye_ptr;
}