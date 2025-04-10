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

    void setPosition(float x = 0, float y = 0);
    sf::Vector2f getPosition();

    void stopFollow();
    void followUnit(EntityID id);
    bool isFollowingUnit();

private:
    EntityID entityID; // entity to follow

};
#endif