#ifndef _COMPONENT_MANAGER_
#define _COMPONENT_MANAGER_

#include <vector>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <cstddef>
#include <algorithm>

#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"

#include "constants.h"

/// =====================
// COMPONENT STORAGE INTERFACE
// =====================
class IComponentStorage {
    public:
        virtual ~IComponentStorage() = default;
        virtual void remove(EntityID id) = 0;
    };
    
    // =====================
    // COMPONENT STORAGE TEMPLATE
    // =====================
    template<typename T>
    class ComponentStorage : public IComponentStorage {
    public:
        std::vector<T> components;
        std::unordered_map<EntityID, size_t> entityToIndex;
        std::unordered_map<size_t, EntityID> indexToEntity;
    
        void add(EntityID id, const T& component) {
            size_t index = components.size();
            components.push_back(component);
            entityToIndex[id] = index;
            indexToEntity[index] = id;
        }
    
        T* get(EntityID id) {
            if (entityToIndex.find(id) == entityToIndex.end())
                return nullptr;
            return &components[entityToIndex[id]];
        }
    
        void remove(EntityID id) override {
            auto it = entityToIndex.find(id);
            if (it == entityToIndex.end()) return;
    
            size_t index = it->second;
            size_t lastIndex = components.size() - 1;
    
            if (index != lastIndex) {
                std::swap(components[index], components[lastIndex]);
                EntityID movedID = indexToEntity[lastIndex];
                entityToIndex[movedID] = index;
                indexToEntity[index] = movedID;
            }
    
            components.pop_back();
            entityToIndex.erase(id);
            indexToEntity.erase(lastIndex);
        }
    };
    
    // =====================
    // COMPONENT MANAGER
    // =====================
    class ComponentManager {
    public:
        template<typename T>
        ComponentStorage<T>& getStorage() {
            std::type_index type = std::type_index(typeid(T));
            if (storages.find(type) == storages.end())
                storages[type] = std::make_unique<ComponentStorage<T>>();
            return *static_cast<ComponentStorage<T>*>(storages[type].get());
        }
    
        template<typename T>
        void addComponent(EntityID id, const T& component) {
            getStorage<T>().add(id, component);
        }
    
        template<typename T>
        T* getComponent(EntityID id) {
            return getStorage<T>().get(id);
        }
    
        template<typename T>
        void removeComponent(EntityID id) {
            getStorage<T>().remove(id);
        }
    
    private:
        std::unordered_map<std::type_index, std::unique_ptr<IComponentStorage>> storages;
    };

#endif