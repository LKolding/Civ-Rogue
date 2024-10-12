#include "Game.hpp"

std::string GAME_PATH = "../assets/tmx/maps/grass_chunk.tmx";
const sf::Time targetFrameTime = sf::seconds(1.f / 60); // 60 fps

struct CommonValues {
    const sf::FloatRect startViewRect = sf::FloatRect(sf::Vector2f(100.0f, 100.0f), sf::Vector2f(WINDOW_WIDTH*0.7, WINDOW_HEIGHT*0.7));
} commonValues;

// Game class

// this function gets called by this class' own constructor
void Game::initialize() {
    this->allocator = std::make_shared<ResourceAllocator>();    // init allocator
    this->allocator->initialize();
    this->gameState.state = GameState::State::MENU;             // start in menu
}

Game::Game() {
    this->initialize();
    this->entityManager.setAllocator(this->allocator);

    this->player = std::make_shared<Player>();
    this->inputManager = std::make_shared<InputManager>();

}

void Game::initializeGame(std::string &gameFilePath, sf::RenderWindow &ren) {
    this->player = std::make_shared<Player>();                  // init player
    this->player->playerView.reset(commonValues.startViewRect); // init player view
    ren.setView(this->player->playerView);

    this->world.initialize(this->allocator, this->player, gameFilePath);
    this->world.generateRandomChunk(sf::Vector2f(0,0));
    this->world.createChunkSprites(this->allocator);

    //this->entityManager.createBorderEntities(this->allocator, sf::Vector2i(0,0));

    this->systems.push_back(std::make_unique<MovementSystem>());
    this->systems.push_back(std::make_unique<CollisionSystem>());
    this->systems.push_back(std::make_unique<AnimationSystem>());
    this->systems.push_back(std::make_unique<ObjectiveSystem>());
    this->systems.push_back(std::make_unique<LifetimeSystem>());

    this->entityManager.addEntity(buildNinja(allocator));
    this->entityManager.addEntity(buildNinja(allocator));
    this->entityManager.addEntity(buildEyeBug(allocator));
    this->entityManager.addEntity(buildWell(allocator, 400, 400));

    // TEMP BUTTON FUNCTIONALITY
    /*
    this->buttons_vectorp.push_back(buildButton(allocator, ((CHUNK_WIDTH*TILE_WIDTH)/2), -100));                     // top
    this->buttons_vectorp.push_back(buildButton(allocator, ((CHUNK_WIDTH*TILE_WIDTH)/2), CHUNK_HEIGHT*TILE_HEIGHT)); // bot
    this->buttons_vectorp.push_back(buildButton(allocator, CHUNK_WIDTH*TILE_WIDTH, (CHUNK_WIDTH*TILE_WIDTH)/2));     // right
    this->buttons_vectorp.push_back(buildButton(allocator, -100, (CHUNK_WIDTH*TILE_WIDTH)/2));                       // left
    */
}

