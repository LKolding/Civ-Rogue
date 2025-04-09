#include "Game.hpp"


const sf::Time targetFrameTime = sf::seconds(1.f / 60);  // 60 fps (unused)


Game::Game() {
    this->inputManager = std::make_unique<InputManager>();
    this->allocator = std::make_shared<ResourceAllocator>();

    this->renderWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode({static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT)}), "klunkabadul");
    /*ContextSettings
    Parameters:
    depth – Depth buffer bits
    stencil – Stencil buffer bits
    antialiasing – Antialiasing level
    major – Major number of the context version
    minor – Minor number of the context version
    attributes – Attribute flags of the context
    sRgb – sRGB capable framebuffer
    */
}

void Game::initializeGame() {
    //  init player
    this->player = std::make_shared<Player>();
    //  read game file and initialize world
    std::string gameFileName = "../saveGames/game1.tmx";
    this->world.initialize(this->allocator, this->player, gameFileName);

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
    
}

void Game::updateSystems(float deltaTime) {
    animationSystem.update(deltaTime, this->entityManager, this->componentManager);
    cursorSystem.update(*this->renderWindow, this->entityManager, this->componentManager);
    viewpanSystem.update(deltaTime, this->player->playerView, *this->inputManager);
    collisionSystem.update(deltaTime, this->entityManager, this->componentManager);
    //movementSystem.update(deltaTime, this->entityManager.getAllEntities());
    //interactionSystem.update(this->entityManager.getAllEntities(), this->inputManager, this->renderWindow, this->player);

}
//  Defines and declares sf::Event. Then polls and processes it
void Game::handleEventQueue() {
    while (const std::optional event = this->renderWindow->pollEvent()) {

        if (event->is<sf::Event::Closed>()) {
            this->renderWindow->close();
        }

        //  Render arrow at position of mouse button click
        // else if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
        //     sf::Vector2i mousePos = sf::Mouse::getPosition(*this->renderWindow);
        //     this->entityManager.renderArrow(this->renderWindow->mapPixelToCoords(mousePos));
        // }

        //  Follow/unfollow
        /*
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
            if (player->isFollowingUnit()) {
                player->stopFollow();
                continue;
            }*/

            // for (auto& ent : this->entityManager.getAllEntities()) {
            //     if (ent.lock()->hasComponent<FollowComponent>() && isEntityHovered(this->renderWindow, ent)) {
            //         player->followUnit(ent.lock());
            //         return;
            //     }
            // }

        //  Lets inputManager take care of catching which key 
        //  event it is and update the lookup table accordingly
        if (event)
            this->inputManager->update(event.__get());
    }
}

void Game::gameLoop() {
    while (this->renderWindow->isOpen()) {
        // 1.  Frame start: Calculate deltaTime
        sf::Time frameStartTime = clock.restart();
        const float deltaTime = frameStartTime.asSeconds();

        // 2.  Input
        this->handleEventQueue();                                // Handle SFML event(s)

        // 3.  Update systems, player & entities
        this->updateSystems(deltaTime);                          // Update ECS systems
        //this->player->update(deltaTime, inputManager);           // Update player logic
        //this->entityManager.update(deltaTime);                   // Update all entities

        // 4.  Render
        this->renderWindow->clear();
        this->renderWindow->setView(player->playerView);         // Set view for world rendering
        this->render();                                          // Render all game tiles & entities
        
        // temp (UI/debug stuff)
        this->renderWindow->setView(this->renderWindow->getDefaultView());  // escape playerView

        std::string following = this->player->isFollowingUnit() ? "controlling character" : "free cam";
        drawString(this->renderWindow, this->allocator, following);

        std::string mouse_coordinates = std::to_string(this->cursorSystem.mouse_pos.x) + ", "+ std::to_string(this->cursorSystem.mouse_pos.y);
        drawString(this->renderWindow, this->allocator, mouse_coordinates, 50);

        int entitiesAmount = this->entityManager.getAllEntities().size();
        drawString(this->renderWindow, this->allocator, "Entities: " + std::to_string(entitiesAmount), 25);

        this->renderWindow->setView(player->playerView);  // reset view
        this->renderWindow->display();

        // 5.  FPS mangement (currently unused eg. no frame cap applied)
        sf::Time frameTime = clock.getElapsedTime();
        if (frameTime < targetFrameTime) {
            //sf::sleep(targetFrameTime - frameTime);
        }
    }
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