#ifndef _GAME_FUNCS_
#define _GAME_FUNCS_

#include <SFML/Graphics.hpp>

#include <memory>

#include "coordinate_calculators.hpp"
#include "constants.h"

#include "Player.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"

#include "ECS/Components/Components.hpp"


bool inline isEntityHovered(sf::RenderWindow& ren, ComponentManager& cm, EntityID ent) {
    if (ent) {
        sf::Vector2f mouse_pos = ren.mapPixelToCoords(sf::Mouse::getPosition(ren));
        auto bounds = cm.getComponent<BoundsComponent>(ent)->bounds;
        if (bounds.contains(mouse_pos)) {
            return true;
        }
    }
    return false;
}

#endif