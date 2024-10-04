#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

// system
#include <cmath>
#include <iostream>

// My stuff
#include "ResourceAllocator.hpp"
#include "World.hpp"
#include "Player.hpp"

#include "HumanFactory.hpp"  // crafts HumanEntity
// Entities
#include "ECS/Entities/Entity.hpp"
#include "ECS/Entities/Human.hpp"
// Systems
#include "ECS/Systems/MovementSystem.hpp"
#include "ECS/Systems/RenderSystem.hpp"

#include "input.hpp"

#include "coordinate_calculators.hpp"


void renderSelectionBox(sf::RenderWindow &ren) {
    //  ren.mapPixelToCoords()
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


std::string game_path = "../saveGames/game1.tmx";

int main() {
    // window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Civ_Rogue");
    //  player and view
    auto playerp = std::make_shared<Player>();
    playerp->playerView.reset(sf::FloatRect(sf::Vector2f(100.f, 100.f), sf::Vector2f(WINDOW_WIDTH*0.7, WINDOW_HEIGHT*0.7)));
    window.setView(playerp->playerView);
    //  allocator and world
    ResourceAllocator allocator;
    World world1;
    world1.initialize(allocator, playerp, game_path);
    //  input handler
    InputManager inputhandler(playerp);
    //  create sprites for chunks
    world1.createChunkSprites(allocator);
    //  entities
    std::vector<std::unique_ptr<Entity>> entities;
    entities.push_back(buildHumanMage(allocator));
    entities.push_back(buildHumanMage(allocator, 200.0f, 200.0f));
    entities.push_back(buildHumanMage(allocator, 300.0f, 200.0f));
    //  systems
    MovementSystem movementSystem;
    RenderSystem renderSystem;

    float deltaTime = 0.01f;
    while (window.isOpen()) {
        inputhandler.update(deltaTime);

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            else {
                if (!handle_event(event, inputhandler, playerp, deltaTime)) {
                    world1.saveMapToTMX(game_path);
                    window.close();
                }
            } 
        }

        // Update entities
        for (const auto& entity : entities) { entity->update(deltaTime); }
        window.setView(playerp->playerView);
        window.clear();
        world1.render(window);
        renderSystem.update(window, entities);
        //renderSelectionBox(window);
        window.display();
    }
    return 0;
}
