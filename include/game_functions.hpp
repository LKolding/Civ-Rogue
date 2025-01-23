#ifndef _GAME_FUNCS_
#define _GAME_FUNCS_

#include <SFML/Graphics.hpp>

#include <memory>

#include "coordinate_calculators.hpp"
#include "constants.h"

#include "Player.hpp"
#include "EntityManager.hpp"

#include "ECS/Entities/Entity.hpp"
#include "ECS/Components/Components.hpp"


bool inline isEntityHovered(sf::RenderWindow &ren, std::weak_ptr<Entity> entity_p) {
    // Returns true if the entity's CollisionComponents boundingbox is currently
    // below sf::Mouse::getPosition()
    if (auto entity = entity_p.lock()) {
        sf::Vector2f worldCoords = ren.mapPixelToCoords(sf::Mouse::getPosition(ren));
        auto spr_ptr = entity->getComponent<SpriteComponent>();
        if (spr_ptr->sprite.getGlobalBounds().contains(worldCoords.x, worldCoords.y)) {
            return true;
        }
    }
    return false;
}

// checks if any entity is hovered that has
void inline followEntity(sf::RenderWindow &ren, std::shared_ptr<Player> player, std::vector<std::weak_ptr<Entity>> entities_p) {
    for (auto entity_p : entities_p) {
        if (auto entity = entity_p.lock()) {
            if (isEntityHovered(ren, entity)) {
                player->followUnit(entity);
                return;
            }
        }
    }
}


#endif