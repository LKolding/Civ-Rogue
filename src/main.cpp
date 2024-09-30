#include <iostream>
#include <SFML/Graphics.hpp>

// My stuff
#include "ResourceAllocator.hpp"
#include "World.hpp"


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Civ_Rogue");

    ResourceAllocator allocator;
    World world1(allocator);

    if (!world1.loadMap("grass_chunk.tmx")) {
        return -1;
    }
    world1.generateChunks();
    for (Chunk& chunk : world1.chunks) {
        world1.createChunkSprites(chunk, allocator);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }

        window.clear();
        world1.render(window);
        window.display();
    }



    return 0;
}