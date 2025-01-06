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
    void update(float dt);

};

// Returns false if Escape (or another exit key) was pressed
bool handle_event(sf::Event& event, std::shared_ptr<InputManager> input);
#endif