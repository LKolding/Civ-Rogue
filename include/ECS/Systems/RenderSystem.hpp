#ifndef _RENDER_SYSTEM_
#define _RENDER_SYSTEM_

#include <memory>
#include <typeindex>

#include <SFML/Graphics.hpp>

#include "ECS/Components/Components.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"


#define DRAW_COLLISION_BOX false


class RenderSystem {
public:
    inline void update(std::unique_ptr<sf::RenderWindow>& renderWindow, std::shared_ptr<ResourceAllocator>& allocator, EntityManager& em, ComponentManager& cm) {
        for (auto& ent : em.getAllEntities()) {
            if ( cm.getComponent<PositionComponent>(ent) && cm.getComponent<SpriteComponent>(ent) ) {
                PositionComponent* cposition = cm.getComponent<PositionComponent>(ent);
                SpriteComponent* csprite = cm.getComponent<SpriteComponent>(ent);

                // Sprite to be drawn
                sf::Sprite sprite(*allocator->loadTexture(csprite->texturePath));
                sprite.setOrigin(csprite->origin); // Necessary ?
                sprite.setTextureRect(csprite->textureRectangle);
                
                // FLIP BEHAVIOR
                if ( cm.getComponent<FlipComponent>(ent)) {
                    FlipComponent* flip = cm.getComponent<FlipComponent>(ent);
                    // Flip
                    if (flip->isFlipped && !csprite->hasBeenFlipped) {
                        sprite.setScale(sf::Vector2f(-1.0f, 1.0f));
                        csprite->hasBeenFlipped = true;

                    }
                    // Un-flip
                    if (!flip->isFlipped && csprite->hasBeenFlipped) {
                        sprite.setScale(sf::Vector2f(1.0f, 1.0f));
                        csprite->hasBeenFlipped = false;

                    }
                }

                // Draw sprite
                if (csprite->isVisible) {
                    renderWindow->draw(sprite);
                }
                // Draw hitbox TODO

            }
        }
    }
    /*
    inline void update2(std::unique_ptr<sf::RenderWindow>& ren, std::vector<std::weak_ptr<Entity>> entities) {
        for (auto& entity_p : entities) {
            if (auto entity = entity_p.lock()) {
                //  Make sure the sprite position is
                //  PositionComponent of the entity
                if (auto posPtr = entity->getComponent<PositionComponent>()) {
                    entity->getComponent<SpriteComponent>()->sprite.setPosition(posPtr->x, posPtr->y);
                }
                //  Handle flip behavior
                if (entity->hasComponent<FlipComponent>()) {
                    // flip texture
                    if (entity->getComponent<FlipComponent>()->isFlipped && !entity->getComponent<SpriteComponent>()->hasBeenFlipped) {
                        entity->getComponent<SpriteComponent>()->sprite.setScale(sf::Vector2f(-1.0f, 1.0f));
                        entity->getComponent<SpriteComponent>()->hasBeenFlipped = true;
                    }
                    // unflip texture
                    if (!entity->getComponent<FlipComponent>()->isFlipped && entity->getComponent<SpriteComponent>()->hasBeenFlipped) {
                        entity->getComponent<SpriteComponent>()->sprite.setScale(sf::Vector2f(1.0f, 1.0f));
                        entity->getComponent<SpriteComponent>()->hasBeenFlipped = false;
                    }
                }
                //  Draw sprite
                if (entity->getComponent<SpriteComponent>()->isVisible)
                    ren->draw(entity->getComponent<SpriteComponent>()->sprite);

                if (entity->hasComponent<FollowComponent>() && entity->getComponent<FollowComponent>()->isFollowing) {
                    // get hitbox
                    sf::FloatRect floatRect = getBoundingBox(entity);
                    // create drawable shape
                    sf::RectangleShape hitbox = sf::RectangleShape(floatRect.getSize());
                    hitbox.setPosition(floatRect.left, floatRect.top);
                    // appearance
                    hitbox.setFillColor(sf::Color::Transparent);
                    hitbox.setOutlineColor(sf::Color::Green);
                    hitbox.setOutlineThickness(1.0f);
                    ren->draw(hitbox);
                }
                
                //  Draw hitbox
                if (DRAW_COLLISION_BOX && (entity->hasComponent<CollisionComponent>() || entity->hasComponent<StaticCollisionComponent>())) {
                    // get hitbox
                    sf::FloatRect floatRect = getBoundingBox(entity);
                    // create drawable shape
                    sf::RectangleShape hitbox = sf::RectangleShape(floatRect.getSize());
                    hitbox.setPosition(floatRect.left, floatRect.top);
                    // appearance
                    hitbox.setFillColor(sf::Color::Transparent);
                    hitbox.setOutlineColor(sf::Color::Black);
                    hitbox.setOutlineThickness(0.8f);
                    ren->draw(hitbox);

                }           
            }
        }
    }*/
};

#endif