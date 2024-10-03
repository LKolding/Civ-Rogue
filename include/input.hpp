#ifndef _INPUT_
#define _INPUT_

#include <SFML/Graphics.hpp>

#include "World.hpp"

class InputManager {
public:
    std::map<sf::Keyboard::Key, bool> keyState;
    InputManager(std::shared_ptr<Player> p) { 
        this->player = p;
        this->keyState[sf::Keyboard::W] = false;
        this->keyState[sf::Keyboard::A] = false;
        this->keyState[sf::Keyboard::S] = false;
        this->keyState[sf::Keyboard::D] = false;
    }

    void update(float dt) {
        //  Check every key you care about
        float MOVE_AMOUNT = this->player->getSpeed() * dt;
        float moveX = 0;
        float moveY = 0;
        for (auto& key : this->keyState) { // moveX and Y simply indicates direction
            if (key.first == sf::Keyboard::W && key.second == true) { moveY -= 1.0f; }
            if (key.first == sf::Keyboard::A && key.second == true) { moveX -= 1.0f; }
            if (key.first == sf::Keyboard::S && key.second == true) { moveY += 1.0f; }
            if (key.first == sf::Keyboard::D && key.second == true) { moveX += 1.0f; }
        }
        // Calculate the length of the movement vector
        float length = std::sqrt(moveX * moveX + moveY * moveY);

        // Normalize the movement vector if it's non-zero
        if (length != 0.0f) {
            moveX /= length;
            moveY /= length;
        }
        // update player
        this->player->move(moveX * MOVE_AMOUNT, moveY * MOVE_AMOUNT);
    }

private:
    std::shared_ptr<Player> player;
};

inline bool handle_event(
    sf::Event& event,
    InputManager& input,
    std::shared_ptr<Player> playerp,
    float deltaTime
    ) {

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        return false;
    

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::W) {
            input.keyState[sf::Keyboard::W] = true;
        } 
        if (event.key.code == sf::Keyboard::A) {
            input.keyState[sf::Keyboard::A] = true;
        }
        if (event.key.code == sf::Keyboard::S) {
            input.keyState[sf::Keyboard::S] = true;
        }
        if (event.key.code == sf::Keyboard::D) {
            input.keyState[sf::Keyboard::D] = true;
        }
        if (event.key.code == sf::Keyboard::Escape) {
            input.keyState[sf::Keyboard::Escape] = true;
        }
    }
    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::W) {
            input.keyState[sf::Keyboard::W] = false;
        }
        if (event.key.code == sf::Keyboard::A) {
            input.keyState[sf::Keyboard::A] = false;
        }
        if (event.key.code == sf::Keyboard::S) {
            input.keyState[sf::Keyboard::S] = false;
        }
        if (event.key.code == sf::Keyboard::D) {
            input.keyState[sf::Keyboard::D] = false;
        }
        if (event.key.code == sf::Keyboard::Escape) {
            input.keyState[sf::Keyboard::Escape] = false;
        }
    }
    //  zoom w/ mousewheel
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta > 0) {
            if (playerp->playerView.getSize().x > 300)
                playerp->playerView.zoom(0.9f);  // Zoom in
        } else if (event.mouseWheelScroll.delta < 0) {
            if (playerp->playerView.getSize().x < 800)
                playerp->playerView.zoom(1.1f);  // Zoom out
        }
    }

    return true; // return true if no exit button has been clicked
}
#endif