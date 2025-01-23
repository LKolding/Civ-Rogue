#include "EntityFactory.hpp"

// Ninja
std::shared_ptr<NinjaEntity>  buildNinja(std::shared_ptr<ResourceAllocator> allocator, const float &x,const float &y) {
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
    if (auto velo_ptr = ninja_ptr->getComponent<VelocityComponent>()) {
        velo_ptr->xSpeed = 17.0f;
        velo_ptr->ySpeed = 17.0f;
        velo_ptr->moveAmount = 10.0f;
    }
    // UUID
    if (auto uuid_ptr = ninja_ptr->getComponent<UUIDComponent>()) {
        uuid_ptr->ID = reinterpret_cast<uint64_t>(&ninja_ptr);
    }
    // FlipComponent
    if (auto flip_ptr = ninja_ptr->getComponent<FlipComponent>()) {
        flip_ptr->isFlipped = false;
    }
    return ninja_ptr;
}
// Healthbar
std::shared_ptr<HealthbarEntity> buildHealthbar(std::shared_ptr<ResourceAllocator> allocator, std::shared_ptr<Entity> ownerEntity, const float &x, const float &y) {
    auto hp_ptr = std::make_shared<HealthbarEntity>(ownerEntity);
    // Setup components
    // Position
    if (auto pos_ptr = hp_ptr->getComponent<PositionComponent>()) {
        pos_ptr->x = x;
        pos_ptr->y = y;
    }
    // Sprite
    if (auto spr_ptr = hp_ptr->getComponent<SpriteComponent>()) {
        spr_ptr->sprite.setTexture(*allocator->loadTexture("../assets/textures/UI/health_bars.png"));
        tmx::Tileset tileset = allocator->getTileset("health_bars");
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
// BUtton
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
    if (auto uid_ptr = btn_ptr->getComponent<UUIDComponent>()) {
        uid_ptr->ID = reinterpret_cast<uint64_t>(&(*btn_ptr));
    }
    return btn_ptr;
}

std::shared_ptr<EyeBugEntity> buildEyeBug(std::shared_ptr<ResourceAllocator> allocator, const float &x, const float &y) {
    auto eye_ptr = std::make_shared<EyeBugEntity>();

    if (auto pos_ptr = eye_ptr->getComponent<PositionComponent>()) {
        pos_ptr->x = x;
        pos_ptr->y = y;
    }
    if (auto spr_ptr = eye_ptr->getComponent<SpriteComponent>()) {
        spr_ptr->sprite.setTexture(*allocator->loadTexture("../assets/textures/characters/eyebug.png"));
        spr_ptr->sprite.setTextureRect(sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(32, 32)));
        spr_ptr->sprite.setOrigin(sf::Vector2f(32/2, 32/2));
    }
    if (auto hlt_ptr = eye_ptr->getComponent<HealthComponent>()) {
        hlt_ptr->currentHealth = 30;
        hlt_ptr->maxHealth = 40;
    }
    if (auto ani_ptr = eye_ptr->getComponent<AnimationComponent>()) {
        ani_ptr->animationIndex = 0;
        ani_ptr->elapsedTime = 0.0f;
        ani_ptr->frameTime = 0.1;
        ani_ptr->animationIndexMax = 96;
        
    }
    if (auto col_ptr = eye_ptr->getComponent<CollisionComponent>()) {
        col_ptr->bounds = eye_ptr->getComponent<SpriteComponent>()->sprite.getGlobalBounds();
    }
    if (auto uid_ptr = eye_ptr->getComponent<UUIDComponent>()) {
        uid_ptr->ID = reinterpret_cast<uint64_t>(&eye_ptr);
    }
    if (auto vel_ptr = eye_ptr->getComponent<VelocityComponent>()) {
        vel_ptr->xSpeed = 17.0f;
        vel_ptr->ySpeed = 17.0f;
        vel_ptr->moveAmount = 2.0f;
    }
    return eye_ptr;
}

std::shared_ptr<TreeEntity>   buildTree(std::shared_ptr<ResourceAllocator> allocator, const float &x, const float &y) {
    auto tre_ptr = std::make_shared<TreeEntity>();

    if (auto pos_ptr = tre_ptr->getComponent<PositionComponent>()) {
        pos_ptr->x = x;
        pos_ptr->y = y;
    }

    if (auto spr_ptr = tre_ptr->getComponent<SpriteComponent>()) {
        spr_ptr->sprite.setTexture(*allocator->loadTexture("../assets/textures/tilesheets/TX Plant.png"));
        spr_ptr->sprite.setTextureRect(sf::IntRect(0,0, 128+32, 128+32));
        spr_ptr->sprite.setPosition(tre_ptr->getComponent<PositionComponent>()->x, tre_ptr->getComponent<PositionComponent>()->y);
    }
    if (auto hlt_ptr = tre_ptr->getComponent<HealthComponent>()) {
        hlt_ptr->currentHealth = 160;
        hlt_ptr->maxHealth = 180;
    }
    if (auto col_ptr = tre_ptr->getComponent<StaticCollisionComponent>()) {
        col_ptr->bounds = tre_ptr->getComponent<SpriteComponent>()->sprite.getGlobalBounds();
    }
    if (auto uid_ptr = tre_ptr->getComponent<UUIDComponent>()) {
        uid_ptr->ID = reinterpret_cast<uint64_t>(&tre_ptr);
    }
    return tre_ptr;
}

