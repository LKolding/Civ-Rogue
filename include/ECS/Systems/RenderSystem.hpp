#ifndef _RENDER_SYSTEM_
#define _RENDER_SYSTEM_

#include <memory>

#include <SFML/Graphics.hpp>

#include "ECS/Systems/System.hpp"
#include "ECS/Entities/Entity.hpp"
#include "ECS/Components/Components.hpp"


#define DRAW_HITBOX false  // only draws box if entity has CollisionComponent or StaticCollisionComponent

void inline drawSelectedBox(sf::RenderWindow &ren, float x, float y, int width, int height, sf::Color color = sf::Color::Black) {
    sf::RectangleShape hitbox;
    hitbox.setPosition(x, y);
    sf::Vector2f hbsize;
    hbsize.x = width;
    hbsize.y = height;
    hitbox.setSize(hbsize);

    hitbox.setOutlineColor(color);
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineThickness(1.5);
    //hitbox.scale(1.2f,1.2f);

    ren.draw(hitbox);
}

class RenderSystem {
public:
    inline void update(sf::RenderWindow& ren, std::vector<std::weak_ptr<Entity>> entities) {
        for (auto& entity_p : entities) {
            if (auto entity = entity_p.lock()) {
                if (entity->hasComponent<SpriteComponent>()) {
                    if (auto spriteCompPtr = entity->getComponent<SpriteComponent>()) {
                        // handle flip behavior
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
                        if (entity->getComponent<SpriteComponent>()->isVisible)
                            ren.draw(spriteCompPtr->sprite);

                        // draw box around entity (if DRAW_HITBOX)
                        if ( DRAW_HITBOX && entity->hasComponent<CollisionComponent>()) {
                            drawSelectedBox(
                            ren, 
                            entity->getComponent<CollisionComponent>()->bounds.left, 
                            entity->getComponent<CollisionComponent>()->bounds.top,
                            entity->getComponent<CollisionComponent>()->bounds.width,
                            entity->getComponent<CollisionComponent>()->bounds.height
                            );
                        }    
                        // draw box around entity (if DRAW_HITBOX)
                        if ( DRAW_HITBOX && entity->hasComponent<StaticCollisionComponent>()) {
                            drawSelectedBox(
                            ren, 
                            entity->getComponent<StaticCollisionComponent>()->bounds.left, 
                            entity->getComponent<StaticCollisionComponent>()->bounds.top,
                            entity->getComponent<StaticCollisionComponent>()->bounds.width,
                            entity->getComponent<StaticCollisionComponent>()->bounds.height
                            );
                        }   

                        // draw box around entity (if selected)
                        if (entity->hasComponent<SelectableComponent>() && entity->getComponent<SelectableComponent>()->isSelected) {
                            drawSelectedBox(
                            ren, 
                            spriteCompPtr->sprite.getGlobalBounds().left, 
                            spriteCompPtr->sprite.getGlobalBounds().top,
                            spriteCompPtr->sprite.getTextureRect().width,
                            spriteCompPtr->sprite.getTextureRect().height,
                            sf::Color::Green
                            );
                        }    
                    }
                }
            }
        }
    }
};

#endif