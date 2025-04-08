#ifndef _ENTITY_MANAGER_
#define _ENTITY_MANAGER_

#include <memory>
#include <vector>
#include <typeinfo>
#include <typeindex>

#include "ResourceAllocator.hpp"

#include "constants.h" // EntityID


//  MANAGER CLASS
class EntityManager {
public:
    
    EntityID createEntity();
    void destroyEntity(EntityID id);

    std::vector<EntityID> getAllEntities();

    //void renderArrow(sf::Vector2f mouse);



private:
    EntityID nextID;
    std::vector<EntityID> freeIDs;
    std::vector<EntityID> aliveEntities;

};

#endif