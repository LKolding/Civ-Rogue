#include "Game.hpp"


const sf::Time targetFrameTime = sf::seconds(1.f / 60);  // 60 fps (unused)
const char* gamefilepath = "../saveGames/game1.tmx"; 

Game::Game() {
    this->inputManager = std::make_unique<InputManager>();
    this->allocator = std::make_shared<ResourceAllocator>();
    this->renderWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode({static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT)}), "klunkabadul");
    ImGui::SFML::Init(*this->renderWindow);
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
    
    buildNinja(this->componentManager, this->entityManager);  // Create ninja
    buildNinja(this->componentManager, this->entityManager);  // Create ninja
    buildNinja(this->componentManager, this->entityManager);  // Create ninja
    buildNinja(this->componentManager, this->entityManager);  // Create ninja
    buildNinja(this->componentManager, this->entityManager);  // Create ninja
    buildNinja(this->componentManager, this->entityManager);  // Create ninja
    buildNinja(this->componentManager, this->entityManager);  // Create ninja
    buildNinja(this->componentManager, this->entityManager);  // Create ninja
    
}

void Game::updateSystems(float deltaTime) {
    animationSystem.update(deltaTime, this->entityManager, this->componentManager);
    cursorSystem.update(*this->renderWindow, this->entityManager, this->componentManager);
    viewpanSystem.update(deltaTime, this->player->playerView, *this->inputManager);
    collisionSystem.update(deltaTime, this->entityManager, this->componentManager);
    inputSystem.update(*this->inputManager, this->componentManager);
    velocitySystem.update(deltaTime, this->entityManager, this->componentManager);
    objectiveSystem.update(this->entityManager, this->componentManager);
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
        this->handleEventQueue();                                // Handle SFML event(s)

        // 3. ImGui
        // mouse coordinates
        const std::string mouse_coordinates = "(monitor)  " + std::to_string(this->cursorSystem.mouse_pos.x) + ", "+ std::to_string(this->cursorSystem.mouse_pos.y);
        const std::string mouse_coordinates2= "(world)    " + std::to_string((int)this->cursorSystem.world_pos.x) + ", "+ std::to_string((int)this->cursorSystem.world_pos.y);
        const std::string entities_amount   = "(entities) " + std::to_string(this->entityManager.getAllEntities().size());

        ImGui::SFML::Update(*this->renderWindow, frameStartTime);

        ImGui::Begin("Debug");
        // ImGui::Button("Look at this pretty button");

        ImGui::TextColored({0.0, 0.5, 1.0, 1.0}, "Information:");
        ImGui::BeginChild("Scrolling");
        ImGui::Text("            mouse x,y");
        ImGui::Text(mouse_coordinates.c_str());
        ImGui::Text(mouse_coordinates2.c_str());
        ImGui::Text(entities_amount.c_str());
        ImGui::EndChild();
        
        ImGui::End();

        // 4.  Update systems
        this->updateSystems(deltaTime);

        // 5.  Render
        this->renderWindow->clear();
        this->renderWindow->setView(player->playerView);         // Set view for world rendering
        this->render();                                          // Render all game tiles & entities
        
        //imgui
        ImGui::SFML::Render(*this->renderWindow);

        // ------------------------------------
        // ----- // Temp (UI/debug stuff) -----
        // ------------------------------------
        this->renderWindow->setView(this->renderWindow->getDefaultView());  // escape playerView
        // isFollowing/freecam
        std::string following = this->player->isFollowingUnit() ? "controlling character" : "free cam";
        drawString(this->renderWindow, this->allocator, following);

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
}

int Game::run() {
    this->initializeGame();
    this->gameLoop();  // keeps running until RenderWindow closes

    return 0;

}