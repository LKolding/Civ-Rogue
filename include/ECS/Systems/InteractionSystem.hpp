#ifndef _INTERACTION_SYSTEM_
#define _INTERACTION_SYSTEM_

#include "ECS/Components/Components.hpp"
#include "ECS/Entities/Entity.hpp"

#include <iostream>

class InteractionSystem {
public:
    void update(std::vector<std::weak_ptr<Entity>> entities, sf::Vector2i mouse_pos) {
        std::cout << mouse_pos.x << ", " << mouse_pos.x << "\n";
    }
};

#endif