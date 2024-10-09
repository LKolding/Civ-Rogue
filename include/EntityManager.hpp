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
    inline void createEntity() {
        if (std::is_same<T, NinjaEntity>::value) {
            auto nja_p = buildNinja(this->allocator_p);
            auto htl_p = buildHealthbar(this->allocator_p, nja_p);

            this->entities.push_back(std::move(nja_p));
            this->entities.push_back(std::move(htl_p));

            //this->mappedEntities[nja_p->getComponent<UUIDComponent>()->ID] = nja_p;
            //this->mappedEntities[htl_p->getComponent<UUIDComponent>()->ID] = htl_p;
            
        } else
        if (std::is_same<T, TreeEntity>::value) {
            this->entities.push_back(buildTree(this->allocator_p));
        } else 
        if (std::is_same<T, EyeBugEntity>::value) {
            this->entities.push_back(buildTree(this->allocator_p));
            this->entities.push_back(buildHealthbar(this->allocator_p, this->entities.back()));
        }
    };
    void addEntity(std::shared_ptr<Entity> entity);
    void killEntity(uint64_t ID);
    void renderArrow(sf::Vector2f mouse);
    // pos = pos of chunk
    void createBorderEntities(std::shared_ptr<ResourceAllocator> allocator, sf::Vector2i pos); 
    void update(float deltaTime);
    // Returns a vector containing weak_ptr to all entities
    std::vector<std::weak_ptr<Entity>> getAllEntities();
    std::weak_ptr<Entity> getEntity(uint64_t ID);

private:
    std::shared_ptr<ResourceAllocator> allocator_p;
    std::vector<std::shared_ptr<Entity>> entities;  // holds the "last" shared pointer to entities
    std::unordered_map<uint32_t, std::weak_ptr<Entity>> mappedEntities; 
};

#endif