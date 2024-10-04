#ifndef _PLAYER_
#define _PLAYER_

#include <SFML/Graphics.hpp>

#include "ECS/Entities/Entity.hpp"


//  Basically only holds the playerView
struct Player {
    sf::View playerView;

    // x and y only indicate direction
    // and therefore should only be of
    // value 0, 1 or -1
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


private:
    sf::Vector2f pos;
    const float moveSpeed = 10.0f; // View panning speed
    void updateView() {
        this->playerView.setCenter(this->pos);
    }
};
#endif