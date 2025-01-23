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
#include "ECS/Systems/MovementSystem.hpp"
#include "ECS/Systems/AnimationSystem.hpp"
#include "ECS/Systems/CollisionSystem.hpp"
#include "ECS/Systems/InteractionSystem.hpp"

#include "ECS/Systems/RenderSystem.hpp"

#include "EntityManager.hpp"
#include "EntityFactory.hpp"

#include "game_functions.hpp"
  

class Game {
public:

    int run(); // game execution starts and ends here
    Game();

private:
    std::shared_ptr<ResourceAllocator> allocator;

    World world;
    EntityManager entityManager;
    std::shared_ptr<Player> player;

    std::unique_ptr<sf::RenderWindow> renderWindow;
    sf::Clock clock;

    std::unique_ptr<MovementSystem> movementSystem;
    std::unique_ptr<AnimationSystem>animationSystem;
    std::unique_ptr<CollisionSystem>collisionSystem;
    std::unique_ptr<InteractionSystem>interactionSystem;
    std::unique_ptr<RenderSystem>   renderSystem;

    std::unique_ptr<InputManager> inputManager;

    void initializeGame();

    void updateSystems(float deltaTime);

    void handleEventQueue();

    void gameLoop();
    void render();
    void initialize();

};

#endif