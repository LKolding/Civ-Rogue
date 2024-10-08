#ifndef _ENTITY_FACTORY_
#define _ENTITY_FACTORY_
// system
#include <memory>
// sfml
#include <SFML/Graphics.hpp>
// my stuff
#include "ResourceAllocator.hpp"
// ecs
#include "ECS/Entities/Entity.hpp"
#include "ECS/Entities/Human.hpp"
#include "ECS/Entities/Ninja.hpp"
#include "ECS/Entities/Healthbar.hpp"
#include "ECS/Entities/Border.hpp"
#include "ECS/Entities/Button.hpp"
#include "ECS/Entities/Eyebug.hpp"
#include "ECS/Components/Components.hpp"


std::shared_ptr<HumanMageEntity> buildHumanMage(ResourceAllocator &allocator, const float &x = 100, const float &y = 100);

// Ninja
std::shared_ptr<NinjaEntity> buildNinja(std::shared_ptr<ResourceAllocator> allocator, const float &x = CHUNK_WIDTH*TILE_WIDTH/2 ,const float &y = CHUNK_HEIGHT*TILE_HEIGHT/2);

// Healthbar
std::shared_ptr<HealthbarEntity> buildHealthbar(std::shared_ptr<ResourceAllocator> allocator, std::shared_ptr<Entity> ownerEntity, const float &x = 100, const float &y = 100);

// Border
std::shared_ptr<BorderEntity> buildBorder(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100, const float &y = 100);

// BUtton
std::shared_ptr<ButtonEntity> buildButton(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100, const float &y = 100);

std::shared_ptr<EyeBugEntity> buildEyeBug(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100, const float &y = 100);

#endif