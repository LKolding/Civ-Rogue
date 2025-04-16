#include "Player.hpp"


Player::Player() {
    //  init player view
    this->playerView.setSize({viewMaxX, viewMaxY});
    // minimap
    this->minimapView.setSize({viewMaxX, viewMaxY});
    this->minimapView.setViewport(sf::FloatRect({0.75f, 0.0f}, {0.25f, 0.25f}));
    this->minimapView.setCenter(this->playerView.getCenter());
    this->minimapView.zoom(3.0f);
}

// Moves both views (playerView and minimapView)
void Player::move(float x, float y) {
    this->playerView.move({x,y});
    this->minimapView.move({x,y});
}

void Player::stopFollow() {
    if (this->entityFollow.has_value())
        this->entityFollow.reset();
}

void Player::followUnit(EntityID id) {
    if (!this->entityFollow.has_value())
        this->entityFollow = id;
}

bool Player::isFollowingUnit() {
    if (this->entityFollow.has_value())
        return true;
    else
        return false;
    
}
