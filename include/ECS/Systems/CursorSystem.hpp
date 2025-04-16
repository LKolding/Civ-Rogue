#ifndef _CURSOR_SYSTEM_
#define _CURSOR_SYSTEM_
// system
#include <memory>
// sfml
#include <SFML/Graphics.hpp>
// game
#include "ECS/ComponentManager.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/Components/Components.hpp"

#include "game_functions.hpp"


class CursorSystem {
public:
    sf::Vector2i mouse_pos{0,0};  // Relative to screen
    sf::Vector2f mouse_pos_float{(float)this->mouse_pos.x, (float)this->mouse_pos.y};

    sf::Vector2f world_pos;  // Mouse position in world coordinates

    inline void update(sf::RenderWindow& ren, EntityManager& em, ComponentManager& cm) {
        auto new_pos = sf::Mouse::getPosition(ren);

        if (world_pos == ren.mapPixelToCoords(new_pos)) { // <- can I not just check against window coordinates here?
            return;  // skip if mouse hasn't moved
        } else {
            mouse_pos = new_pos;
            world_pos = ren.mapPixelToCoords(mouse_pos);
        }

        for (auto& ent : em.getAllEntities()) {  // iterate entities
            auto phover = cm.getComponent<HoverComponent>(ent); 
            if (!phover)
                continue;
            // -----------------------
            // ----- Hover logic -----
            // -----------------------
            // If being hovered
            if (isEntityHovered(cm, ent, this->world_pos)) {
                phover->isHovered = true;
            // If not, AND the hover component is set to true, switch it
            } else if (phover->isHovered) {
                phover->isHovered = false;  //switch
            }
        }

    }
};

#endif