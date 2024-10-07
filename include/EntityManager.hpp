#ifndef _ENTITY_MANAGER_
#define _ENTITY_MANAGER_

#include <memory>
#include <vector>
#include "ResourceAllocator.hpp"
#include "EntityFactory.hpp"


class EntityManager {
public:

    void setAllocator(std::shared_ptr<ResourceAllocator> allocator);
    // Creates entity and stores it
    template <typename T>
    void createEntity(T entityClass);
    // Returns a vector containing weak_ptr to all entities
    std::vector<std::weak_ptr<Entity>> getAllEntities();

private:
    std::shared_ptr<ResourceAllocator> allocator_p;
    std::vector<std::shared_ptr<Entity>> entities;
};

#endif