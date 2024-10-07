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

bool inline isEntityHovered(sf::RenderWindow &ren, std::shared_ptr<Entity>& entity) {
    // Returns true if the entity's CollisionComponents boundingbox is currently
    // below sf::Mouse::getPosition()
    if (entity->hasComponent<DeletableComponent>()) {
        return false; // skip if entity is a healthbar (only entity containing Deletable)
    }
    sf::Vector2f worldCoords = ren.mapPixelToCoords(sf::Mouse::getPosition(ren));
    auto colli_ptr = entity->getComponent<CollisionComponent>();
    if (colli_ptr->bounds.contains(worldCoords.x, worldCoords.y)) {
        return true;
    }
    return false;
}

void inline selectUnits(sf::RenderWindow &ren, std::shared_ptr<Player> player, std::vector<std::shared_ptr<Entity>>& entities) {
    for (auto& entity : entities) {
        if (entity->hasComponent<CollisionComponent>() && isEntityHovered(ren, entity) && entity->hasComponent<SelectableComponent>()) {
            entity->getComponent<SelectableComponent>()->isSelected = true;
            player->selectUnit(entity);
        }
    }
}

void inline deselectUnits(sf::RenderWindow &ren, std::shared_ptr<Player> player, std::vector<std::shared_ptr<Entity>>& entities) {
    for (auto& entity : entities) {
        for (auto& entity : entities) {
            if (entity->hasComponent<CollisionComponent>() && isEntityHovered(ren, entity) && entity->hasComponent<SelectableComponent>()) {
                entity->getComponent<SelectableComponent>()->isSelected = false;
                player->deselectUnit(entity);
            }
        }
        
    }
}

/*
void drawGameState(sf::RenderWindow &ren, GameState& game_state) {
    static sf::Font font;
    static bool hasBeenLoaded;
    if (!hasBeenLoaded) {
        font.loadFromFile("../assets/fonts/BD_Cartoon_Shout.ttf");
        hasBeenLoaded = true;
    }
    std::string entity_text = "Entities: " + std::to_string(game_state.amount_of_entities);
    sf::Text text;
    text.setFont(font);
    text.setString(entity_text);
    text.setCharacterSize(32);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(40, 20));

    ren.draw(text);

}*/


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

    EntityManager entityManager;
    std::shared_ptr<InputManager> inputManager;

    sf::Clock clock;

    void initializeGame(std::string &gameFilePath, sf::RenderWindow &ren);

    void updateSystems(float deltaTime);

    bool handleEvent(sf::RenderWindow &ren);

    // Returns false on exit
    void gameLoop(sf::RenderWindow &ren);

    void render(sf::RenderWindow &ren);
    void initialize();

};

#endif