#include "Game.hpp"


const sf::Time targetFrameTime = sf::seconds(1.f / 120); // 120 fps
const char* gamefilepath = "../saveGames/game1.tmx";


Game::Game() {
    if (!sf::Shader::isAvailable())
        std::cout << "Shaders are not available. Will proceed regardless...\n";

    this->inputManager = std::make_unique<InputManager>();
    this->allocator = std::make_shared<ResourceManager>();
    this->renderWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode({static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT)}), "klunkabadul");
    this->player = std::make_shared<Player>();

    if (!ImGui::SFML::Init(*this->renderWindow)) // init imgui
        throw std::runtime_error("[GAME ERROR] Could not initialize ImGui");
    else
        this->renderWindow->resetGLStates();
}

void Game::initializeGame() {
    //  read game file and initialize world
    this->worldManager.initialize(this->allocator, gamefilepath);
    //  Chunks generation
    // this->worldManager.generateRandomChunk({0,0});  // center
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

    viewzoomSystem.update(this->player->playerView, *this->inputManager);

    if (!this->player->isFollowingUnit()) // I should make a minimap system...
        viewpanSystem.update(deltaTime, *this->player, *this->inputManager);
    else 
        controlSystem.update(this->entityManager, this->componentManager, *this->inputManager, *this->player);

    chunkgenSystem.update(this->entityManager, this->componentManager, this->worldManager);

}
// Polls event from SFML, sends it to ImGui ProcessEvent() and then to inputManager->update()
// Also checks for Event::Closed and returns false if so, otherwise returns true; 
bool Game::handleEventQueue() {
    while (const std::optional event = this->renderWindow->pollEvent()) {
        // Event::Closed
        if (event->is<sf::Event::Closed>()) {
            this->worldManager.saveMapToTMX(gamefilepath, this->entityManager, this->componentManager);
            this->renderWindow->close();
            return false;//<-- escape loop and send exit signal
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
            break;//<-- break (upon sf::Event::Closed)

        // 3. ImGui
        ImGui::SFML::Update(*this->renderWindow, frameStartTime);     // Update imgui

        // 4.  Update systems (incl. imgui system[!] )
        this->updateSystems(deltaTime);                               // Update systems

        // 5.  Render
        this->renderWindow->clear();
        this->renderWindow->setView(player->playerView);         // Set view for world rendering
        this->render();                                          // Render all game tiles, entities and minimap

        ImGui::SFML::Render(*this->renderWindow);                // imgui
        this->minimapSystem.update(                              // minimap
            *this->renderWindow, 
            *this->player, 
            this->worldManager);
        this->renderWindow->display();                           // display

        this->inputManager->reset();                             // reset inputManager

        // 6.  FPS mangement (currently unused eg. no frame cap applied)
        sf::Time frameTime = clock.getElapsedTime();
        if (frameTime < targetFrameTime) {
            sf::sleep(targetFrameTime - frameTime);
        }

    } // while loop
    ImGui::SFML::Shutdown();
}

void Game::render() {
    this->renderWindow->clear(BLUE_COLOR);  // blue background

    this->worldManager.render(*this->renderWindow); // render all background tiles (all chunks)
    renderSystem.update(*renderWindow, this->allocator, this->entityManager, this->componentManager);

    this->renderWindow->setView(this->renderWindow->getDefaultView());  // escape playerView
    // isFollowing/freecam
    drawString(this->renderWindow, this->allocator, this->player->isFollowingUnit() ? "CONTROLLING" : "FREE CAM");
    
}

int Game::run() {
    this->initializeGame();
    this->gameLoop();  // keeps running until RenderWindow closes
    return 0;
}