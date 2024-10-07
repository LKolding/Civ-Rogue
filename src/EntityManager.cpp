#include "EntityManager.hpp"


void EntityManager::setAllocator(std::shared_ptr<ResourceAllocator> allocator) {
        this->allocator_p = allocator;
        // TODO TEMP
        this->entities.push_back(buildNinja(allocator_p));
        this->entities.push_back(buildNinja(allocator_p, 300, 200));
        this->entities.push_back(buildNinja(allocator_p, 460, 400));

}


template <typename T>
void EntityManager::createEntity(T entityClass) {
    if (std::is_same<T, NinjaEntity>::value) {
        this->entities.push_back(buildNinja(this->allocator_p));
    }
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
}

void EntityManager::update(float deltaTime) {
    for (auto& entity : this->entities) {
        entity->update(deltaTime);
    }
}

std::vector<std::weak_ptr<Entity>> EntityManager::getAllEntities() {
    std::vector<std::weak_ptr<Entity>> entities_vector;

    for (auto &entity : this->entities) {
        entities_vector.push_back(std::weak_ptr<Entity>(entity));
    }

    return entities_vector;
}