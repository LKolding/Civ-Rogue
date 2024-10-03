#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

// system
#include <cmath>
#include <iostream>

// My stuff
#include "ResourceAllocator.hpp"
#include "World.hpp"
#include "Player.h"
// render
#include "Entity.hpp"

#include "input.hpp"

sf::Vector2i getChunkCoords(sf::Vector2f coords) {
    // Calculate chunk coordinates
    int chunkX = static_cast<int>(coords.x / (CHUNK_WIDTH  * TILE_WIDTH / 2));
    int chunkY = static_cast<int>(coords.y / (CHUNK_HEIGHT * TILE_HEIGHT / 2));
    return sf::Vector2i(chunkX,chunkY);
}
// returns the index of the tile, that the
// coordinates are 
sf::Vector2f getTileIndex(sf::Vector2f coords) {
    // Calculate local coordinates within the chunk (in pixels)
    int localXInChunk = static_cast<int>(coords.x) % (CHUNK_WIDTH * (TILE_WIDTH / 2));
    int localYInChunk = static_cast<int>(coords.y) % (CHUNK_HEIGHT * (TILE_HEIGHT / 2));

    // Determine the tile within the chunk
    int tileX = localXInChunk / (TILE_WIDTH / 2);
    int tileY = localYInChunk / (TILE_HEIGHT / 2);

    return sf::Vector2f(tileX, tileY);

}

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



int main() {
    // window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Civ_Rogue");
    // player and view
    auto playerp = std::make_shared<Player>();
    playerp->playerView.reset(sf::FloatRect(sf::Vector2f(100.f, 100.f), sf::Vector2f(WINDOW_WIDTH*0.7, WINDOW_HEIGHT*0.7)));
    window.setView(playerp->playerView);
    // allocator and world
    ResourceAllocator allocator;
    World world1;
    world1.initialize(allocator, playerp);
    // input handler
    InputManager inputhandler(playerp);
    // create sprites
    world1.createChunkSprites(allocator);

    float deltaTime = 0.01f;
    while (window.isOpen()) {
        inputhandler.update(deltaTime);

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
                
            else {
                if (!handle_event(event, inputhandler, playerp, deltaTime)) {
                    world1.saveMapToTMX("../saveGames/game1_test.tmx");
                    window.close();
                }
            }
                            
        }
        
        window.setView(playerp->playerView);
        window.clear();
        world1.render(window);
        renderSelectionBox(window);
        window.display();
    }
    return 0;
}
