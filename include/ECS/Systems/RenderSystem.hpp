#ifndef _RENDER_SYSTEM_
#define _RENDER_SYSTEM_

#include <memory>

#include <SFML/Graphics.hpp>

#include "ECS/Entities/Entity.hpp"
#include "ECS/Entities/Human.hpp"
#include "ECS/Components/Components.hpp"

class RenderSystem {
    public:
    void update(sf::RenderWindow& ren, std::vector<std::unique_ptr<Entity>>& entities) {
        for (auto& entity : entities) {
            if (entity->hasComponent<SpriteComponent>()) {
                // Use getComponent and dereference the shared_ptr to access the underlying component
                auto spriteCompPtr = entity->getComponent<SpriteComponent>();
                if (spriteCompPtr) {
                    ren.draw(spriteCompPtr->sprite);
                }
            }
        }
    }
};

#endif