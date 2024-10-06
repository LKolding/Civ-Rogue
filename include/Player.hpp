#ifndef _PLAYER_
#define _PLAYER_

#include <SFML/Graphics.hpp>

#include "ECS/Entities/Entity.hpp"


//  Basically only holds the playerView
struct Player {
    sf::View playerView;

    // x and y simply indicate direction, and should be of
    // value 1.0, 0.0 or -1.0
    void move(float x, float y, float dt) {
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

    void setPosition(float x, float y) {
        this->pos.x = x < 0 ? 0 : x;
        this->pos.y = y < 0 ? 0 : y;
        this->updateView();
    }

    void selectUnit(std::shared_ptr<Entity> &entity) {
        this->selectedEntities[entity->getComponent<UUIDComponent>()->ID] = std::weak_ptr<Entity>(entity);
    }

    void deselectUnit(std::shared_ptr<Entity> &entity) {
        this->selectedEntities.erase(entity->getComponent<UUIDComponent>()->ID);
    }

    void addObjectiveToSelectedUnits(sf::Vector2i pos) {
        for (auto& entity : this->selectedEntities) {
            if (!entity.second.lock()->hasComponent<ObjectiveComponent>()) {
                continue;
            }
            entity.second.lock()->getComponent<ObjectiveComponent>()->addObjective(pos);
        }
    }

private:
    sf::Vector2f pos;
    std::unordered_map<uint32_t, std::weak_ptr<Entity>> selectedEntities;
    const float moveSpeed = 0.001f; // View panning speed
    void updateView() {
        this->playerView.setCenter(this->pos);
    }
};
#endif