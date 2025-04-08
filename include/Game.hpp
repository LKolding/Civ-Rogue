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

// // ecs
// #include "ECS/Systems/VelocitySystem.hpp"
// #include "ECS/Systems/CollisionSystem.hpp"
#include "ECS/Systems/AnimationSystem.hpp"
#include "ECS/Systems/CursorSystem.hpp"

#include "ECS/Systems/RenderSystem.hpp"

#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"
#include "EntityFactory.hpp"

#include "game_functions.hpp"
#include "TextDraw.hpp"
  

class Game {
public:

    int run(); // game execution starts and ends here
    Game();

private:
    std::shared_ptr<ResourceAllocator> allocator;
    std::shared_ptr<Player> player;

    std::unique_ptr<sf::RenderWindow> renderWindow;
    sf::Clock clock;

    World world;
    EntityManager entityManager;
    ComponentManager componentManager;

    // VelocitySystem    movementSystem;
    AnimationSystem animationSystem;
    // CollisionSystem   collisionSystem;
    CursorSystem cursorSystem;

    RenderSystem renderSystem;

    std::unique_ptr<InputManager> inputManager;

    void initializeGame();

    void updateSystems(float deltaTime);

    void handleEventQueue();

    void gameLoop();
    void render();
    void initialize();

};

#endif