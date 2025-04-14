#ifndef _INPUT_SYSTEM_
#define _INPUT_SYSTEM_

// SYSTEM RESPONSIBLE FOR:
// checking all relevant keys in the InputManagers lookup table
// and makes sure that entities are (de-)selected upon mouse presses
// and followed upon space bar

#include "input.hpp" // <- InputManager
#include "Player.hpp"
// #include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"
#include "ECS/Components/Components.hpp"
#include "ECS/Systems/CursorSystem.hpp"


class InputSystem {
    public:
    inline void update(InputManager& im, ComponentManager& cm, CursorSystem& cursor, Player& player) {
        // Keyboard
        for (auto& key : im.keyState) {
            // Space key
            if (key.first == sf::Keyboard::Key::Space && key.second) {
                // Ensures to, if entity wasn't hovered, call stopFollow() on Player class 
                bool didHoverEntity = false;

                auto& storage = cm.getStorage<HoverComponent>();
                for (size_t i = 0; i < storage.components.size(); ++i) {
                    auto& component = storage.components[i];

                    if (component.isHovered) { // <--- if hovered, call followUnit on Player class
                        EntityID hoveredEntity = storage.indexToEntity[i];
                        player.followUnit(hoveredEntity);
                        didHoverEntity = true;
                    }
                }

                if (!didHoverEntity)
                    player.stopFollow();
            }
        }
        // Mouse buttons
        for (auto& key : im.mkeyState) {
            // Right mouse btn
            if (key.first == sf::Mouse::Button::Right && key.second) {
                auto& hoverStorage = cm.getStorage<HoverComponent>();
                for (size_t i = 0; i < hoverStorage.components.size(); ++i) {
                    auto& component = hoverStorage.components[i];

                    if (component.isHovered) { // <--- if hovered, UN-follow entity
                        EntityID hoveredEntity = hoverStorage.indexToEntity[i];
                        cm.getComponent<SelectComponent>(hoveredEntity)->isSelected = false;
                    }
                }
            }
            // Left mouse btn ( selects and sends objective to hovered entity(-ies) )
            else if (key.first == sf::Mouse::Button::Left && key.second) {
                // Ensures that, if any entity was hovered, no objective will be sent selected entities
                bool didHoverEntity = false;
                // Get hovered entities
                auto& hoverStorage = cm.getStorage<HoverComponent>();
                for (size_t i = 0; i < hoverStorage.components.size(); ++i) {
                    auto& component = hoverStorage.components[i];

                    if (component.isHovered) { // <--- if hovered, select entity
                        EntityID hoveredEntity = hoverStorage.indexToEntity[i];
                        cm.getComponent<SelectComponent>(hoveredEntity)->isSelected = true;
                        didHoverEntity = true;
                    }
                }
                if (didHoverEntity)
                    break;

                // Get selected entities
                auto& selectStorage = cm.getStorage<SelectComponent>();
                for (size_t i = 0; i < selectStorage.components.size(); ++i) {
                    auto& component = selectStorage.components[i];

                    if (component.isSelected) { // <--- if selected, send objective
                        EntityID selectedEntity = selectStorage.indexToEntity[i];
                        cm.getComponent<ObjectiveComponent>(selectedEntity)->hasObjective = true;
                        cm.getComponent<ObjectiveComponent>(selectedEntity)->x = cursor.world_pos.x;
                        cm.getComponent<ObjectiveComponent>(selectedEntity)->y = cursor.world_pos.y;
                    }
                }
            }
        }
    }
};

#endif