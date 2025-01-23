#ifndef _RENDER_SYSTEM_
#define _RENDER_SYSTEM_

#include <memory>

#include <SFML/Graphics.hpp>

#include "ECS/Entities/Entity.hpp"
#include "ECS/Components/Components.hpp"


#define DRAW_HITBOX true  // only draws box if entity has CollisionComponent or StaticCollisionComponent

class RenderSystem {
public:
    inline void update(std::unique_ptr<sf::RenderWindow>& ren, std::vector<std::weak_ptr<Entity>> entities) {
        for (auto& entity_p : entities) {
            if (auto entity = entity_p.lock()) {
                if (entity->hasComponent<SpriteComponent>()) {
                    if (auto spriteCompPtr = entity->getComponent<SpriteComponent>()) {
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
                            ren->draw(spriteCompPtr->sprite);  
                           
                    }
                }
            }
        }
    }
};

#endif