#include "Game.hpp"


const sf::Time targetFrameTime = sf::seconds(1.f / 60);  // 60 fps (unused)
const char* gamefilepath = "../saveGames/game1.tmx"; 

Game::Game() {
    this->inputManager = std::make_unique<InputManager>();
    this->allocator = std::make_shared<ResourceAllocator>();
    this->renderWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode({static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT)}), "klunkabadul");
    if (!ImGui::SFML::Init(*this->renderWindow)) // init imgui
        std::cout << "[GAME ERROR] Could not initiliaze ImGui.\n";
    else
        this->renderWindow->resetGLStates();
}

void Game::initializeGame() {
    //  init player
    this->player = std::make_shared<Player>();
    //  read game file and initialize world
    this->world.initialize(this->allocator, this->player, gamefilepath);

    //  Chunks generation
    this->world.generateRandomChunk(sf::Vector2f(0,0)); // center

    this->world.generateRandomChunk(sf::Vector2f(1,0)); // right
    this->world.generateRandomChunk(sf::Vector2f(0,1)); // top
    this->world.generateRandomChunk(sf::Vector2f(1,1)); // top-right
    
    this->world.generateRandomChunk(sf::Vector2f(-1,0)); // left
    this->world.generateRandomChunk(sf::Vector2f(0,-1)); // bottom
    this->world.generateRandomChunk(sf::Vector2f(-1,-1));// bot-left
    
    this->world.generateRandomChunk(sf::Vector2f(-1,1)); // top-left
    this->world.generateRandomChunk(sf::Vector2f(1,-1)); // bot-right

    this->world.generateRandomChunk(sf::Vector2f(1,0)); // bot-right
    this->world.generateRandomChunk(sf::Vector2f(2,0)); // bot-right
    this->world.generateRandomChunk(sf::Vector2f(3,0)); // bot-right

    // Generate sprites for all chunks
    this->world.createChunkSprites(this->allocator);
    
    // buildNinja(this->componentManager, this->entityManager);  // Create ninja
    
}

void Game::updateSystems(float deltaTime) {
    imguiSystem.update(this->entityManager, this->componentManager);

    inputSystem.update(*this->inputManager, this->componentManager);
    cursorSystem.update(*this->renderWindow, this->entityManager, this->componentManager);

    velocitySystem.update(deltaTime, this->entityManager, this->componentManager);
    collisionSystem.update(deltaTime, this->entityManager, this->componentManager);

    animationSystem.update(deltaTime, this->entityManager, this->componentManager);
    viewpanSystem.update(deltaTime, this->player->playerView, *this->inputManager);
    objectiveSystem.update(this->entityManager, this->componentManager);
    stateSystem.update(this->entityManager, this->componentManager);

    //interactionSystem.update(this->entityManager.getAllEntities(), this->inputManager, this->renderWindow, this->player);

}
//  Defines and declares sf::Event. Then polls and processes it
void Game::handleEventQueue() {
    while (const std::optional event = this->renderWindow->pollEvent()) {
        if (!event)
            continue;

        if (event->is<sf::Event::Closed>()) {
            this->world.saveMapToTMX(gamefilepath);
            this->renderWindow->close();
        }

        ImGui::SFML::ProcessEvent(*this->renderWindow, *event);
        this->inputManager->update(*event);
            
    }
}

void Game::gameLoop() {
    while (this->renderWindow->isOpen()) {
        // 1.  Frame start: Calculate deltaTime
        sf::Time frameStartTime = clock.restart();
        const float deltaTime = frameStartTime.asSeconds();

        // 2.  Input
        this->handleEventQueue();                                // Handle event(s)

        // 3. ImGui
        ImGui::SFML::Update(*this->renderWindow, frameStartTime);

        // 4.  Update systems (incl. imgui system)
        this->updateSystems(deltaTime);

        // 5.  Render
        this->renderWindow->clear();
        this->renderWindow->setView(player->playerView);         // Set view for world rendering
        this->render();                                          // Render all game tiles & entities

        ImGui::SFML::Render(*this->renderWindow);                // render imgui

        // Apply and display
        this->renderWindow->setView(player->playerView);  // reset view
        this->renderWindow->display();

        // 6.  FPS mangement (currently unused eg. no frame cap applied)
        sf::Time frameTime = clock.getElapsedTime();
        if (frameTime < targetFrameTime) {
            //sf::sleep(targetFrameTime - frameTime);
        }
    } // while loop
    ImGui::SFML::Shutdown();
}

void Game::render() {
    this->world.render(this->renderWindow);  // render all background tiles (all chunks)
    renderSystem.update(this->renderWindow, this->allocator, this->entityManager, this->componentManager);

    this->renderWindow->setView(this->renderWindow->getDefaultView());  // escape playerView
    // isFollowing/freecam
    std::string following = this->player->isFollowingUnit() ? "controlling character" : "free cam";
    drawString(this->renderWindow, this->allocator, following);

    
}

int Game::run() {
    this->initializeGame();
    this->gameLoop();  // keeps running until RenderWindow closes

    return 0;

}