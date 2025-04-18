#ifndef _IMGUI_SYSTEM_
#define _IMGUI_SYSTEM_

#include <string>

#include <SFML/Graphics.hpp>

#include "imgui.h"

#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"
#include "EntityFactory.hpp"

// Unique system that performs logic regarding the minimap and renders it afterwards. This means that this system should
// be called in the rendering stage rather than in conjunction with the rest of the systems in the game.
class ImguiSystem {
public:
    void update(const sf::RenderWindow& ren, EntityManager& em, ComponentManager& cm);
};

#endif