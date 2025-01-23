#ifndef _ENTITY_MANAGER_
#define _ENTITY_MANAGER_

#include <memory>
#include <vector>
#include "ResourceAllocator.hpp"
#include "EntityFactory.hpp"

#include "coordinate_calculators.hpp"
#include "constants.h"

class EntityManager {
public:

    void setAllocator(std::shared_ptr<ResourceAllocator> allocator);
    void addEntity(std::shared_ptr<Entity> entity);
    void killEntity(uint64_t ID);

    // pos = pos of chunk
    void createBorderEntities(std::shared_ptr<ResourceAllocator> allocator, sf::Vector2i pos); 

    void renderArrow(sf::Vector2f mouse);
    void update(float deltaTime);

    // Returns a vector containing weak_ptr to _all_ entities
    std::vector<std::weak_ptr<Entity>> getAllEntities();
    std::weak_ptr<Entity> getEntity(uint64_t ID);

    std::vector<std::weak_ptr<Entity>> getAllUIEntities();
    std::vector<std::weak_ptr<Entity>> getAllIconEntities();

private:
    std::shared_ptr<ResourceAllocator> allocator_p;
    // holds the "last" shared pointer to entities; if this dies, the entity should be considered dead.
    // Interacting with this vector is done through addEntity() and killEntity()
    std::vector<std::shared_ptr<Entity>> entities;
    // Entities mapped to their UUID for quick lookup. Maintained by addEntity() and killEntity()
    // This stores entries for ALL entities accross chunks and game worlds. This also works as a check to see if an entity is still alive.
    std::unordered_map<uint32_t, std::weak_ptr<Entity>> mappedEntities; 
};

#endif