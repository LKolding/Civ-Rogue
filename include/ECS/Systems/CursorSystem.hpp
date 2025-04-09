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
    sf::Vector2i mouse_pos{0,0};  // relative to screen

    inline void update(sf::RenderWindow& ren, EntityManager& em, ComponentManager& cm) {
        auto new_pos = sf::Mouse::getPosition(ren);

        if (mouse_pos == new_pos) { 
            return;  // skip if mouse hasn't moved
        } else {
            mouse_pos = new_pos;
        }

        for (auto& ent : em.getAllEntities()) {
            if (cm.getComponent<HoverComponent>(ent)) {
                // hover logic
                if (isEntityHovered(ren, cm, ent)) {
                    cm.getComponent<HoverComponent>(ent)->isHovered = true;
                }

                auto floatRect = cm.getComponent<SpriteComponent>(ent)->textureRectangle;
                floatRect.position = {(int)cm.getComponent<PositionComponent>(ent)->x, (int)cm.getComponent<PositionComponent>(ent)->y};

                if (floatRect.contains(mouse_pos)) {
                    cm.getComponent<HoverComponent>(ent)->isHovered = true;
                }
            }
        }

    }
};

#endif