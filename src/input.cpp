#include "input.hpp"

InputManager::InputManager() {
    this->reset();
}
void InputManager::reset() {
    this->keyState[sf::Keyboard::Key::W] = false;
    this->keyState[sf::Keyboard::Key::A] = false;
    this->keyState[sf::Keyboard::Key::S] = false;
    this->keyState[sf::Keyboard::Key::D] = false;

    this->keyState[sf::Keyboard::Key::Escape] = false;
    this->keyState[sf::Keyboard::Key::Space]  = false;

    this->mkeyState[sf::Mouse::Button::Left]  = false;
    this->mkeyState[sf::Mouse::Button::Right] = false;
    this->mkeyState[sf::Mouse::Button::Middle] = false;
    this->mouseWheelScroll = 0.0f;
}

void InputManager::update(const sf::Event& event) {
    //  mouse pressed
    if (auto e = event.getIf<sf::Event::MouseButtonPressed>() ) {
        if (e->button == sf::Mouse::Button::Left) {
            this->mkeyState[sf::Mouse::Button::Left] = true;
        }
        if (e->button == sf::Mouse::Button::Right) {
            this->mkeyState[sf::Mouse::Button::Right] = true;
        }
        if (e->button == sf::Mouse::Button::Middle) {
            this->mkeyState[sf::Mouse::Button::Middle] = true;
        }
    }
    //  key  pressed
    if (auto e = event.getIf<sf::Event::KeyPressed>()) {
        if (e->code == sf::Keyboard::Key::W) {
            this->keyState[sf::Keyboard::Key::W] = true;
        } else
        if (e->code == sf::Keyboard::Key::A) {
            this->keyState[sf::Keyboard::Key::A] = true;
        } else
        if (e->code == sf::Keyboard::Key::S) {
            this->keyState[sf::Keyboard::Key::S] = true;
        } else
        if (e->code == sf::Keyboard::Key::D) {
            this->keyState[sf::Keyboard::Key::D] = true;
        } else
        if (e->code == sf::Keyboard::Key::Escape) {
            this->keyState[sf::Keyboard::Key::Escape] = true;
        } else
        if (e->code == sf::Keyboard::Key::Space) {
            this->keyState[sf::Keyboard::Key::Space] = true;
        }
    }
    //  mouse released
    if (auto e = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (e->button == sf::Mouse::Button::Left) {
            this->mkeyState[sf::Mouse::Button::Left] = false;
        }
        if (e->button == sf::Mouse::Button::Right) {
            this->mkeyState[sf::Mouse::Button::Right] = false;
        }
        if (e->button == sf::Mouse::Button::Middle) {
            this->mkeyState[sf::Mouse::Button::Middle] = false;
        }
    }
    //  key  released
    if (auto e = event.getIf<sf::Event::KeyReleased>()) {
        if (e->code == sf::Keyboard::Key::W) {
            this->keyState[sf::Keyboard::Key::W] = false;
        } else
        if (e->code == sf::Keyboard::Key::A) {
            this->keyState[sf::Keyboard::Key::A] = false;
        } else
        if (e->code == sf::Keyboard::Key::S) {
            this->keyState[sf::Keyboard::Key::S] = false;
        } else
        if (e->code == sf::Keyboard::Key::D) {
            this->keyState[sf::Keyboard::Key::D] = false;
        } else
        if (e->code == sf::Keyboard::Key::Escape) {
            this->keyState[sf::Keyboard::Key::Escape] = false;
        } else
        if (e->code == sf::Keyboard::Key::Space) {
            this->keyState[sf::Keyboard::Key::Space] = false;
        }
    }
    //  mousewheel
    if (auto e = event.getIf<sf::Event::MouseWheelScrolled>()) {
        if (e->wheel == sf::Mouse::Wheel::Vertical) {
            this->mouseWheelScroll = e->delta;
        }
    }
}