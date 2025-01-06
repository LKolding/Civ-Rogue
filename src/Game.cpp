#include "Game.hpp"

std::string GAME_PATH = "../assets/tmx/maps/grass_chunk.tmx";
const sf::Time targetFrameTime = sf::seconds(1.f / 60); // 60 fps

struct CommonValues { // starting view rect values
    const sf::FloatRect startViewRect = sf::FloatRect(sf::Vector2f(100.0f, 100.0f), sf::Vector2f(WINDOW_WIDTH*0.7, WINDOW_HEIGHT*0.7));
} commonValues;

Game::Game() {
    this->allocator = std::make_shared<ResourceAllocator>();
    this->allocator->initialize();
    this->entityManager.setAllocator(this->allocator);

    this->player = std::make_shared<Player>();
    this->inputManager = std::make_shared<InputManager>();
}

void Game::initializeGame(std::string &gameFilePath, sf::RenderWindow &ren) {
    this->player = std::make_shared<Player>();                  // init player
    this->player->playerView.reset(commonValues.startViewRect); // init player view

    this->world.initialize(this->allocator, this->player, gameFilePath);
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

    //  Generate sprites for all chunks
    this->world.createChunkSprites(this->allocator);

    this->systems.push_back(std::make_unique<MovementSystem>());
    this->systems.push_back(std::make_unique<CollisionSystem>());
    this->systems.push_back(std::make_unique<AnimationSystem>());
    this->systems.push_back(std::make_unique<LifetimeSystem>());

    
    auto weapon = buildWeapon(this->allocator, "Elven sword.png"); // Create weapon
    auto ninja  = buildNinja(allocator);    // Create ninja
        
    ninja->setEquippedWeapon(weapon);      // Set weapon

    this->entityManager.addEntity(weapon); // Register weapon
    this->entityManager.addEntity(ninja);  // Register Ninja

    this->entityManager.addEntity(buildTree(allocator));

}

void Game::updateSystems(float deltaTime) {
    for (auto &system : this->systems) {
        system->update(deltaTime, this->entityManager.getAllEntities());
        
    }
}
//  Defines and declares sf::Event. Then polls and processes it
void Game::handleEvent(sf::RenderWindow &ren) {
    sf::Event event;
    while (ren.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            ren.close();

        performEventBehavior(event, ren, this->player, this->entityManager, this->allocator);
        // Let handle_event function (defined in input.hpp) handle the event
        if (!handle_event(event, this->inputManager)) {
            //this->world.saveMapToTMX(GAME_PATH);
            ren.close();
        }
    }
}

void Game::gameLoop(sf::RenderWindow &ren) {
    while (ren.isOpen()) {
        // FPS
        sf::Time frameStartTime = this->clock.restart();
        float deltaTime = frameStartTime.asSeconds();
        // Set playerView
        ren.setView(this->player->playerView);

        // Handle sf::Event
        this->handleEvent(ren);
        // Update input
        this->inputManager->update(deltaTime);
        // Update player
        this->player->update(deltaTime, this->inputManager);
        // Interation check
        auto mousePos = ren.mapPixelToCoords(sf::Mouse::getPosition(ren));
        checkForInteraction(mousePos, this->entityManager);

        this->updateSystems(deltaTime);         // updates all systems (but renderSystem)
        this->entityManager.update(deltaTime);

        //  Render
        ren.setView(this->player->playerView);
        ren.clear();
        this->render(ren);                      // calls renderSystem.update()
        // UI
        ren.setView(ren.getDefaultView());      // change view
        this->renderUI(ren);                    // render ui
        ren.setView(this->player->playerView);  // reset view

        ren.display();
        // calculate fps
        sf::Time frameTime = clock.getElapsedTime();
        if (frameTime < targetFrameTime) {
            sf::sleep(targetFrameTime - frameTime);
        }
    }
}

void Game::render(sf::RenderWindow &ren) {
    this->world.render(ren); // render all background tiles (all chunks)
    this->renderSystem->update(ren, this->entityManager.getAllEntities());
    // draw icons (technicially part of UI, but gets drawn in relation to global mouse_pos)
    for (auto &entp : this->entityManager.getAllIconEntities()) {
        if (auto ent = entp.lock()) {
            if (!ent->getComponent<SpriteComponent>()->isVisible)
                continue;
            ren.draw(ent->getComponent<SpriteComponent>()->sprite);
        }
    }
    
}

void Game::renderUI(sf::RenderWindow &ren) {
    for (auto &entp : this->entityManager.getAllUIEntities()) {
        if (auto ent = entp.lock()) {
            if (!ent->getComponent<SpriteComponent>()->isVisible)
                continue;
            ren.draw(ent->getComponent<SpriteComponent>()->sprite);
        }
    }
}

// public methods
int Game::run() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "klunkabadul");

    this->initializeGame(GAME_PATH, window);
    this->gameLoop(window);

    return 0;

}