std::shared_ptr<AxeChopEntity> buildAxeChop(std::shared_ptr<ResourceAllocator> allocator, std::shared_ptr<Entity> ownerEntity, const float &x, const float &y) {
    auto axe_ptr = std::make_shared<AxeChopEntity>(ownerEntity);

    //  Animation
    if (auto anime_ptr = axe_ptr->getComponent<AnimationComponent>()) {
        anime_ptr->animationIndex = 0;
        anime_ptr->elapsedTime = 0.0f;
        anime_ptr->animationIndexMax = 10;
        anime_ptr->frameTime = 0.07f;
    }
    if (auto spr_ptr = axe_ptr->getComponent<SpriteComponent>()) {
        spr_ptr->sprite.setTexture(*allocator->loadTexture("../assets/textures/animations/axe_chop.png"));
        spr_ptr->sprite.setTextureRect(sf::IntRect(0,0, 64, 64));
        spr_ptr->sprite.setOrigin(sf::Vector2f(spr_ptr->sprite.getTextureRect().width/2, spr_ptr->sprite.getTextureRect().height/2));
    }
    if (auto uid_ptr = axe_ptr->getComponent<UUIDComponent>()) {
        uid_ptr->ID = reinterpret_cast<uint64_t>(&axe_ptr);
    }

    return axe_ptr;
}
//  Blue pointer
std::shared_ptr<BluePointerEntity> buildBluePointer(std::shared_ptr<ResourceAllocator> allocator, const float &x, const float &y) {
    auto ptr_ptr = std::make_shared<BluePointerEntity>(); // lol "ptr_ptr"

    if (auto spr_ptr = ptr_ptr->getComponent<SpriteComponent>()) {
        spr_ptr->sprite.setTexture(*allocator->loadTexture("../assets/textures/UI/blue_pointer.png"));
        spr_ptr->sprite.setTextureRect(sf::IntRect(0,0, 32, 32));
        spr_ptr->sprite.setOrigin(sf::Vector2f(32/2, 32/2));
        spr_ptr->sprite.setPosition(x, y-20);
    }
    if (auto del_ptr = ptr_ptr->getComponent<DeletableComponent>()) {
        del_ptr->markedForDeletion = false;
    }
    if (auto lft_ptr = ptr_ptr->getComponent<LifetimeComponent>()) {
        lft_ptr->timeAlive = 0.0f;
        lft_ptr->deathTime = 2.0f;
    }
    if ( auto pos_ptr = ptr_ptr->getComponent<PositionComponent>()) {
        pos_ptr->x = ptr_ptr->getComponent<SpriteComponent>()->sprite.getPosition().x;
        pos_ptr->y = ptr_ptr->getComponent<SpriteComponent>()->sprite.getPosition().y;
    }
    if (auto uid_ptr = ptr_ptr->getComponent<UUIDComponent>()) {
        uid_ptr->ID = reinterpret_cast<uint64_t>(&(*ptr_ptr));
    }
    if (auto ani_ptr = ptr_ptr->getComponent<AnimationComponent>()) {
        ani_ptr->animationIndex = 0;
        ani_ptr->animationIndexMax = 15;
        ani_ptr->elapsedTime = 0.0f;
        ani_ptr->frameTime = 0.03655;
    }
    return ptr_ptr;

}

