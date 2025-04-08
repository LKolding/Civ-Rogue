#ifndef _ENTITY_FACTORY_
#define _ENTITY_FACTORY_
// system
#include <memory>
// sfml
#include <SFML/Graphics.hpp>
// my stuff
#include "ResourceAllocator.hpp"

#include "ECS/Components/Components.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"

#include "constants.h"

// Ninja
//std::shared_ptr<NinjaEntity> buildNinja(std::shared_ptr<ResourceAllocator> allocator, const float &x = CHUNK_WIDTH*TILE_WIDTH/2 ,const float &y = CHUNK_HEIGHT*TILE_HEIGHT/2);

EntityID buildNinja(ComponentManager& cm, EntityManager& em);

/*
// Healthbar
std::shared_ptr<HealthbarEntity> buildHealthbar(std::shared_ptr<ResourceAllocator> allocator, std::shared_ptr<Entity> ownerEntity, const float &x = 100, const float &y = 100);

std::shared_ptr<EyeBugEntity> buildEyeBug(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100, const float &y = 100);

std::shared_ptr<TreeEntity> buildTree(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100, const float &y = 100);

std::shared_ptr<BluePointerEntity> buildBluePointer(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100, const float &y = 100);

std::shared_ptr<MossyWellEntity> buildWell(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100, const float &y = 100);

std::shared_ptr<WeaponEntity> buildWeapon(std::shared_ptr<ResourceAllocator> allocator, std::string texture_name, const float &x = 100, const float &y = 100);
*/
#endif