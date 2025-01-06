#ifndef _GAME_
#define _GAME_

// sfml
#include <SFML/Graphics.hpp>
// system
#include <vector>
// my stuff

// helpers and InputManager
#include "input.hpp"
#include "coordinate_calculators.hpp"
#include "constants.h"
// Game stuff
#include "ResourceAllocator.hpp"
#include "Player.hpp"
#include "World.hpp"

// ecs
#include "ECS/Systems/System.hpp"
#include "ECS/Systems/MovementSystem.hpp"
#include "ECS/Systems/AnimationSystem.hpp"
#include "ECS/Systems/CollisionSystem.hpp"
#include "ECS/Systems/LifetimeSystem.hpp"

#include "ECS/Systems/RenderSystem.hpp"

#include "ECS/Entities/Button.hpp"

#include "EntityManager.hpp"
#include "EntityFactory.hpp"

#include "game_functions.hpp"


//  Handles a few mouse press detection behaviors and space bar to follow unit mechanic
void inline performEventBehavior(sf::Event event, sf::RenderWindow &ren, std::shared_ptr<Player> player, EntityManager& entityManager, std::shared_ptr<ResourceAllocator> allocator) {
    // Follow Entity that is being hovered on Spacebar
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
        if (player->isFollowingUnit()) {
            player->stopFollow();
        } else {
            followEntity(ren, player, entityManager.getAllEntities());
        }
    }
    //  LEFT MOUSE BTN (select units / add unit to selected)
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        const auto mouse_pos = sf::Vector2i(ren.mapPixelToCoords(sf::Mouse::getPosition(ren)).x, ren.mapPixelToCoords(sf::Mouse::getPosition(ren)).y);
        selectUnits(ren, player, entityManager.getAllEntities());
    }
    // deselect units
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
        const auto mouse_pos = sf::Vector2i(ren.mapPixelToCoords(sf::Mouse::getPosition(ren)).x, ren.mapPixelToCoords(sf::Mouse::getPosition(ren)).y);
        bool someEntityIsBeingHovered = false;
        // iterates through all entites and checks if any are currently being hovered
        for (auto entity : entityManager.getAllEntities()) { 
            if ((someEntityIsBeingHovered = isEntityHovered(ren, entity)))
                break;
        }
        if (someEntityIsBeingHovered) {
            deselectUnits(ren, player, entityManager.getAllEntities());
            
        } else {
            entityManager.addEntity(buildBluePointer(allocator, mouse_pos.x, mouse_pos.y));
        }
    }
}

class Game {
public:
    int run(); // game execution starts and ends here
    Game();

private:
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

    void handleEvent(sf::RenderWindow &ren);

    // Returns false on exit
    void gameLoop(sf::RenderWindow &ren);
    void renderUI(sf::RenderWindow &ren);
    void render(sf::RenderWindow &ren);
    void initialize();

};

#endif