#ifndef _ENTITY_FACTORY_
#define _ENTITY_FACTORY_
// system
#include <memory>
// sfml
#include <SFML/Graphics.hpp>
// my stuff
#include "ResourceManager.hpp"

#include "ECS/Components/Components.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"

#include "constants.h"

// Ninja
//std::shared_ptr<NinjaEntity> buildNinja(std::shared_ptr<ResourceAllocator> allocator, const float &x = CHUNK_WIDTH*TILE_WIDTH/2 ,const float &y = CHUNK_HEIGHT*TILE_HEIGHT/2);

EntityID buildNinja(ComponentManager& cm, EntityManager& em, float x = 0, float y = 0);

#endif