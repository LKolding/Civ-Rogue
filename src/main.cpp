#include <iostream>
#include <SFML/Graphics.hpp>

// My stuff
#include "ResourceAllocator.hpp"
#include "World.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Civ_Rogue");
    sf::View view1(sf::Vector2f(100.f, 100.f), sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    window.setView(view1);

    ResourceAllocator allocator;
    World world1(allocator);

    sf::Vector2f pos = {1,0};
    world1.chunks.push_back(world1.generateRandomChunk(pos));

    // create sprites
    world1.createChunkSprites(allocator);

    int MOVE_AMOUNT = 12;
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

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::W:
                        view1.move(0, -MOVE_AMOUNT);
                        break;

                    case sf::Keyboard::A:
                        view1.move(-MOVE_AMOUNT, 0);
                        break;

                    case sf::Keyboard::S:
                        view1.move(0, MOVE_AMOUNT);
                        break;

                    case sf::Keyboard::D:
                        view1.move(MOVE_AMOUNT, 0);
                        break;

                    default:
                        break;
                }
            }
        }
        window.setView(view1);
        window.clear();
        world1.render(window);
        window.display();
    }

    world1.saveMapToTMX("../saveGames/game1.tmx");

    return 0;
}
