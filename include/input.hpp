#ifndef _INPUT_
#define _INPUT_

#include <SFML/Graphics.hpp>

#include "World.hpp"

class InputManager {
public:
    std::map<sf::Mouse::Button, bool> mkeyState;
    std::map<sf::Keyboard::Key, bool> keyState;

    void start(std::shared_ptr<Player> p);
    void update(float dt);

private:
    std::shared_ptr<Player> player;
};

// Returns false if Escape (or another exit key) was pressed
bool handle_event(
    sf::Event& event,
    std::shared_ptr<InputManager> input,
    std::weak_ptr<Player> player
    );
#endif