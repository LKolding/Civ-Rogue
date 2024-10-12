#include "EntityManager.hpp"


void EntityManager::setAllocator(std::shared_ptr<ResourceAllocator> allocator) {
        this->allocator_p = allocator;
}

void EntityManager::addEntity(std::shared_ptr<Entity> entity) {

    this->mappedEntities[entity->getComponent<UUIDComponent>()->ID] = entity;
    this->entities.push_back(std::move(entity));
    
    if (this->entities.back()->hasComponent<HealthComponent>()) { // if healthcompenent, create healthbar
        auto htl_ptr = buildHealthbar(allocator_p, this->entities.back());
        this->entities.push_back(std::move(htl_ptr));
        this->mappedEntities[this->entities.back()->getComponent<UUIDComponent>()->ID] = this->entities.back();

    }

}

void EntityManager::killEntity(uint64_t ID) {
    if (!this->mappedEntities[ID].lock())
        return;

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

void EntityManager::createBorderEntities(std::shared_ptr<ResourceAllocator> allocator, sf::Vector2i pos) {
    auto topBorder1 = buildBorder(allocator, pos.x, pos.y+10);
    topBorder1->getComponent<SpriteComponent>()->sprite.scale(1.0f, -1.0);
    auto topBorder2 = buildBorder(allocator, 256, pos.y+10);
    topBorder2->getComponent<SpriteComponent>()->sprite.scale(1.0f, -1.0);

    auto botBorder1 = buildBorder(allocator, pos.x, pos.y+15*32);
    auto botBorder2 = buildBorder(allocator, 256, pos.y+15*32);

    this->entities.push_back(std::move(topBorder1));
    this->entities.push_back(std::move(topBorder2));
    this->entities.push_back(std::move(botBorder1));
    this->entities.push_back(std::move(botBorder2));
    
    auto lefBorder1 = buildBorder(allocator, pos.x, pos.y);
    lefBorder1->getComponent<SpriteComponent>()->sprite.setRotation(90);  
    auto lefBorder2 = buildBorder(allocator, pos.x, pos.y+240);
    lefBorder2->getComponent<SpriteComponent>()->sprite.setRotation(90);  

    this->entities.push_back(std::move(lefBorder1));
    this->entities.push_back(std::move(lefBorder2));
}

void EntityManager::update(float deltaTime) {
    std::vector<uint64_t> entitiesToBeDeleted;

    //  Add entities (if marked) to deletion tracker vector 
    for (auto& ent : this->entities) {
        if (!ent) { continue; }

        if (ent->hasComponent<DeletableComponent>()) {
            if (ent->getComponent<DeletableComponent>()->markedForDeletion) {
                entitiesToBeDeleted.push_back(ent->getComponent<UUIDComponent>()->ID);
            }
        } 
        ent->update(deltaTime);
        
    }

    if (entitiesToBeDeleted.size() > 0) {
        for (int i = 0; i < entitiesToBeDeleted.size(); ++i) {
            this->killEntity(entitiesToBeDeleted[i]);
        }
        entitiesToBeDeleted.clear();
    }
}

std::vector<std::weak_ptr<Entity>> EntityManager::getAllEntities() {
    std::vector<std::weak_ptr<Entity>> entities_vector;

    for (auto &entity : this->entities) {
        entities_vector.push_back(std::weak_ptr<Entity>(entity));
    }

    return entities_vector;
}

//  The parameter ID is the same as UUIDComponent ID member
// and it meant to be used in junction with said value
std::weak_ptr<Entity> EntityManager::getEntity(uint64_t ID) {
    return this->mappedEntities[ID];
}