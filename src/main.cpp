#include <SFML/Graphics.hpp>

// system
#include <cmath>
#include <iostream>

// My stuff
#include "ResourceAllocator.hpp"
#include "World.hpp"
#include "Player.hpp"

#include "EntityFactory.hpp"
// Entities
#include "ECS/Entities/Entity.hpp"
#include "ECS/Entities/Human.hpp"
// Systems
#include "ECS/Systems/MovementSystem.hpp"
#include "ECS/Systems/RenderSystem.hpp"
#include "ECS/Systems/AnimationSystem.hpp"
#include "ECS/Systems/CollisionSystem.hpp"
#include "ECS/Systems/ObjectiveSystem.hpp"

#include "input.hpp"

#include "coordinate_calculators.hpp"


void renderSelectionBox(sf::RenderWindow &ren) {
    //  renders a rectangle on top of the tile that the cursor
    //  is currently hovering above
    sf::Vector2f worldCoords = ren.mapPixelToCoords(sf::Mouse::getPosition(ren));

    sf::RectangleShape rect1(sf::Vector2f(TILE_WIDTH/2, TILE_HEIGHT/2));
    
    sf::Vector2f rectPos;
    // offset position of rectangle based on chunk
    rectPos.x = getChunkCoords(worldCoords).x * CHUNK_WIDTH * (TILE_WIDTH / 2);  
    rectPos.y = getChunkCoords(worldCoords).y * CHUNK_HEIGHT * (TILE_HEIGHT / 2);
    // offset position of rectangle based on tile (within chunk)
    rectPos.x+= getTileIndex(worldCoords).x * (TILE_WIDTH / 2);
    rectPos.y+= getTileIndex(worldCoords).y * (TILE_HEIGHT / 2);

    rect1.setPosition(rectPos);
    rect1.setOutlineColor(sf::Color::Black);
    rect1.setFillColor(sf::Color::Transparent);
    rect1.setOutlineThickness(1.5);
    rect1.setScale(0.9f, 0.9f);

    ren.draw(rect1);
}

bool isEntityHovered(sf::RenderWindow &ren, std::shared_ptr<Entity>& entity) {
    // Returns true if the entity's CollisionComponents boundingbox is currently
    // below sf::Mouse::getPosition()
    if (entity->hasComponent<DeletableComponent>()) {
        return false; // skip if entity is a healthbar (only entity containing Deletable)
    }
    sf::Vector2f worldCoords = ren.mapPixelToCoords(sf::Mouse::getPosition(ren));
    auto colli_ptr = entity->getComponent<CollisionComponent>();
    if (colli_ptr->bounds.contains(worldCoords.x, worldCoords.y)) {
        return true;
    }
    return false;
}

void selectUnits(sf::RenderWindow &ren, std::shared_ptr<Player> player, std::vector<std::shared_ptr<Entity>>& entities) {
    for (auto& entity : entities) {
        if (entity->hasComponent<CollisionComponent>() && isEntityHovered(ren, entity) && entity->hasComponent<SelectableComponent>()) {
            entity->getComponent<SelectableComponent>()->isSelected = true;
            player->selectUnit(entity);
        }
    }
}

void deselectUnits(sf::RenderWindow &ren, std::shared_ptr<Player> player, std::vector<std::shared_ptr<Entity>>& entities) {
    for (auto& entity : entities) {
        for (auto& entity : entities) {
            if (entity->hasComponent<CollisionComponent>() && isEntityHovered(ren, entity) && entity->hasComponent<SelectableComponent>()) {
                entity->getComponent<SelectableComponent>()->isSelected = false;
                player->deselectUnit(entity);
            }
        }
        
    }
}


std::string game_path = "../assets/tmx/maps/grass_chunk.tmx";
// std::string game_path = "../saveGames/game1.tmx";

int main() {
    const unsigned int targetFPS = 60;
    const sf::Time targetFrameTime = sf::seconds(1.f / targetFPS); // Time per frame
    sf::Clock clock;  // To measure the frame time

    // window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Civ_Rogue");
    //  player and view
    auto playerp = std::make_shared<Player>();
    playerp->playerView.reset(sf::FloatRect(sf::Vector2f(100.f, 100.f), sf::Vector2f(WINDOW_WIDTH*0.7, WINDOW_HEIGHT*0.7)));
    window.setView(playerp->playerView);
    //  allocator and world
    ResourceAllocator allocator;
    World world1;
    world1.initialize(allocator, playerp, game_path);
    sf::Vector2f chunk1pos = sf::Vector2f(0,0);
    sf::Vector2f chunk2pos = sf::Vector2f(1,0);
    sf::Vector2f chunk3pos = sf::Vector2f(0,1);
    sf::Vector2f chunk4pos = sf::Vector2f(1,1);
    world1.generateRandomChunk(chunk1pos);
    world1.generateRandomChunk(chunk2pos);
    world1.generateRandomChunk(chunk3pos);
    world1.generateRandomChunk(chunk4pos);
    //  input handler ( a bit hacky due to the fact that sfml behaves weirdly on m1 mac)
    InputManager inputhandler(playerp);
    //  create sprites for chunks
    world1.createChunkSprites(allocator);
    //  entities
    std::vector<std::shared_ptr<Entity>> entities;

    auto ninja = buildNinja(allocator);
    auto ninja_hp = buildHealthbar(allocator, ninja);
    entities.push_back(std::move(ninja));
    entities.push_back(std::move(ninja_hp));

    //  systems
    MovementSystem  movementSystem;
    RenderSystem    renderSystem;
    AnimationSystem animationSystem;
    CollisionSystem collisionSystem;
    ObjectiveSystem objectiveSystem;

    float deltaTime = 1.0f;
    while (window.isOpen()) {
        // Restart the clock at the beginning of the frame
        sf::Time frameStartTime = clock.restart();
        deltaTime = frameStartTime.asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Mouse events

            // select units
            
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
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
            } 
        }

        // Important
        inputhandler.update(frameStartTime.asMicroseconds());

        // Update entities
        for (const auto& entity : entities) {
            entity->update(deltaTime); 
        }
        movementSystem.update(deltaTime, entities);
        animationSystem.update(deltaTime, entities);
        collisionSystem.update(deltaTime, entities);
        objectiveSystem.update(deltaTime, entities);

        window.setView(playerp->playerView);
        window.clear();
        world1.render(window);
        renderSystem.update(window, entities);
        //renderSelectionBox(window);
        window.display();

        // Measure the time it took to complete the frame
        sf::Time frameTime = clock.getElapsedTime();
        // If the frame was completed too quickly, sleep to cap the FPS
        if (frameTime < targetFrameTime) {
            sf::sleep(targetFrameTime - frameTime);
        }
    }
    return 0;
}
