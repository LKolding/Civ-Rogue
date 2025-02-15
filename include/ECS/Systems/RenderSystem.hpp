#ifndef _RENDER_SYSTEM_
#define _RENDER_SYSTEM_

#include <memory>

#include <SFML/Graphics.hpp>

#include "ECS/Entities/Entity.hpp"
#include "ECS/Components/Components.hpp"


#define DRAW_COLLISION_BOX false

sf::FloatRect inline getBoundingBox(std::shared_ptr<Entity>& ent) {
    if (ent->hasComponent<CollisionComponent>()) {
        return ent->getComponent<CollisionComponent>()->bounds;
    } else {
        return ent->getComponent<StaticCollisionComponent>()->bounds;
    }
}

class RenderSystem {
public:
    inline void update(std::unique_ptr<sf::RenderWindow>& ren, std::vector<std::weak_ptr<Entity>> entities) {
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
    }
};

#endif