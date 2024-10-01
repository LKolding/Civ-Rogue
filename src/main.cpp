#include <iostream>
#include <SFML/Graphics.hpp>

// My stuff
#include "ResourceAllocator.hpp"
#include "World.hpp"
#include "Player.h"


int main() {
    // window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Civ_Rogue");
    // player and view
    auto playerp = std::make_shared<Player>();
    playerp->playerView.reset(sf::FloatRect(sf::Vector2f(100.f, 100.f),sf::Vector2f(WINDOW_WIDTH*0.7, WINDOW_HEIGHT*0.7)));

    // allocator and world
    ResourceAllocator allocator;
    World world1;
    world1.initialize(allocator, playerp);

    // create sprites
    world1.createChunkSprites(allocator);

    int MOVE_AMOUNT = 12;
    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                //world1.saveMapToTMX("../saveGames/game1_test.tmx");
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                world1.saveMapToTMX("../saveGames/game1_test.tmx");
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::W:
                        playerp->playerView.move(0, -MOVE_AMOUNT);
                        break;

                    case sf::Keyboard::A:
                        playerp->playerView.move(-MOVE_AMOUNT, 0);
                        break;

                    case sf::Keyboard::S:
                        playerp->playerView.move(0, MOVE_AMOUNT);
                        break;

                    case sf::Keyboard::D:
                        playerp->playerView.move(MOVE_AMOUNT, 0);
                        break;

                    default:
                        break;
                }
            }

            // zoom w/ mousewheel
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) {
                    if (playerp->playerView.getSize().x > 300)
                        playerp->playerView.zoom(0.9f);  // Zoom in
                } else if (event.mouseWheelScroll.delta < 0) {
                    if (playerp->playerView.getSize().x < 800)
                        playerp->playerView.zoom(1.1f);  // Zoom out
                }
            }
        }

        window.setView(playerp->playerView);
        window.clear();
        world1.render(window);
        window.display();
    }

    return 0;
}
