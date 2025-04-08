#ifndef _GAME_FUNCS_
#define _GAME_FUNCS_

#include <SFML/Graphics.hpp>

#include <memory>

#include "coordinate_calculators.hpp"
#include "constants.h"

#include "Player.hpp"
#include "ECS/EntityManager.hpp"

#include "ECS/Components/Components.hpp"


// bool inline isEntityHovered(std::unique_ptr<sf::RenderWindow>& ren, std::weak_ptr<Entity> entity_p) {
//     if (auto entity = entity_p.lock()) {
//         sf::Vector2f worldCoords = ren->mapPixelToCoords(sf::Mouse::getPosition(*ren));
//         auto spr_ptr = entity->getComponent<SpriteComponent>();
//         // if (spr_ptr->sprite.getGlobalBounds().contains(worldCoords.x, worldCoords.y)) {
//         //     return true;
//         // }
//     }
//     return false;
// }

#endif