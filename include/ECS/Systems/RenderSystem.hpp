#ifndef _RENDER_SYSTEM_
#define _RENDER_SYSTEM_

#include <memory>

#include <SFML/Graphics.hpp>

#include "ECS/Entities/Entity.hpp"
#include "ECS/Entities/Human.hpp"
#include "ECS/Components/Components.hpp"

void drawSelectedBox(sf::RenderWindow &ren, float x, float y, int width, int height) {
    sf::RectangleShape hitbox;
    hitbox.setPosition(x, y);
    sf::Vector2f hbsize;
    hbsize.x = width;
    hbsize.y = height;
    hitbox.setSize(hbsize);

    hitbox.setOutlineColor(sf::Color::Black);
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineThickness(1.5);

    ren.draw(hitbox);
}

class RenderSystem {
    public:
    void update(sf::RenderWindow& ren, std::vector<std::unique_ptr<Entity>>& entities) {
        for (auto& entity : entities) {
            if (entity->hasComponent<SpriteComponent>()) {
                // Use getComponent and dereference the shared_ptr to access the underlying component
                auto spriteCompPtr = entity->getComponent<SpriteComponent>();
                if (spriteCompPtr) {
                    ren.draw(spriteCompPtr->sprite);

                    // draw box around entity (if selected)
                    if (entity->hasComponent<SelectableComponent>() && entity->getComponent<SelectableComponent>()->isSelected) {
                        drawSelectedBox(
                        ren, 
                        spriteCompPtr->sprite.getGlobalBounds().left, 
                        spriteCompPtr->sprite.getGlobalBounds().top,
                        spriteCompPtr->sprite.getTextureRect().width,
                        spriteCompPtr->sprite.getTextureRect().height
                        );
                    }    
                }
            }
        }
    }
};

#endif