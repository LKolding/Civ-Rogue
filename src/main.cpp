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
    Player player;
    player.playerView.reset(sf::FloatRect(sf::Vector2f(100.f, 100.f),sf::Vector2f(WINDOW_WIDTH*0.7, WINDOW_HEIGHT*0.7)));

    // allocator and world
    ResourceAllocator allocator;
    World world1(allocator);
    world1.setPlayer(player);

    // generate a few random chunks
    sf::Vector2f pos = {1,0};
    world1.generateRandomChunk(pos);
    pos.x++;
    world1.generateRandomChunk(pos);
    pos.x =0;
    pos.y++;
    world1.generateRandomChunk(pos);
    pos.x++;
    world1.generateRandomChunk(pos);

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
                        player.playerView.move(0, -MOVE_AMOUNT);
                        break;

                    case sf::Keyboard::A:
                        player.playerView.move(-MOVE_AMOUNT, 0);
                        break;

                    case sf::Keyboard::S:
                        player.playerView.move(0, MOVE_AMOUNT);
                        break;

                    case sf::Keyboard::D:
                        player.playerView.move(MOVE_AMOUNT, 0);
                        break;

                    default:
                        break;
                }
            }

            // zoom w/ mousewheel
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) {
                    if (player.playerView.getSize().x > 300)
                        player.playerView.zoom(0.9f);  // Zoom in
                } else if (event.mouseWheelScroll.delta < 0) {
                    if (player.playerView.getSize().x < 800)
                        player.playerView.zoom(1.1f);  // Zoom out
                }
            }
        }

        window.setView(player.playerView);
        window.clear();
        world1.render(window);
        window.display();
    }

    //world1.saveMapToTMX("../saveGames/game1.tmx");

    return 0;
}
