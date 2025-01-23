#include "Game.hpp"

const sf::Time targetFrameTime = sf::seconds(1.f / 60); // 60 fps

struct CommonValues { // starting view rect values
    const sf::FloatRect startViewRect = sf::FloatRect(sf::Vector2f(100.0f, 100.0f), sf::Vector2f(WINDOW_WIDTH*0.7, WINDOW_HEIGHT*0.7));
} commonValues;

Game::Game() {
    this->inputManager = std::make_unique<InputManager>();
    this->allocator = std::make_shared<ResourceAllocator>();

    this->movementSystem = std::make_unique<MovementSystem>();
    this->animationSystem= std::make_unique<AnimationSystem>();
    this->collisionSystem= std::make_unique<CollisionSystem>();

    this->entityManager.setAllocator(this->allocator);

    this->renderWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "klunkabadul");
}

void Game::initializeGame() {
    this->player = std::make_shared<Player>();                  // init player
    this->player->playerView.reset(commonValues.startViewRect); // init player view

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

    //  Generate sprites for all chunks
    this->world.createChunkSprites(this->allocator);
    
    auto weapon = buildWeapon(this->allocator, "Elven sword.png"); // Create weapon
    auto ninja  = buildNinja(allocator);    // Create ninja
        
    ninja->setEquippedWeapon(weapon);      // Set weapon

    this->entityManager.addEntity(weapon); // Register weapon
    this->entityManager.addEntity(ninja);  // Register Ninja

    this->entityManager.addEntity(buildTree(allocator));

}

void Game::updateSystems(float deltaTime) {
    this->animationSystem->update(deltaTime, this->entityManager.getAllEntities());
    this->collisionSystem->update(deltaTime, this->entityManager.getAllEntities());
    this->movementSystem->update(deltaTime, this->entityManager.getAllEntities());
    this->interactionSystem->update(this->entityManager.getAllEntities(), sf::Mouse::getPosition(*this->renderWindow));
}
//  Defines and declares sf::Event. Then polls and processes it
void Game::handleEventQueue() {
    sf::Event event;

    while (this->renderWindow->pollEvent(event)) {

        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            this->renderWindow->close();

        else
            //  Lets inputManager take care of catching which key 
            //  event it is and update the lookup table accordingly
            this->inputManager->update(event);

    }
}

void Game::gameLoop() {
    sf::Vector2f mousePos;
    while (this->renderWindow->isOpen()) {
        // 1.  Frame Start: Calculate deltaTime
        sf::Time frameStartTime = clock.restart();
        const float deltaTime = frameStartTime.asSeconds();

        // 2.  Input
        this->handleEventQueue();                                // Handle SFML event(s)

        // 3.  Update systems, player & entities
        this->updateSystems(deltaTime);                          // Update ECS systems
        this->player->update(deltaTime, inputManager, mousePos); // Update player logic
        this->entityManager.update(deltaTime);                   // Update all entities

        // 4.  Render
        this->renderWindow->clear();

        this->renderWindow->setView(player->playerView);         // Set view for world rendering
        this->render();                                          // Render all game tiles & entities
        
        this->renderWindow->display();

        // 5.  FPS mangement
        sf::Time frameTime = clock.getElapsedTime();
        if (frameTime < targetFrameTime) {
            sf::sleep(targetFrameTime - frameTime);
        }
    }
}

void Game::render() {
    this->world.render(this->renderWindow); // render all background tiles (all chunks)
    this->renderSystem->update(this->renderWindow, this->entityManager.getAllEntities());
    
}


int Game::run() {

    this->initializeGame();
    this->gameLoop();  // keeps running until RenderWindow "window" closes

    return 0;

}