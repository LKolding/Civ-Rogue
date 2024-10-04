#ifndef _PLAYER_
#define _PLAYER_

#include <SFML/Graphics.hpp>

#include "ECS/Entities/Entity.hpp"

struct Player {
    sf::View playerView;
    std::vector<std::shared_ptr<Entity>> selected_entities;

    void move(float x, float y) {
        this->pos.x += x;
        this->pos.y += y;
        this->updateView();
    }

    void setPosition(float x, float y) {
        this->pos.x = x < 0 ? 0 : x;
        this->pos.y = y < 0 ? 0 : y;
        this->updateView();
    }
    
    float getSpeed() {
        return this->moveSpeed;
    }

    void addSelectedEntity(Entity& entity) {
        //this->selected_entities.push_back(std::make_shared<Entity>(&entity));
    }

private:
    sf::Vector2f pos;
    const float moveSpeed = 10.0f;
    void updateView() {
        this->playerView.setCenter(this->pos);
    }
};
#endif