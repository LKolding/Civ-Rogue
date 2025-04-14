#ifndef _GAME_
#define _GAME_

// sfml
#include <SFML/Graphics.hpp>
// imgui
#include "imgui.h"
#include "imgui-SFML.h"
// system
#include <vector>
#include <memory>
// my stuff

// InputManager
#include "input.hpp"
// helpers
#include "coordinate_calculators.hpp"
#include "game_functions.hpp"
#include "constants.h"
#include "TextDraw.hpp"
// Game stuff
#include "ResourceAllocator.hpp"
#include "Player.hpp"
#include "World.hpp"

// ecs
// systems
#include "ECS/Systems/VelocitySystem.hpp"
#include "ECS/Systems/CollisionSystem.hpp"
#include "ECS/Systems/AnimationSystem.hpp"
#include "ECS/Systems/CursorSystem.hpp"
#include "ECS/Systems/RenderSystem.hpp"
#include "ECS/Systems/ViewpanSystem.hpp"
#include "ECS/Systems/InputSystem.hpp"
#include "ECS/Systems/ObjectiveSystem.hpp"
#include "ECS/Systems/StateSystem.hpp"
#include "ECS/Systems/ImguiSystem.hpp"
#include "ECS/Systems/ControlSystem.hpp"
// entities / components + managers
#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"
#include "EntityFactory.hpp"


class Game
{
public:
    int run(); // game execution starts and ends here
    Game();

private:
    std::shared_ptr<ResourceAllocator> allocator;
    std::shared_ptr<Player> player;

    std::unique_ptr<sf::RenderWindow> renderWindow;
    sf::Clock clock;

    WorldManager worldManager;
    EntityManager entityManager;
    ComponentManager componentManager;

    VelocitySystem velocitySystem;
    AnimationSystem animationSystem;
    CollisionSystem collisionSystem;
    CursorSystem cursorSystem;
    RenderSystem renderSystem;
    ViewpanSystem viewpanSystem;
    ControlSystem controlSystem;
    InputSystem inputSystem;
    ObjectiveSystem objectiveSystem;
    StateSystem stateSystem;
    ImguiSystem imguiSystem;

    std::unique_ptr<InputManager> inputManager;

    void initializeGame();

    void updateSystems(float deltaTime);

    bool handleEventQueue();

    void gameLoop();
    void render();
    void initialize();
};

#endif