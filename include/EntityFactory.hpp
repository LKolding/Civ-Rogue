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
#include "ECS/Entities/Ninja.hpp"
#include "ECS/Entities/Healthbar.hpp"
#include "ECS/Entities/Border.hpp"
#include "ECS/Entities/Button.hpp"
#include "ECS/Entities/Eyebug.hpp"
#include "ECS/Entities/Tree.hpp"
#include "ECS/Entities/BluePointer.hpp"
#include "ECS/Entities/Well.hpp"
#include "ECS/Entities/UI/Inventory.hpp"
#include "ECS/Entities/UI/Icon.hpp"
#include "ECS/Entities/Weapon.hpp"

#include "ECS/Entities/Animations/AxeChop.hpp"

#include "ECS/Components/Components.hpp"

// Ninja
std::shared_ptr<NinjaEntity> buildNinja(std::shared_ptr<ResourceAllocator> allocator, const float &x = CHUNK_WIDTH*TILE_WIDTH/2 ,const float &y = CHUNK_HEIGHT*TILE_HEIGHT/2);
// Healthbar
std::shared_ptr<HealthbarEntity> buildHealthbar(std::shared_ptr<ResourceAllocator> allocator, std::shared_ptr<Entity> ownerEntity, const float &x = 100, const float &y = 100);
// Border
std::shared_ptr<BorderEntity> buildBorder(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100, const float &y = 100);
// BUtton
std::shared_ptr<ButtonEntity> buildButton(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100, const float &y = 100);

std::shared_ptr<EyeBugEntity> buildEyeBug(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100, const float &y = 100);

std::shared_ptr<TreeEntity> buildTree(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100, const float &y = 100);

std::shared_ptr<AxeChopEntity> buildAxeChop(std::shared_ptr<ResourceAllocator> allocator, std::shared_ptr<Entity> ownerEntity, const float &x = 100, const float &y = 100);

std::shared_ptr<BluePointerEntity> buildBluePointer(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100, const float &y = 100);

std::shared_ptr<MossyWellEntity> buildWell(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100, const float &y = 100);

std::shared_ptr<InventoryEntity> buildInventory(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100, const float &y = 100);

std::shared_ptr<IconEntity> buildIcon(std::shared_ptr<ResourceAllocator> allocator, std::string texture_name, const float &x = 100, const float &y = 100);

std::shared_ptr<WeaponEntity> buildWeapon(std::shared_ptr<ResourceAllocator> allocator, std::string texture_name, const float &x = 100, const float &y = 100);

#endif