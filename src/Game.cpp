#include "Game.hpp"

std::string GAME_PATH = "../saveGames/game1.tmx";
const sf::Time targetFrameTime = sf::seconds(1.f / 60); // 60 fps

struct CommonValues {
    const sf::FloatRect startViewRect = sf::FloatRect(sf::Vector2f(100.0f, 100.0f), sf::Vector2f(WINDOW_WIDTH*0.7, WINDOW_HEIGHT*0.7));
    // 
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

    this->inputManager->start(this->player);
}

void Game::initializeGame(std::string &gameFilePath, sf::RenderWindow &ren) {
    this->player = std::make_shared<Player>();                  // init player
    this->player->playerView.reset(commonValues.startViewRect); // init player view
    ren.setView(this->player->playerView);

    this->world.initialize(this->allocator, this->player, gameFilePath);
    this->world.generateRandomChunk(sf::Vector2f(0,0));
    this->world.createChunkSprites(this->allocator);

    this->entityManager.createBorderEntities(this->allocator, sf::Vector2i(0,0));

    this->systems.push_back(std::make_unique<MovementSystem>());
    this->systems.push_back(std::make_unique<AnimationSystem>());
    this->systems.push_back(std::make_unique<CollisionSystem>());
    this->systems.push_back(std::make_unique<ObjectiveSystem>());

    // TEMP
    this->buttons_vectorp.push_back(buildButton(allocator, (CHUNK_WIDTH*TILE_WIDTH)/2, -100));
    this->buttons_vectorp.push_back(buildButton(allocator, (CHUNK_WIDTH*TILE_WIDTH)/2, CHUNK_HEIGHT*TILE_HEIGHT));
    this->buttons_vectorp.push_back(buildButton(allocator, CHUNK_WIDTH*TILE_WIDTH, (CHUNK_WIDTH*TILE_WIDTH)/2));
    this->buttons_vectorp.push_back(buildButton(allocator, -100, (CHUNK_WIDTH*TILE_WIDTH)/2));

}

void Game::updateSystems(float deltaTime) {
    for (auto &system : this->systems) {
        system->update(deltaTime, this->entityManager.getAllEntities());
    }
}

bool Game::handleEvent(sf::RenderWindow &ren) {
    sf::Event event;
    while (ren.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            ren.close();

        // select units
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            bool someEntityIsBeingHovered = false;
            // iterates through all entites and checks if any are currently being hovered
            for (auto entity : this->entityManager.getAllEntities()) { 
                if ((someEntityIsBeingHovered = isEntityHovered(ren, entity)))
                    break;

            }
            if (someEntityIsBeingHovered) { // if any entity was clicked
                selectUnits(ren, this->player, entityManager.getAllEntities());
            } 
            else { // if no entities are being hovered, add mouse position
                this->player->addObjectiveToSelectedUnits(sf::Vector2i(ren.mapPixelToCoords(sf::Mouse::getPosition(ren)).x, ren.mapPixelToCoords(sf::Mouse::getPosition(ren)).y));
            }
            
        }
        // deselect units
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
            deselectUnits(ren, this->player, entityManager.getAllEntities());
        }

        // Let handle_event function (defined in input.hpp) handle the event
        else {
            if (!handle_event(event, this->inputManager, this->player)) {
                this->world.saveMapToTMX(GAME_PATH);
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

        ren.setView(this->player->playerView);
        ren.clear();
        // render
        this->world.render(ren);
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