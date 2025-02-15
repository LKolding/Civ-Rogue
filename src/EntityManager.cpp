#include "EntityManager.hpp"


void EntityManager::setAllocator(std::shared_ptr<ResourceAllocator> allocator) {
        this->allocator_p = allocator;
}

void EntityManager::addEntity(std::shared_ptr<Entity> entity) {
    this->mappedEntities[entity->getComponent<UUIDComponent>()->ID] = entity;
    this->entities.push_back(std::move(entity));  // final dst for shared ptr
    
    // temp weak ptr to work with entity
    std::weak_ptr<Entity> entP = this->entities.back();
    if (auto ent = entP.lock()) {
        sf::Vector2f entityPosition;
        entityPosition.x = ent->getComponent<PositionComponent>()->x;
        entityPosition.y = ent->getComponent<PositionComponent>()->y;

        if (ent->hasComponent<HealthComponent>()) { // if healthcompenent, create healthbar
            auto htl_ptr = buildHealthbar(allocator_p, this->entities.back());
            this->entities.push_back(std::move(htl_ptr));
            // also store health bar in mappedEntities 
            this->mappedEntities[this->entities.back()->getComponent<UUIDComponent>()->ID] = this->entities.back();

        }
    }
}

void EntityManager::killEntity(uint64_t ID) {
    if (!this->mappedEntities[ID].lock()) {
        std::cout << "Something is wrong. An Entity with UID " << std::to_string(ID) << " was marked for destruction, yet no entry for it exists in mapped_entities. Leaving entity alive...\n";
        return;
    }

    for (size_t i = 0; i < this->entities.size(); ++i) {
        auto& ent = this->entities[i];
        
        if (!ent)
            continue;

        if (!ent->hasComponent<UUIDComponent>())
            continue;

        if (ent->getComponent<UUIDComponent>()->ID == ID) {
            ent.reset(); // Reset the entity
            this->mappedEntities[ID].reset(); // Reset the mapped entity
            this->entities.erase(this->entities.begin() + i); // Erase the entity by index
            return;
        }
    }
    
}

void EntityManager::renderArrow(sf::Vector2f mouse) {
    this->addEntity(buildBluePointer(this->allocator_p, mouse.x, mouse.y));
}

// Updates all entities
void EntityManager::update(float deltaTime) {
    for (auto &ent : this->entities) {
        ent->update(deltaTime);
    }
}

std::vector<std::weak_ptr<Entity>> EntityManager::getAllEntities() {
    std::vector<std::weak_ptr<Entity>> entities_vector;

    for (auto &entity : this->entities) {
        entities_vector.push_back(std::weak_ptr<Entity>(entity));
    }

    return entities_vector;
}
