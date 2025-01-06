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

        std::pair<int,int> chunkCoordinates;
        chunkCoordinates.first  = getChunkCoords(entityPosition).x;
        chunkCoordinates.second = getChunkCoords(entityPosition).y;
        // Add entity to the correct chunk entry in entitiesInChunk map
        this->entitiesInChunk[chunkCoordinates].push_back(ent);

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
                // add UUID of entity to list of entities marked for destruction
                entitiesToBeDeleted.push_back(ent->getComponent<UUIDComponent>()->ID); 
            }
        }
        ent->update(deltaTime);  // update entity
        
    }

    if (this->UIentities.size() > 0) {
        for (auto &ent : this->UIentities) {
            if (!ent.second)
                continue;
            if (!ent.second->hasComponent<SpriteComponent>())
                continue;
            
            ent.second->getComponent<SpriteComponent>()->isVisible = false;
        }
    }

    if (this->interactionIconEntities.size() > 0) {
        for (auto &ent : this->interactionIconEntities) {
            if (!ent.second)
                continue;
            if (!ent.second->hasComponent<SpriteComponent>())
                continue;
            
            ent.second->getComponent<SpriteComponent>()->isVisible = false;
        }
    }

    // Properly delete entities that are marked for destruction
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

std::vector<std::weak_ptr<Entity>> EntityManager::getAllEntitiesInChunk(std::pair<int,int> chunk_coords) {
    std::vector<std::weak_ptr<Entity>> entitiesToBeReturned;
    if (this->entitiesInChunk[chunk_coords].size() == 0) {
        return entitiesToBeReturned;
    }
    for (auto &entp : this->entitiesInChunk[chunk_coords]) {
        if (auto ent = entp.lock()) {
            entitiesToBeReturned.push_back(ent);
        }
    }
    return entitiesToBeReturned;
}

void EntityManager::showEntityInventory(uint64_t ID) {
    if (auto ent = this->mappedEntities[ID].lock()) {
        if (this->UIentities[ent->getComponent<UUIDComponent>()->ID]) {
            UIentities[ent->getComponent<UUIDComponent>()->ID]->getComponent<SpriteComponent>()->isVisible = true;
        } else {
            std::shared_ptr<Entity> p = buildInventory(this->allocator_p, WINDOW_WIDTH-400, WINDOW_HEIGHT-230);
            this->UIentities[p->getComponent<UUIDComponent>()->ID] = std::move(p);
        }
    }
}


void EntityManager::showIconEntity(std::string texture_name, sf::Vector2f mousePos) {
    mousePos.x += 10;
    mousePos.y -= 30;
    if (this->interactionIconEntities[texture_name]) {
        // display existing inventory entity (& update position)
        this->interactionIconEntities[texture_name]->getComponent<SpriteComponent>()->sprite.setPosition(mousePos);
        this->interactionIconEntities[texture_name]->getComponent<SpriteComponent>()->isVisible = true;
    } else {
        // automacially gets set to visible in factory function
        this->interactionIconEntities[texture_name] = buildIcon(allocator_p, texture_name, mousePos.x, mousePos.y);
    }
}

std::vector<std::weak_ptr<Entity>> EntityManager::getAllUIEntities() {
    std::vector<std::weak_ptr<Entity>> entitiesToBeReturned;
    for (auto &ent : this->UIentities) {
        entitiesToBeReturned.push_back(ent.second);
    }
    return entitiesToBeReturned;
}

std::vector<std::weak_ptr<Entity>> EntityManager::getAllIconEntities() {
    std::vector<std::weak_ptr<Entity>> entitiesToBeReturned;
    for (auto &ent : this->interactionIconEntities) {
        entitiesToBeReturned.push_back(ent.second);
    }
    return entitiesToBeReturned;

}

//  The parameter ID is the same as UUIDComponent ID member
//  and it meant to be used in junction with said value
std::weak_ptr<Entity> EntityManager::getEntity(uint64_t ID) {
    return this->mappedEntities[ID];
}