#ifndef _INPUT_
#define _INPUT_

#include <SFML/Graphics.hpp>
#include <map>

class InputManager {
public:
    std::map<sf::Mouse::Button, bool> mkeyState;
    std::map<sf::Keyboard::Key, bool> keyState;
    float mouseWheelScroll; // sf mouseScroll.delta
    InputManager();
    void reset();
    void update(const sf::Event& event);

};

#endif