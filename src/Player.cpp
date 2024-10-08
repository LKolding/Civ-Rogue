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
    this->selectedEntities[entity->getComponent<UUIDComponent>()->ID] = std::weak_ptr<Entity>(entity);
}

void Player::deselectUnit(std::shared_ptr<Entity> &entity) {
    this->selectedEntities.erase(entity->getComponent<UUIDComponent>()->ID);
}

void Player::update(float deltaTime) {
    //  Follow entity
    if (auto ent_p = this->entityFollow.lock()) {
        auto ent_pos = sf::Vector2f(ent_p->getComponent<PositionComponent>()->x, ent_p->getComponent<PositionComponent>()->y);
        auto pla_pos = this->getPosition();

        if (ent_p->getComponent<PositionComponent>()->x != pla_pos.x) {
            this->setPosition(ent_pos.x);
        } 

        if (ent_p->getComponent<PositionComponent>()->y != pla_pos.y) {
            this->setPosition(0, ent_pos.y);
        }
        
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
    for (auto& entity : this->selectedEntities) {
        if (!entity.second.lock()->hasComponent<ObjectiveComponent>()) {
            continue;
        }
        entity.second.lock()->getComponent<ObjectiveComponent>()->addObjective(pos);
    }
}