#include "Player.hpp"


void Player::updateView() {
    this->playerView.setCenter(this->pos);
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

    this->pos.x += x * MOVE_AMOUNT;
    this->pos.y += y * MOVE_AMOUNT;
    this->updateView();
}

void Player::setPosition(float x, float y) {
    this->pos.x = x == 0 ? this->pos.x : x;
    this->pos.y = y == 0 ? this->pos.y : y;
    this->updateView();
}
sf::Vector2f Player::getPosition() {
    return this->pos;
}


void Player::update(float deltaTime, std::unique_ptr<InputManager>& inputManager, sf::Vector2f mouse_pos) {
    this->m_mouse_pos = mouse_pos;  // update local mouse position variable
    //  Follow entity if pointer is valid
    if (auto ent_p = this->entityp.lock()) {
        auto ent_pos = sf::Vector2f(ent_p->getComponent<PositionComponent>()->x, ent_p->getComponent<PositionComponent>()->y);
        auto view_pos = this->getPosition();

        if (ent_p->getComponent<PositionComponent>()->x != view_pos.x) {
            this->setPosition(ent_pos.x);//  <- set view directly to entity coords
        } 

        if (ent_p->getComponent<PositionComponent>()->y != view_pos.y) {
            this->setPosition(0, ent_pos.y);//  <- and here
        }
    }
    // check for input
    float moveX = 0.0f;
    float moveY = 0.0f;
    bool lmclick = false;  // left mouse click
    bool rmclick = false;  // right mouse click
    bool mmclick = false;  // middle mouse
    bool spacebar= false;  // spacebar

    // keyboard
    if (inputManager->keyState[sf::Keyboard::W]) { moveY = -1.0f; }  // up
    if (inputManager->keyState[sf::Keyboard::A]) { moveX = -1.0f; }  // left
    if (inputManager->keyState[sf::Keyboard::S]) { moveY =  1.0f; }  // down
    if (inputManager->keyState[sf::Keyboard::D]) { moveX =  1.0f; }  // right
    if (inputManager->keyState[sf::Keyboard::Space]) { spacebar = true; }
    // mouse
    if (inputManager->mkeyState[sf::Mouse::Left])  { lmclick = true; }
    if (inputManager->mkeyState[sf::Mouse::Right]) { rmclick = true; }
    if (inputManager->mkeyState[sf::Mouse::Middle]){ mmclick = true; }
    // zoom w/ mouse wheel
    if (inputManager->mouseWheelScroll > 0.0f && this->playerView.getSize().x > 500) { this->playerView.zoom(0.9f); }  // Zoom in 
    if (inputManager->mouseWheelScroll < 0.0f && this->playerView.getSize().x < 900) { this->playerView.zoom(1.1f); }  // Zoom out
    // reset mWheelScrollDelta
    inputManager->mouseWheelScroll = 0.0f;

    //  If following entity (make entity move w/ WASD and attack w/ m click)
    if (auto ent = this->entityp.lock()) {
        // update animation
        if (moveX != 0.0 || moveY != 0.0)
            std::static_pointer_cast<NinjaEntity>(ent)->transitionState(NinjaStateComponent::WALK);
        else
            std::static_pointer_cast<NinjaEntity>(ent)->transitionState(NinjaStateComponent::IDLE);

        // update X and Y velocity/direction
        ent->getComponent<VelocityComponent>()->xDir = moveX;
        ent->getComponent<VelocityComponent>()->yDir = moveY;

        if (lmclick) {
            std::static_pointer_cast<NinjaEntity>(ent)->weapon_attack(this->m_mouse_pos);
        }
        
    } else {  // else we move the m_playerView through move() method
        if (moveX != 0.0f || moveY != 0.0f )
            this->move(moveX, moveY, deltaTime);

        if (mmclick) {
            this->stopFollow();
        }
    }
}

void Player::followUnit(std::shared_ptr<Entity>& entity) {
    this->entityp = entity;
}

void Player::stopFollow() {
    if (auto p = this->entityp.lock()) {
        // reset entity movement
        p->getComponent<VelocityComponent>()->xDir = 0.0f;
        p->getComponent<VelocityComponent>()->yDir = 0.0f;
    }
    this->entityp.reset();
}

bool Player::isFollowingUnit() {
    if (this->entityp.lock())
        return true;
    else
        return false;
    
}
