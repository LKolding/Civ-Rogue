#ifndef _ENTITIY_
#define _ENTITIY_

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <typeinfo>

#include "Components.hpp"

class Entity {
public:
    // Store components using std::type_index to map to their types
    std::unordered_map<std::type_index, std::shared_ptr<void>> components;

    // Method to add a component
    template <typename T>
    void addComponent(std::shared_ptr<T> component) {
        components[typeid(T)] = component;
    }

    // Method to check if the entity has a specific component
    template <typename T>
    bool hasComponent() const {
        return components.find(typeid(T)) != components.end();
    }

    // Method to retrieve a component
    template <typename T>
    std::shared_ptr<T> getComponent() const {
        auto it = components.find(typeid(T));
        if (it != components.end()) {
            // Cast the void pointer back to the correct type
            return std::static_pointer_cast<T>(it->second);
        }
        return nullptr;  // Return nullptr if component is not found
    }
};

#endif