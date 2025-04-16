#ifndef _INPUT_
#define _INPUT_

#include <SFML/Graphics.hpp>
#include <map>

class InputManager {
public:
    // ------------------
    // ----- Fields -----
    // ------------------
    
    std::map<sf::Mouse::Button, bool> mkeyState;
    std::map<sf::Keyboard::Key, bool> keyState;
    float mouseWheelScroll;  // sf mouseScroll.delta
    std::map<sf::Keyboard::Key, bool> keyJustPressed; // Only for keyboard keys
    std::map<sf::Keyboard::Key, bool> keyJustReleased;// Only for keyboard keys


    // -------------------
    // ----- Methods -----
    // -------------------

    InputManager();
    void reset();
    void update(const sf::Event& event);

};

#endif