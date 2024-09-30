#include <iostream>
#include <SFML/Graphics.hpp>

// My stuff
#include "ResourceAllocator.hpp"
#include "World.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Civ_Rogue");

    ResourceAllocator allocator;
    World world1(allocator);

    sf::Vector2f pos = {1,0};
    world1.chunks.push_back(world1.generateRandomChunk(pos));

    // create sprites
    world1.createChunkSprites(allocator);

    while (window.isOpen()) 
    {
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

    world1.saveMapToTMX("../saveGames/dildo.tmx");

    return 0;
}