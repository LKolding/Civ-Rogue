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
#include "ECS/Components/Components.hpp"


std::shared_ptr<HumanMageEntity> buildHumanMage(ResourceAllocator &allocator, const float &x = 100, const float &y = 100);

// Ninja
std::shared_ptr<NinjaEntity> buildNinja(std::shared_ptr<ResourceAllocator> allocator, const float &x = 100 ,const float &y = 100);

// Healthbar
std::shared_ptr<HealthbarEntity> buildHealthbar(std::shared_ptr<ResourceAllocator> allocator, std::shared_ptr<Entity> ownerEntity, const float &x = 100, const float &y = 100);
#endif