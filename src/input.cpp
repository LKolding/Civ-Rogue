#include "input.hpp"

void InputManager::start(std::shared_ptr<Player> p) {
    this->player = p;
    this->keyState[sf::Keyboard::W] = false;
    this->keyState[sf::Keyboard::A] = false;
    this->keyState[sf::Keyboard::S] = false;
    this->keyState[sf::Keyboard::D] = false;
}

void InputManager::update(float dt) {
    float moveX = 0;
    float moveY = 0;
    for (auto& key : this->keyState) { // moveX and Y simply indicates direction
        if (key.first == sf::Keyboard::W && key.second == true) { moveY -= 1.0f; }
        if (key.first == sf::Keyboard::A && key.second == true) { moveX -= 1.0f; }
        if (key.first == sf::Keyboard::S && key.second == true) { moveY += 1.0f; }
        if (key.first == sf::Keyboard::D && key.second == true) { moveX += 1.0f; }
    }
    
    // update player
    this->player->move(moveX, moveY, dt);
}

// Returns false if Escape (or another exit key) was pressed
bool handle_event(
    sf::Event& event,
    std::shared_ptr<InputManager> input,
    std::weak_ptr<Player> player
    ) {
    //  exit
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        return false;
    
    //  m btn pressed
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            input->mkeyState[sf::Mouse::Left] = true;
        }
        if (event.mouseButton.button == sf::Mouse::Right) {
            input->mkeyState[sf::Mouse::Right] = true;
        }
    }
    //  keypressed
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::W) {
            input->keyState[sf::Keyboard::W] = true;
        } else
        if (event.key.code == sf::Keyboard::A) {
            input->keyState[sf::Keyboard::A] = true;
        } else
        if (event.key.code == sf::Keyboard::S) {
            input->keyState[sf::Keyboard::S] = true;
        } else
        if (event.key.code == sf::Keyboard::D) {
            input->keyState[sf::Keyboard::D] = true;
        } else
        if (event.key.code == sf::Keyboard::Escape) {
            input->keyState[sf::Keyboard::Escape] = true;
        } else
        if (event.key.code == sf::Keyboard::Space) {
            input->keyState[sf::Keyboard::Space] = true;
        }
    }
    //  mbtn released
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            input->mkeyState[sf::Mouse::Left] = false;
        }
        if (event.mouseButton.button == sf::Mouse::Right) {
            input->mkeyState[sf::Mouse::Right] = false;
        }
    }
    //  keyreleased
    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::W) {
            input->keyState[sf::Keyboard::W] = false;
        } else
        if (event.key.code == sf::Keyboard::A) {
            input->keyState[sf::Keyboard::A] = false;
        } else
        if (event.key.code == sf::Keyboard::S) {
            input->keyState[sf::Keyboard::S] = false;
        } else
        if (event.key.code == sf::Keyboard::D) {
            input->keyState[sf::Keyboard::D] = false;
        } else
        if (event.key.code == sf::Keyboard::Escape) {
            input->keyState[sf::Keyboard::Escape] = false;
        } else
        if (event.key.code == sf::Keyboard::Space) {
            input->keyState[sf::Keyboard::Space] = false;
        }
    }
    //  mousewheel scroll
    if (auto playerp = player.lock()) {
        //  zoom w/ mousewheel
        if (event.type == sf::Event::MouseWheelScrolled) {
            if (event.mouseWheelScroll.delta > 0) {
                if (playerp->playerView.getSize().x > 300)
                    playerp->playerView.zoom(0.9f);  // Zoom in
            } else if (event.mouseWheelScroll.delta < 0) {
                if (playerp->playerView.getSize().x < 1000)
                    playerp->playerView.zoom(1.1f);  // Zoom out
            }
        }
    }
    


    return true; // return true if no exit button has been clicked
}