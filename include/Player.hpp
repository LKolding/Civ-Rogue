#ifndef _PLAYER_
#define _PLAYER_

#include <SFML/Graphics.hpp>

#include "ECS/EntityManager.hpp"  // for EntityID

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

    void selectUnit(EntityID id);

    void deselectUnit();

    void stopFollow();
    bool isFollowingUnit();

private:
 
    EntityID entityID; // entity to follow
    const float moveSpeed = 400.0f; // View panning speed
    const int viewMinX = 1920/4;    // View minimum X length
    const int viewMaxX = 1920/2;    // View maximum X length
    const int viewMinY = 1080/4;    // View minimum Y length
    const int viewMaxY = 1080/2;    // View maximum Y length

};
#endif