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

void Player::selectUnit(std::shared_ptr<Entity> &entity) {
    if (entity->hasComponent<SelectableComponent>())
        this->selectedEntities[entity->getComponent<UUIDComponent>()->ID] = std::weak_ptr<Entity>(entity);
}

void Player::deselectUnit(std::shared_ptr<Entity> &entity) {
    this->selectedEntities.erase(entity->getComponent<UUIDComponent>()->ID);
}

void Player::update(float deltaTime, std::shared_ptr<InputManager> inputManager) {
    //  Follow entity if pointer is valid
    if (auto ent_p = this->entityFollow.lock()) {
        auto ent_pos = sf::Vector2f(ent_p->getComponent<PositionComponent>()->x, ent_p->getComponent<PositionComponent>()->y);
        auto pla_pos = this->getPosition();

        if (ent_p->getComponent<PositionComponent>()->x != pla_pos.x) {
            this->setPosition(ent_pos.x);//  <- set view directly to entity coords
        } 

        if (ent_p->getComponent<PositionComponent>()->y != pla_pos.y) {
            this->setPosition(0, ent_pos.y);//  <- and here
        }
    }
    // check for input
    float moveX = 0;
    float moveY = 0;
    if (inputManager->keyState[sf::Keyboard::W]) { moveY = -1.0f; } // up
    if (inputManager->keyState[sf::Keyboard::A]) { moveX = -1.0f; } // left
    if (inputManager->keyState[sf::Keyboard::S]) { moveY =  1.0f; } // down
    if (inputManager->keyState[sf::Keyboard::D]) { moveX =  1.0f; } // right
    // zoom
    if (inputManager->mouseWheelScroll > 0.0f && this->playerView.getSize().x > 500) {
        this->playerView.zoom(0.9f);  // Zoom in
    } 
    if (inputManager->mouseWheelScroll < 0.0f && this->playerView.getSize().x < 900) {
        this->playerView.zoom(1.1f);  // Zoom out
    }
    // reset mWheelScrollDelta
    inputManager->mouseWheelScroll = 0.0f;
    
    // update entity velocity if selected
    if (auto ent = this->entityFollow.lock()) {
        if (moveX != 0.0 || moveY != 0.0)
            std::static_pointer_cast<NinjaEntity>(ent)->transitionState(NinjaStateComponent::WALK);
        else
            std::static_pointer_cast<NinjaEntity>(ent)->transitionState(NinjaStateComponent::IDLE);
        ent->getComponent<VelocityComponent>()->xDir = moveX;
        ent->getComponent<VelocityComponent>()->yDir = moveY;
        
    } else { // else simply update view member through move() method
        if (moveX != 0.0f || moveY != 0.0f )
            this->move(moveX, moveY, deltaTime);
    }
    

}

void Player::followUnit(std::shared_ptr<Entity>& entity) {
    this->entityFollow = entity;
}

void Player::stopFollow() {
    if (this->entityFollow.lock()) {
        this->entityFollow.reset();
    }
}

bool Player::isFollowingUnit() {
    if (this->entityFollow.lock()) {
        return true;
    } else {
        return false;
    }
}

void Player::addObjectiveToSelectedUnits(sf::Vector2i pos) {
    for (auto& entity_p : this->selectedEntities) {
        if (auto entity = entity_p.second.lock()) { 
            // if entity doesn't have objComp OR entity can be selected, and in fact is selected; skip
            if (!entity->hasComponent<ObjectiveComponent>() || (entity->hasComponent<SelectableComponent>() && !entity->getComponent<SelectableComponent>()->isSelected)) {
                continue;
            }
            entity->getComponent<ObjectiveComponent>()->addObjective(pos);
        }
    }
}