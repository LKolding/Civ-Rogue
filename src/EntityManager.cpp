#include "ECS/EntityManager.hpp"


EntityID EntityManager::createEntity() {
    if (!freeIDs.empty()) {
        EntityID id = freeIDs.back();
        freeIDs.pop_back();
        return id;
    }
    EntityID id = nextID++;
    aliveEntities.push_back(id);
    return id;
}


void EntityManager::destroyEntity(EntityID id) {
    freeIDs.push_back(id);
    
}


std::vector<EntityID> EntityManager::getAllEntities() {
    std::vector<EntityID> entities;
    for (auto& entity : this->aliveEntities) {
        entities.push_back(entity);
    }
    return entities;
}

/*
void EntityManager::renderArrow(sf::Vector2f mouse) {
    this->addEntity(buildBluePointer(this->allocator_p, mouse.x, mouse.y));
}*/