void Game::updateSystems(float deltaTime) {
    static float collisionDetectionTimer = 0.0f;
    for (auto &system : this->systems) {
        if (dynamic_cast<CollisionSystem*>(system.get())) {  // check if collision system
            collisionDetectionTimer += deltaTime;
            if (collisionDetectionTimer >= 0.06f) {
                system->update(deltaTime, this->entityManager.getAllEntities());// update on interval
                collisionDetectionTimer = 0.0f;
            }
        } else {  // if system is any other system than collision, simply update
            system->update(deltaTime, this->entityManager.getAllEntities());
        }
    }
}
//  Defines and declares sf::Event and processes it
void Game::handleEvent(sf::RenderWindow &ren) {
    sf::Event event;
    while (ren.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            ren.close();

        // Follow Entity that is being hovered on Spacebar
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
            if (this->player->isFollowingUnit()) {
                this->player->stopFollow();
            } else {
                followEntity(ren, this->player, entityManager.getAllEntities());
            }
        }
        //  LEFT MOUSE BTN
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            const auto mouse_pos = sf::Vector2i(ren.mapPixelToCoords(sf::Mouse::getPosition(ren)).x, ren.mapPixelToCoords(sf::Mouse::getPosition(ren)).y);
            // Check for button
            for (auto btn : this->buttons_vectorp) {
                if (isEntityHovered(ren, btn)) {
                    std::static_pointer_cast<ButtonEntity>(btn)->click();
                    continue;
                }
            }
            bool someEntityIsBeingHovered = false;
            // iterates through all entites and checks if any are currently being hovered
            for (auto entity : this->entityManager.getAllEntities()) { 
                if ((someEntityIsBeingHovered = isEntityHovered(ren, entity)))
                    break;
            }
            if (someEntityIsBeingHovered) { // if any entity was hovered while click happened
                /*  // KILLS ALL ENTITIES BELOW
                auto worldCoords = ren.mapPixelToCoords(sf::Mouse::getPosition(ren));
                for (auto entity_p : this->entityManager.getAllEntities()) {
                    if (auto entity = entity_p.lock()) {
                        auto colli_ptr = entity->getComponent<CollisionComponent>();
                        if (colli_ptr->bounds.contains(worldCoords)) {
                            this->entityManager.killEntity(entity->getComponent<UUIDComponent>()->ID);
                        }
                    }
                }*/
                selectUnits(ren, this->player, entityManager.getAllEntities());
            }
            else { // if no entities are being hovered, add mouse position
                this->player->addObjectiveToSelectedUnits(mouse_pos);
                this->entityManager.addEntity(buildBluePointer(this->allocator, mouse_pos.x, mouse_pos.y));
                //this->entityManager.addEntity(buildArrow(this->allocator, mouse_pos.x, mouse_pos.y));
            }
            
        }
        // deselect units
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
            deselectUnits(ren, this->player, entityManager.getAllEntities());
        }
        // Let handle_event function (defined in input.hpp) handle the event
        else {
            if (!handle_event(event, this->inputManager)) {
                //this->world.saveMapToTMX(GAME_PATH);
                ren.close();
            }
        } 
    }
}

void Game::gameLoop(sf::RenderWindow &ren) {
    while (ren.isOpen()) {

        sf::Time frameStartTime = this->clock.restart();
        float deltaTime = frameStartTime.asSeconds();

        this->handleEvent(ren);
        this->inputManager->update(deltaTime);

        for (auto& btn : this->buttons_vectorp) {
            btn->update(deltaTime);
        }

        this->updateSystems(deltaTime);
        this->entityManager.update(deltaTime);

        this->player->update(deltaTime, this->inputManager);

        ren.setView(this->player->playerView);
        ren.clear();

        this->render(ren);
        ren.setView(ren.getDefaultView());
        // draw ui
        ren.setView(this->player->playerView);
        ren.display();

        sf::Time frameTime = clock.getElapsedTime();
        if (frameTime < targetFrameTime) {
            sf::sleep(targetFrameTime - frameTime);
        }
    }
}

void Game::render(sf::RenderWindow &ren) {
    this->world.render(ren); // render all background tiles (all chunks)
    this->renderSystem->update(ren, this->entityManager.getAllEntities());
    this->renderUI(ren);
}

void Game::renderUI(sf::RenderWindow &ren) {
    for (auto &btn : this->buttons_vectorp) {
        ren.draw(btn->getComponent<SpriteComponent>()->sprite);
    }
}

// public methods
int Game::run() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "klunkabadul");

    this->initializeGame(GAME_PATH, window); // TODO 
    this->gameLoop(window);
    /*
    while (true) {
        switch(this->gameState.state) {
            case GameState::INGAME:
                // go ingame; load world, player etc.
                // and then call this->gameLoop();
                this->initializeGame(GAME_PATH, window); // TODO 
                this->gameLoop(window);
                break;

            case GameState::MENU:
                // load menu
                // if game quit == break out of both the switch and while loop
                break;

            default:
                break;

        }

    }*/

    return 0;

}