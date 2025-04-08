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


class CursorSystem {
public:
    inline void update(std::unique_ptr<sf::RenderWindow>& ren, EntityManager& em, ComponentManager& cm) {
        auto mouse_pos = sf::Mouse::getPosition(*ren);

        for (auto& ent : em.getAllEntities()) {
            if (cm.getComponent<HoverComponent>(ent)) {
                // hover logic
            }
        }

    }
};

#endif