#ifndef _RENDER_SYSTEM_
#define _RENDER_SYSTEM_

#include <memory>

#include <SFML/Graphics.hpp>

#include "ECS/Systems/System.hpp"
#include "ECS/Entities/Entity.hpp"
#include "ECS/Entities/Human.hpp"
#include "ECS/Components/Components.hpp"

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

    ren.draw(hitbox);
}

#define DRAW_HITBOX false

class RenderSystem {
public:
    inline void update(sf::RenderWindow& ren, std::vector<std::weak_ptr<Entity>> entities) {
        for (auto& entity_p : entities) {
            if (auto entity = entity_p.lock()) {
                if (entity->hasComponent<SpriteComponent>()) {
                    // Use getComponent and dereference the shared_ptr to access the underlying component
                    auto spriteCompPtr = entity->getComponent<SpriteComponent>();
                    if (spriteCompPtr) {
                        if (entity->hasComponent<PositionComponent>()) { // update sprite position
                            spriteCompPtr->sprite.setPosition(entity->getComponent<PositionComponent>()->x, entity->getComponent<PositionComponent>()->y);
                        }

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

                        ren.draw(spriteCompPtr->sprite);

                        // draw box around entity (if DRAW_HITBOX)
                        if ( DRAW_HITBOX) {
                            drawSelectedBox(
                            ren, 
                            spriteCompPtr->sprite.getGlobalBounds().left, 
                            spriteCompPtr->sprite.getGlobalBounds().top,
                            spriteCompPtr->sprite.getTextureRect().width,
                            spriteCompPtr->sprite.getTextureRect().height
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