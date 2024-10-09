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
#include "ECS/Entities/Healthbar.hpp"
#include "ECS/Entities/Border.hpp"
#include "ECS/Entities/Button.hpp"
#include "ECS/Entities/Eyebug.hpp"
#include "ECS/Entities/Tree.hpp"
#include "ECS/Entities/Arrow.hpp"
#include "ECS/Entities/BluePointer.hpp"

#include "ECS/Entities/Animations/AxeChop.hpp"

#include "ECS/Components/Components.hpp"


inline std::shared_ptr<HumanMageEntity> buildHumanMage(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100, const float &y = 100) {
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
        sprite_ptr->sprite.setTexture(*allocator->loadTexture("../assets/textures/characters/MiniMage.png"));
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
inline std::shared_ptr<NinjaEntity> buildNinja(std::shared_ptr<ResourceAllocator> allocator, const float &x = CHUNK_WIDTH*TILE_WIDTH/2 ,const float &y = CHUNK_HEIGHT*TILE_HEIGHT/2) {
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
        colli_ptr->bounds.width  -= 9;
        colli_ptr->bounds.height -= 9;
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
inline std::shared_ptr<HealthbarEntity> buildHealthbar(std::shared_ptr<ResourceAllocator> allocator, std::shared_ptr<Entity> ownerEntity, const float &x = 100, const float &y = 100) {
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
inline std::shared_ptr<BorderEntity> buildBorder(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100, const float &y = 100) {
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
inline std::shared_ptr<ButtonEntity> buildButton(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100, const float &y = 100) {
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

inline std::shared_ptr<EyeBugEntity> buildEyeBug(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100, const float &y = 100) {
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
        vel_ptr->moveAmount = 2.0f;
    }
    return eye_ptr;
}

inline std::shared_ptr<TreeEntity> buildTree(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100, const float &y = 100) {
    auto tre_ptr = std::make_shared<TreeEntity>();

    if (auto pos_ptr = tre_ptr->getComponent<PositionComponent>()) {
        pos_ptr->x = x;
        pos_ptr->y = y;
    }

    if (auto spr_ptr = tre_ptr->getComponent<SpriteComponent>()) {
        spr_ptr->sprite.setTexture(*allocator->loadTexture("../assets/textures/tilesheets/TX Plant.png"));
        spr_ptr->sprite.setTextureRect(sf::IntRect(0,0, 128+32, 128+32));
    }

    if (auto hlt_ptr = tre_ptr->getComponent<HealthComponent>()) {
        hlt_ptr->currentHealth = 100;
        hlt_ptr->maxHealth = 100;
    }
    
    if (auto uid_ptr = tre_ptr->getComponent<UUIDComponent>()) {
        uid_ptr->ID = reinterpret_cast<uint64_t>(&tre_ptr);
    }
    return tre_ptr;
}

inline std::shared_ptr<AxeChopEntity> buildAxeChop(std::shared_ptr<ResourceAllocator> allocator, std::shared_ptr<Entity> ownerEntity, const float &x = 100, const float &y = 100) {
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

inline std::shared_ptr<ArrowEntity> buildArrow(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100, const float &y = 100) {
    auto arw_ptr = std::make_shared<ArrowEntity>();

    if (auto spr_ptr = arw_ptr->getComponent<SpriteComponent>()) {
        spr_ptr->sprite.setTexture(*allocator->loadTexture("../assets/textures/UI/arrow.png"));
        spr_ptr->sprite.setTextureRect(sf::IntRect(0,0, spr_ptr->sprite.getTextureRect().width, spr_ptr->sprite.getTextureRect().height));

        spr_ptr->sprite.setOrigin(sf::Vector2f(spr_ptr->sprite.getTextureRect().width/2, spr_ptr->sprite.getTextureRect().height/2));
        spr_ptr->sprite.setRotation(90);
        spr_ptr->sprite.setScale(0.01f,0.01f);
        spr_ptr->sprite.setPosition(x,y);
    }

    if (auto uid_ptr = arw_ptr->getComponent<UUIDComponent>()) {
        uid_ptr->ID = reinterpret_cast<uint64_t>(&arw_ptr);
    }
    if (auto lft_ptr = arw_ptr->getComponent<LifetimeComponent>()) {
        lft_ptr->timeAlive = 0.0f;
        lft_ptr->deathTime = 1.5f;
    }
    if (auto del_ptr = arw_ptr->getComponent<DeletableComponent>()) {
        del_ptr->markedForDeletion = false;
    }
    return arw_ptr;
}
//  Blue pointer
inline std::shared_ptr<BluePointerEntity> buildBluePointer(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100, const float &y = 100) {
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

#endif