std::shared_ptr<MossyWellEntity> buildWell(std::shared_ptr<ResourceAllocator> allocator, const float &x, const float &y) {
    auto well_ptr = std::make_shared<MossyWellEntity>();
    if (auto pos_ptr = well_ptr->getComponent<PositionComponent>()) {
        pos_ptr->x = x;
        pos_ptr->y = y;
    }
    if (auto spr_ptr = well_ptr->getComponent<SpriteComponent>()) {
        spr_ptr->sprite.setTexture(*allocator->loadTexture("../assets/textures/objects/mossy_well.png"));
        spr_ptr->sprite.setTextureRect(sf::IntRect(0,0,64,64));
        spr_ptr->sprite.setOrigin(spr_ptr->sprite.getTexture()->getSize().x/2, spr_ptr->sprite.getTexture()->getSize().y/2);
        spr_ptr->sprite.setPosition(well_ptr->getComponent<PositionComponent>()->x, well_ptr->getComponent<PositionComponent>()->y);
    }
    if (auto col_ptr = well_ptr->getComponent<StaticCollisionComponent>()) {
        col_ptr->bounds = well_ptr->getComponent<SpriteComponent>()->sprite.getGlobalBounds();
    }
    if (auto uid_ptr = well_ptr->getComponent<UUIDComponent>()) {
        uid_ptr->ID = reinterpret_cast<uint64_t>(&(*well_ptr));
    }
    return well_ptr;
}

std::shared_ptr<InventoryEntity> buildInventory(std::shared_ptr<ResourceAllocator> allocator, const float &x, const float &y) {
    auto inv_ptr = std::make_shared<InventoryEntity>();

    if (auto pos_ptr = inv_ptr->getComponent<PositionComponent>()) {
        pos_ptr->x = x;
        pos_ptr->y = y;
    }
    if (auto spr_ptr = inv_ptr->getComponent<SpriteComponent>()) {
        spr_ptr->sprite.setTexture(*allocator->loadTexture("../assets/textures/UI/inventory_background.png"));
        spr_ptr->sprite.setTextureRect(sf::IntRect(0,0, spr_ptr->sprite.getTexture()->getSize().x, spr_ptr->sprite.getTexture()->getSize().y));
        spr_ptr->sprite.setPosition(inv_ptr->getComponent<PositionComponent>()->x, inv_ptr->getComponent<PositionComponent>()->y);
        spr_ptr->isVisible = true;
    }
    //  Velocity
    if (auto velo_ptr = inv_ptr->getComponent<VelocityComponent>()) {
        velo_ptr->xSpeed = 17.0f;
        velo_ptr->ySpeed = 17.0f;
        velo_ptr->moveAmount = 10.0f;
    }
    if (auto uid_ptr = inv_ptr->getComponent<UUIDComponent>()) {
        uid_ptr->ID = reinterpret_cast<uint64_t>(&(*inv_ptr));
    }
    return inv_ptr;
}

std::shared_ptr<IconEntity> buildIcon(std::shared_ptr<ResourceAllocator> allocator, std::string texture_name, const float &x, const float &y) {
    auto ico_ptr = std::make_shared<IconEntity>();

    if (auto pos_ptr = ico_ptr->getComponent<PositionComponent>()) {
        pos_ptr->x = x;
        pos_ptr->y = y;
    }
    if (auto spr_ptr = ico_ptr->getComponent<SpriteComponent>()) {
        spr_ptr->isVisible = true;
        spr_ptr->sprite.setTexture(*allocator->loadTexture("../assets/textures/UI/icon/" + texture_name));
        spr_ptr->sprite.setScale(0.8, 0.8);
        spr_ptr->sprite.setPosition(ico_ptr->getComponent<PositionComponent>()->x, ico_ptr->getComponent<PositionComponent>()->y);
    }

    if (auto uid_ptr = ico_ptr->getComponent<UUIDComponent>()) {
        uid_ptr->ID = reinterpret_cast<uint64_t>(&(*ico_ptr));
    }
    return ico_ptr;
}

std::shared_ptr<WeaponEntity> buildWeapon(std::shared_ptr<ResourceAllocator> allocator, std::string texture_name, const float &x, const float &y) {
    auto wea_ptr = std::make_shared<WeaponEntity>();

    if (auto pos_ptr = wea_ptr->getComponent<PositionComponent>()) {
        pos_ptr->x = x;
        pos_ptr->y = y;
    }
    if (auto spr_ptr = wea_ptr->getComponent<SpriteComponent>()) {
        spr_ptr->isVisible = false;
        spr_ptr->sprite.setTexture(*allocator->loadTexture("../assets/textures/weapons/" + texture_name));
        spr_ptr->sprite.setPosition(x, y);
        spr_ptr->sprite.setScale(0.7,0.7);
        spr_ptr->sprite.setOrigin(spr_ptr->sprite.getTexture()->getSize().x/2, spr_ptr->sprite.getTexture()->getSize().x/4 * 5);
    }
    // if (auto col_ptr = wea_ptr->getComponent<CollisionComponent>()) {
    //     col_ptr->bounds = wea_ptr->getComponent<SpriteComponent>()->sprite.getGlobalBounds();
    // }
    if (auto uid_ptr = wea_ptr->getComponent<UUIDComponent>()) {
        uid_ptr->ID = reinterpret_cast<uint64_t>(&(*wea_ptr));
    }

    return wea_ptr;
}