#ifndef _IMGUI_SYSTEM_
#define _IMGUI_SYSTEM_

#include <string>

#include "imgui.h"

#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"
#include "EntityFactory.hpp"

class ImguiSystem {
public:
    void update(EntityManager& em, ComponentManager& cm);
};

#endif