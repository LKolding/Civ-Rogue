#ifndef _INPUT_
#define _INPUT_

#include <SFML/Graphics.hpp>

class InputManager {
public:
    std::map<sf::Mouse::Button, bool> mkeyState;
    std::map<sf::Keyboard::Key, bool> keyState;
    float mouseWheelScroll; // sf mouseScroll.delta
    InputManager();
    void reset();
    void update(sf::Event& event);

};

#endif