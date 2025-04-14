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
    this->worldManager.initialize(this->allocator, this->player, gamefilepath);

    //  Chunks generation
    this->worldManager.generateRandomChunk({0,0});  // center
    this->worldManager.generateRandomChunk({1,0});  // right
    this->worldManager.generateRandomChunk({-1,0}); // left

    // Generate sprites for all chunks
    this->worldManager.createChunkSprites(this->allocator);
    
}

void Game::updateSystems(float deltaTime) {
    imguiSystem.update(*this->renderWindow, this->entityManager, this->componentManager);

    inputSystem.update(*this->inputManager, this->componentManager, this->cursorSystem, *this->player);
    cursorSystem.update(*this->renderWindow, this->entityManager, this->componentManager);

    velocitySystem.update(deltaTime, this->entityManager, this->componentManager);
    collisionSystem.update(deltaTime, this->entityManager, this->componentManager);

    animationSystem.update(deltaTime, this->entityManager, this->componentManager);
    stateSystem.update(this->entityManager, this->componentManager);
    
    objectiveSystem.update(this->entityManager, this->componentManager);

    if (!this->player->isFollowingUnit())
        viewpanSystem.update(deltaTime, this->player->playerView, *this->inputManager);

    else 
        controlSystem.update(this->entityManager, this->componentManager, *this->inputManager, *this->player);

}
// Polls event from SFML, sends it to ImGui ProcessEvent() and to inputManager->update()
// Also checks for Event::Closed and returns false if so, otherwise returns true; 
bool Game::handleEventQueue() {
    while (const std::optional event = this->renderWindow->pollEvent()) {
        // Event::Closed
        if (event->is<sf::Event::Closed>()) {
            this->worldManager.saveMapToTMX(gamefilepath);
            this->renderWindow->close();
            return false;// <-- escape loop and send exit signal
        }
        // ImGui
        ImGui::SFML::ProcessEvent(*this->renderWindow, *event);
        // InputManager
        this->inputManager->update(*event);
    }
    return true;
}

void Game::gameLoop() {
    while (this->renderWindow->isOpen()) {
        // 1.  Frame start: Calculate deltaTime
        sf::Time frameStartTime = clock.restart();
        const float deltaTime = frameStartTime.asSeconds();

        // 2.  Input
        if (!this->handleEventQueue())                                // Handle event(s)
            break;

        // 3. ImGui
        ImGui::SFML::Update(*this->renderWindow, frameStartTime);

        // 4.  Update systems (incl. imgui system)
        this->updateSystems(deltaTime);

        // 5.  Render
        this->renderWindow->clear();
        this->renderWindow->setView(player->playerView);         // Set view for world rendering
        this->render();                                          // Render all game tiles & entities

        ImGui::SFML::Render(*this->renderWindow);                // render imgui

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
    this->worldManager.render(this->renderWindow);  // render all background tiles (all chunks)
    renderSystem.update(*renderWindow, this->allocator, this->entityManager, this->componentManager);

    this->renderWindow->setView(this->renderWindow->getDefaultView());  // escape playerView
    // isFollowing/freecam
    drawString(this->renderWindow, this->allocator, this->player->isFollowingUnit() ? "controlling character" : "free cam");
    this->renderWindow->setView(player->playerView);  // reset view
    
}

int Game::run() {
    this->initializeGame();
    this->gameLoop();  // keeps running until RenderWindow closes

    return 0;

}