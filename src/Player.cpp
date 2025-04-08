#include "Player.hpp"

Player::Player() {
    //  init player view
    this->playerView.reset(sf::FloatRect(sf::Vector2f(0,0), sf::Vector2f(this->viewMaxX, this->viewMaxY))); 
}

// x and y simply indicate direction, and should be of
// value 1.0, 0.0 or -1.0
void Player::move(float x, float y, float dt) {
    const float MOVE_AMOUNT = this->moveSpeed * dt;

    float length = std::sqrt(x * x + y * y);

    if (length != 0.0f) {
        x /= length;
        y /= length;
    }

    this->playerView.move(sf::Vector2f(x * MOVE_AMOUNT, y * MOVE_AMOUNT));
}

void Player::update(float deltaTime, std::unique_ptr<InputManager>& inputManager) {
    // check for input
    float moveX = 0.0f;
    float moveY = 0.0f;
    bool lmclick = false;  // left mouse click
    bool rmclick = false;  // right mouse click
    bool spacebar= false;  // spacebar

    //  keyboard
    if (inputManager->keyState[sf::Keyboard::W]) { moveY = -1.0f; }  // up
    if (inputManager->keyState[sf::Keyboard::A]) { moveX = -1.0f; }  // left
    if (inputManager->keyState[sf::Keyboard::S]) { moveY =  1.0f; }  // down
    if (inputManager->keyState[sf::Keyboard::D]) { moveX =  1.0f; }  // right
    //  mouse buttons
    if (inputManager->mkeyState[sf::Mouse::Left])  { lmclick = true; }
    if (inputManager->mkeyState[sf::Mouse::Right]) { rmclick = true; }
    //  mouse wheel
    if (inputManager->mouseWheelScroll > 0.0f && this->playerView.getSize().x > this->viewMinX) { this->playerView.zoom(0.9f); }  // Zoom in 
    if (inputManager->mouseWheelScroll < 0.0f && this->playerView.getSize().x < this->viewMaxX) { this->playerView.zoom(1.1f); }  // Zoom out
    //  reset mWheelScrollDelta
    inputManager->mouseWheelScroll = 0.0f;

    //  If following entity (make entity move w/ WASD and attack w/ m click)
    // if (this->entityp) {
    //     this->playerView.setCenter(
    //         sf::Vector2f(
    //             this->entityp->getComponent<PositionComponent>()->x, 
    //             this->entityp->getComponent<PositionComponent>()->y)
    //             );

    //     const auto& movementPtr = this->entityp->getComponent<MovementComponent>();
    //     if (movementPtr->xDir != moveX || movementPtr->yDir != moveY) {
    //         // update X and Y velocity/direction
    //         movementPtr->xDir = moveX;
    //         movementPtr->yDir = moveY;
    //     }
        
    // } else {  //  if not following entity
        if (moveX != 0.0f || moveY != 0.0f )
            this->move(moveX, moveY, deltaTime);  // update view

    // }
}

void Player::stopFollow() {
    this->entityID = 0;
}

bool Player::isFollowingUnit() {
    if (this->entityID != 0)
        return true;
    else
        return false;
    
}
