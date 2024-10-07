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
    this->world.createChunkSprites(this->allocator);

    this->systems.push_back(std::make_unique<MovementSystem>());
    this->systems.push_back(std::make_unique<AnimationSystem>());
    this->systems.push_back(std::make_unique<CollisionSystem>());
    this->systems.push_back(std::make_unique<ObjectiveSystem>());

    // TEMP
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

        if (!handle_event(event, this->inputManager, this->player))
            ren.close();

        /*
        // select units
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            bool someEntityIsBeingHovered = false;
            // iterates through all entites and checks if any are currently being hovered
            for (auto& entity : entities) { 
                if ((someEntityIsBeingHovered = isEntityHovered(window, entity)))
                    break;

            }
            if (someEntityIsBeingHovered) { // if any entity was clicked
                selectUnits(window, playerp, entities);
            } 
            else { // if no entities are being hovered, add mouse position
                playerp->addObjectiveToSelectedUnits(sf::Vector2i(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x, window.mapPixelToCoords(sf::Mouse::getPosition(window)).y));
            }
            
        }
        // deselect units
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
            deselectUnits(window, playerp, entities);
        }

        // Let handle_event function (defined in input.hpp) handle the event
        else {
            if (!handle_event(event, inputhandler, playerp)) {
                //world1.saveMapToTMX(game_path);
                window.close();
            }
        } */
    }
}

void Game::gameLoop(sf::RenderWindow &ren) {
    while (ren.isOpen()) {
        sf::Time frameStartTime = this->clock.restart();
        float deltaTime = frameStartTime.asSeconds();

        this->handleEvent(ren);
        this->inputManager->update(deltaTime);

        // update entities in EntityManager

        this->updateSystems(deltaTime);

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