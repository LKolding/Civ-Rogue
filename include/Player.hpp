#ifndef _PLAYER_
#define _PLAYER_

#include <SFML/Graphics.hpp>

#include "ECS/Entities/Entity.hpp"
#include "ECS/Entities/Ninja.hpp"

#include "input.hpp"


struct Player {
    static inline sf::View playerView;

    void update(float deltaTime, std::shared_ptr<InputManager> inputManager);
    // x and y simply indicate direction, and should be of
    // value 1.0, 0.0 or -1.0
    void move(float x, float y, float dt);

    void setPosition(float x = 0, float y = 0);
    sf::Vector2f getPosition();

    void selectUnit(std::shared_ptr<Entity> &entity);

    void deselectUnit(std::shared_ptr<Entity> &entity);

    void followUnit(std::shared_ptr<Entity>& entity);
    void stopFollow();
    bool isFollowingUnit();

    void addObjectiveToSelectedUnits(sf::Vector2i pos);

private:
    static inline sf::Vector2f pos;
    // holds the UUIDComponent's ID member of the Entity that the weak_ptr is pointing to
    static inline std::unordered_map<uint32_t, std::weak_ptr<Entity>> selectedEntities; 
    static inline std::weak_ptr<Entity> entityFollow;
    const static inline float moveSpeed = 400.0f; // View panning speed
    void updateView();

};
#endif