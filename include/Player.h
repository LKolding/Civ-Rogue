#ifndef _PLAYER_
#define _PLAYER_

#include <SFML/Graphics.hpp>

struct Player {
    sf::Vector2f pos;
    sf::View playerView;

    void move(float x, float y) {
        this->pos.x += x;
        this->pos.y += y;
        this->updateView();
    }

    void setPosition(float x, float y) {
        this->pos.x = x;
        this->pos.y = y;
        this->updateView();
    }
    
private:
    void updateView() {
        this->playerView.setCenter(this->pos);
    }
};
#endif