#include "input.hpp"

InputManager::InputManager() {
    this->keyState[sf::Keyboard::W] = false;
    this->keyState[sf::Keyboard::A] = false;
    this->keyState[sf::Keyboard::S] = false;
    this->keyState[sf::Keyboard::D] = false;

    this->keyState[sf::Keyboard::Escape] = false;
    this->keyState[sf::Keyboard::Space]  = false;

    this->mkeyState[sf::Mouse::Left]  = false;
    this->mkeyState[sf::Mouse::Right] = false;

    this->mouseWheelScroll = 0.0f;
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

    this->mouseWheelScroll = 0.0f;
}

void InputManager::update(float dt) {
    return;
}

// Returns false if Escape was pressed (or another exit event happened) 
bool handle_event(
    sf::Event& event,
    std::shared_ptr<InputManager> input
    ) {
    //input->reset();
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

    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            input->mouseWheelScroll = event.mouseWheelScroll.delta;
        }
    }

    return true; // return true if no exit button has been clicked
}