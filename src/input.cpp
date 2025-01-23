#include "input.hpp"

InputManager::InputManager() {
    this->reset();
}
void InputManager::reset() {
    this->keyState[sf::Keyboard::W] = false;
    this->keyState[sf::Keyboard::A] = false;
    this->keyState[sf::Keyboard::S] = false;
    this->keyState[sf::Keyboard::D] = false;

    this->keyState[sf::Keyboard::Escape] = false;
    this->keyState[sf::Keyboard::Space]  = false;

    this->mkeyState[sf::Mouse::Left]  = false;
    this->mkeyState[sf::Mouse::Right] = false;
    this->mkeyState[sf::Mouse::Middle] = false;

    this->mouseWheelScroll = 0.0f;
}

void InputManager::update(sf::Event& event) {
    //  mouse pressed
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            this->mkeyState[sf::Mouse::Left] = true;
        }
        if (event.mouseButton.button == sf::Mouse::Right) {
            this->mkeyState[sf::Mouse::Right] = true;
        }
        if (event.mouseButton.button == sf::Mouse::Middle) {
            this->mkeyState[sf::Mouse::Middle] = true;
        }
    }
    //  key  pressed
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::W) {
            this->keyState[sf::Keyboard::W] = true;
        } else
        if (event.key.code == sf::Keyboard::A) {
            this->keyState[sf::Keyboard::A] = true;
        } else
        if (event.key.code == sf::Keyboard::S) {
            this->keyState[sf::Keyboard::S] = true;
        } else
        if (event.key.code == sf::Keyboard::D) {
            this->keyState[sf::Keyboard::D] = true;
        } else
        if (event.key.code == sf::Keyboard::Escape) {
            this->keyState[sf::Keyboard::Escape] = true;
        } else
        if (event.key.code == sf::Keyboard::Space) {
            this->keyState[sf::Keyboard::Space] = true;
        }
    }
    
    //  mouse released
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            this->mkeyState[sf::Mouse::Left] = false;
        }
        if (event.mouseButton.button == sf::Mouse::Right) {
            this->mkeyState[sf::Mouse::Right] = false;
        }
        if (event.mouseButton.button == sf::Mouse::Middle) {
            this->mkeyState[sf::Mouse::Middle] = false;
        }
    }
    //  key  released
    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::W) {
            this->keyState[sf::Keyboard::W] = false;
        } else
        if (event.key.code == sf::Keyboard::A) {
            this->keyState[sf::Keyboard::A] = false;
        } else
        if (event.key.code == sf::Keyboard::S) {
            this->keyState[sf::Keyboard::S] = false;
        } else
        if (event.key.code == sf::Keyboard::D) {
            this->keyState[sf::Keyboard::D] = false;
        } else
        if (event.key.code == sf::Keyboard::Escape) {
            this->keyState[sf::Keyboard::Escape] = false;
        } else
        if (event.key.code == sf::Keyboard::Space) {
            this->keyState[sf::Keyboard::Space] = false;
        }
    }
    //  mousewheel
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            this->mouseWheelScroll = event.mouseWheelScroll.delta;
        }
    }

}