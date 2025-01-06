#ifndef _GAME_FUNCS_
#define _GAME_FUNCS_

#include <SFML/Graphics.hpp>

#include <memory>

#include "coordinate_calculators.hpp"
#include "constants.h"

#include "Player.hpp"
#include "EntityManager.hpp"

#include "ECS/Entities/Entity.hpp"
#include "ECS/Components/Components.hpp"


void inline renderSelectionBox(sf::RenderWindow &ren) {
    //  renders a rectangle on top of the tile that the cursor
    //  is currently hovering above
    sf::Vector2f worldCoords = ren.mapPixelToCoords(sf::Mouse::getPosition(ren));

    sf::RectangleShape rect1(sf::Vector2f(TILE_WIDTH/2, TILE_HEIGHT/2));
    
    sf::Vector2f rectPos;
    // offset position of rectangle based on chunk
    rectPos.x = getChunkCoords(worldCoords).x * CHUNK_WIDTH * (TILE_WIDTH / 2);  
    rectPos.y = getChunkCoords(worldCoords).y * CHUNK_HEIGHT * (TILE_HEIGHT / 2);
    // offset position of rectangle based on tile (within chunk)
    rectPos.x+= getTileIndex(worldCoords).x * (TILE_WIDTH / 2);
    rectPos.y+= getTileIndex(worldCoords).y * (TILE_HEIGHT / 2);

    rect1.setPosition(rectPos);
    rect1.setOutlineColor(sf::Color::Black);
    rect1.setFillColor(sf::Color::Transparent);
    rect1.setOutlineThickness(1.5);
    rect1.setScale(0.9f, 0.9f);

    ren.draw(rect1);
}

bool inline isEntityHovered(sf::RenderWindow &ren, std::weak_ptr<Entity> entity_p) {
    // Returns true if the entity's CollisionComponents boundingbox is currently
    // below sf::Mouse::getPosition()
    if (auto entity = entity_p.lock()) {
        sf::Vector2f worldCoords = ren.mapPixelToCoords(sf::Mouse::getPosition(ren));
        if (!entity->hasComponent<SelectableComponent>()) {
            return false;
        }
        auto spr_ptr = entity->getComponent<SpriteComponent>();
        if (spr_ptr->sprite.getGlobalBounds().contains(worldCoords.x, worldCoords.y)) {
            return true;
        }
    }
    
    return false;
}

// Iterates through all entities and select the one being hovered (if any)
void inline selectUnits(sf::RenderWindow &ren, std::shared_ptr<Player> player, std::vector<std::weak_ptr<Entity>> entities_p) {
    for (auto entity_p : entities_p) {
        if (auto entity = entity_p.lock()) {
            if (entity->hasComponent<CollisionComponent>() && isEntityHovered(ren, entity) && entity->hasComponent<SelectableComponent>()) {
            entity->getComponent<SelectableComponent>()->isSelected = true;
            player->selectUnit(entity);
            }

        }
        
    }
}

void inline deselectUnits(sf::RenderWindow &ren, std::shared_ptr<Player> player, std::vector<std::weak_ptr<Entity>> entities_p) {
    for (auto entity_p : entities_p) {
        if (auto entity = entity_p.lock()) {
            if (entity->hasComponent<CollisionComponent>() && isEntityHovered(ren, entity) && entity->hasComponent<SelectableComponent>()) {
                entity->getComponent<SelectableComponent>()->isSelected = false;
                player->deselectUnit(entity);
            }
        }
        
    }
}

// checks if any entity is hovered that has
void inline followEntity(sf::RenderWindow &ren, std::shared_ptr<Player> player, std::vector<std::weak_ptr<Entity>> entities_p) {
    for (auto entity_p : entities_p) {
        if (auto entity = entity_p.lock()) {
            if (entity->hasComponent<CollisionComponent>() && isEntityHovered(ren, entity)) {
                player->followUnit(entity);
                return;
            }
        }
    }
}


void inline checkForInteraction(sf::Vector2f mousePosition, EntityManager& entityManager) {
    //  Coordinates of the chunk that the mouse position is in
    std::pair<int,int> chunkCoords;
    chunkCoords.first  = getChunkCoords(mousePosition).x;
    chunkCoords.second = getChunkCoords(mousePosition).y;
    auto nearbyEntities = entityManager.getAllEntitiesInChunk(chunkCoords);

    for (auto& entityp : nearbyEntities) {
        if (auto entity = entityp.lock()) {
            if (entity->hasComponent<InteractableComponent>()) {
                auto interactableBounds = entity->getComponent<SpriteComponent>()->sprite.getGlobalBounds();
                // Check if the mouse is within the bounds of the entity
                if (interactableBounds.contains(mousePosition)) {
                    if (!entity->hasComponent<InteractableComponent>())
                        continue;

                    switch (entity->getComponent<InteractableComponent>()->type) {
                        case InteractableComponent::ATTACKABLE:
                            // show attack icon at cursor
                            entityManager.showIconEntity("sword.png", mousePosition);
                            break;

                        case InteractableComponent::MINEABLE:
                            // show pickaxe icon at cursor
                            break;

                        case InteractableComponent::FELLABLE:
                            // show axe icon at cursor
                            entityManager.showIconEntity("axe.png", mousePosition);
                            break;

                        default:
                            break;
                    }
                    return;  // Exit early since we've found an interactable
                }
            }
        }
    }
    //hideInteractionIcon();  // Hide icon if no interactable entity is found
}

#endif