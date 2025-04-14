#ifndef _IMGUI_SYSTEM_
#define _IMGUI_SYSTEM_

#include <string>

#include <SFML/Graphics.hpp>

#include "imgui.h"

#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"
#include "EntityFactory.hpp"

class ImguiSystem {
public:
    void update(const sf::RenderWindow& ren, EntityManager& em, ComponentManager& cm);
};

#endif