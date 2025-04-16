#ifndef _PLAYER_
#define _PLAYER_

#include <SFML/Graphics.hpp>

#include "ECS/EntityManager.hpp"  // for EntityID

#include "constants.h"
#include "input.hpp"


struct Player {
public:
    std::optional<EntityID> entityFollow; // entity to follow
    sf::View playerView;
    sf::View minimapView;

    Player();

    void move(float x = 0, float y = 0);

    void stopFollow();
    void followUnit(EntityID id);
    bool isFollowingUnit();

};
#endif