#ifndef _PLAYER_
#define _PLAYER_

#include <SFML/Graphics.hpp>

#include "ECS/Entities/Entity.hpp"
#include "ECS/Entities/Ninja.hpp"

#include "constants.h"
#include "input.hpp"


struct Player {
public:
    sf::View playerView;

    Player();

    void update(float deltaTime, std::unique_ptr<InputManager>& inputManager);
    // x and y simply indicate direction, and should be of
    // value 1.0, 0.0 or -1.0
    void move(float x, float y, float dt);

    void setPosition(float x = 0, float y = 0);
    sf::Vector2f getPosition();

    void selectUnit(std::shared_ptr<Entity> &entity);

    void deselectUnit(std::shared_ptr<Entity> &entity);

    void followUnit(std::shared_ptr<Entity> entity);
    void stopFollow();
    bool isFollowingUnit();

private:
 
    std::shared_ptr<Entity> entityp;
    const float moveSpeed = 400.0f; // View panning speed
    const int viewMinX = 1920/4;    // View minimum X length
    const int viewMaxX = 1920/2;    // View maximum X length
    const int viewMinY = 1080/4;    // View minimum Y length
    const int viewMaxY = 1080/2;    // View maximum Y length

};
#endif