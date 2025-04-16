#ifndef _GAME_FUNCS_
#define _GAME_FUNCS_

#include <SFML/Graphics.hpp>

#include "constants.h"

#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"

#include "ECS/Components/Components.hpp"


bool inline isEntityHovered(ComponentManager& cm, EntityID ent, sf::Vector2f mouse_pos) {
    auto bounds = cm.getComponent<BoundsComponent>(ent)->bounds;
    if (bounds.contains(mouse_pos))
        return true;
    else
        return false;
}

#endif