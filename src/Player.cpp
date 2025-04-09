#include "Player.hpp"

Player::Player() {
    //  init player view
    // this->playerView.reset(sf::FloatRect(sf::Vector2f(0,0), sf::Vector2f(viewMaxX, viewMaxY))); 
    this->playerView.setSize({viewMaxX, viewMaxY});
}

void Player::stopFollow() {
    this->entityID = 0;
}

void Player::followUnit(EntityID id) {
    if (!this->entityID)
        this->entityID = id;
}

bool Player::isFollowingUnit() {
    if (this->entityID != 0)
        return true;
    else
        return false;
    
}
