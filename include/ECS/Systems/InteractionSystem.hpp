#ifndef _INTERACTION_SYSTEM_
#define _INTERACTION_SYSTEM_

#include "SFML/Graphics.hpp"

#include "ECS/Components/Components.hpp"
#include "ECS/Entities/Entity.hpp"

#include "input.hpp"
#include "game_functions.hpp"  // isEntityHovered()
#include "Player.hpp"

#include <iostream>

class InteractionSystem {
public:
    void update(
        std::vector<std::weak_ptr<Entity>> entities, 
        std::unique_ptr<InputManager>& inputManager, 
        std::unique_ptr<sf::RenderWindow>& ren,
        std::shared_ptr<Player>& player)
        {
        return;
    }
};

#endif