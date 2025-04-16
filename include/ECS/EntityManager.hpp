#ifndef _ENTITY_MANAGER_
#define _ENTITY_MANAGER_

#include <memory>
#include <vector>

#include "ResourceManager.hpp"

#include "constants.h" // EntityID


//  MANAGER CLASS
class EntityManager {
public:
    EntityID createEntity();
    void destroyEntity(EntityID id);

    std::vector<EntityID> getAllEntities();

private:
    EntityID nextID;
    std::vector<EntityID> freeIDs;
    std::vector<EntityID> aliveEntities;

};

#endif