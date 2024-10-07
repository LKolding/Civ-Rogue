#include "EntityManager.hpp"


void EntityManager::setAllocator(std::shared_ptr<ResourceAllocator> allocator) {
        this->allocator_p = allocator;
        // TODO TEMP
        this->entities.push_back(buildNinja(allocator_p));

}


template <typename T>
void EntityManager::createEntity(T entityClass) {
    if (std::is_same<T, NinjaEntity>::value) {
        this->entities.push_back(buildNinja(this->allocator_p));
    }

}


std::vector<std::weak_ptr<Entity>> EntityManager::getAllEntities() {
    std::vector<std::weak_ptr<Entity>> entities_vector;

    for (auto &entity : this->entities) {
        entities_vector.push_back(std::weak_ptr<Entity>(entity));
    }

    return entities_vector;
}