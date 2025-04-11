#ifndef _INPUT_SYSTEM_
#define _INPUT_SYSTEM_

#include "input.hpp" // <- InputManager
// #include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"
#include "ECS/Components/Components.hpp"


class InputSystem {
    public:
    inline void update(InputManager& im, ComponentManager& cm) {
        // Keyboard
        for (auto key : im.keyState) {
            if (key.first == sf::Keyboard::Key::Space && key.second) {
                // space is being held down
                auto& storage = cm.getStorage<HoverComponent>();
                for (size_t i = 0; i < storage.components.size(); ++i) {
                    auto& component = storage.components[i];

                    if (component.isHovered) { // <--- if hovered, follow entity
                        EntityID hoveredEntity = storage.indexToEntity.at(i);
                        cm.getComponent<FollowComponent>(hoveredEntity)->isFollowing = true;
                    }
                }   
            }
        }
        // Mouse buttons
        for (auto& key : im.mkeyState) {
            if (key.first == sf::Mouse::Button::Right && key.second) {
                auto& storage = cm.getStorage<HoverComponent>();
                for (size_t i = 0; i < storage.components.size(); ++i) {
                    auto& component = storage.components[i];

                    if (component.isHovered) { // <--- if hovered, UN-follow entity
                        EntityID hoveredEntity = storage.indexToEntity.at(i);
                        cm.getComponent<FollowComponent>(hoveredEntity)->isFollowing = false;
                    }
                }
            }
        }
    }
};

#endif