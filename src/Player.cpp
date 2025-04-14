#include "Player.hpp"

Player::Player() {
    //  init player view
    // this->playerView.reset(sf::FloatRect(sf::Vector2f(0,0), sf::Vector2f(viewMaxX, viewMaxY))); 
    this->playerView.setSize({viewMaxX, viewMaxY});
}

void Player::stopFollow() {
    this->entityFollow = 0;
}

void Player::followUnit(EntityID id) {
    if (!this->entityFollow)
        this->entityFollow = id;
}

bool Player::isFollowingUnit() {
    if (this->entityFollow != 0)
        return true;
    else
        return false;
    
}
