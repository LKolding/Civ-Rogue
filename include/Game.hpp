#ifndef _GAME_
#define _GAME_

// sfml
#include <SFML/Graphics.hpp>
// system
#include <vector>
// my stuff
#include "ResourceAllocator.hpp"
#include "World.hpp"
#include "Player.hpp"

// ecs
#include "ECS/Systems/System.hpp"
#include "ECS/Systems/MovementSystem.hpp"
#include "ECS/Systems/AnimationSystem.hpp"
#include "ECS/Systems/CollisionSystem.hpp"
#include "ECS/Systems/ObjectiveSystem.hpp"

#include "ECS/Systems/RenderSystem.hpp"

#include "ECS/Entities/Button.hpp"

#include "EntityManager.hpp"
// other
#include "input.hpp"
#include "coordinate_calculators.hpp"

#include "constants.h"


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
        if (!entity->hasComponent<CollisionComponent>()) {
            return false;
        }
        auto colli_ptr = entity->getComponent<CollisionComponent>();
        if (colli_ptr->bounds.contains(worldCoords.x, worldCoords.y)) {
            return true;
        }
    }
    
    return false;
}

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

struct GameState {
    enum State{INGAME, MENU} state;
};


class Game {
public:
    int run(); // game execution starts and ends here
    Game();

private:
    GameState gameState;

    std::shared_ptr<ResourceAllocator> allocator;
    World world;

    std::shared_ptr<Player> player;

    std::vector<std::unique_ptr<System>> systems;
    std::unique_ptr<RenderSystem> renderSystem;

    std::vector<std::shared_ptr<Entity>> buttons_vectorp;

    EntityManager entityManager;
    std::shared_ptr<InputManager> inputManager;

    sf::Clock clock;

    void initializeGame(std::string &gameFilePath, sf::RenderWindow &ren);

    void updateSystems(float deltaTime);

    bool handleEvent(sf::RenderWindow &ren);

    // Returns false on exit
    void gameLoop(sf::RenderWindow &ren);
    void renderUI(sf::RenderWindow &ren);
    void render(sf::RenderWindow &ren);
    void initialize();

};

#endif