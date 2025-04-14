#ifndef _VIEWPAN_SYSTEM_
#define _VIEWPAN_SYSTEM_

//                       (& zooming)
// System responsible for panning the view of the player
// in relation to the key lookup table in InputManager.

#include <SFML/Graphics.hpp>

#include "constants.h"
#include "input.hpp"


class ViewpanSystem {
public:
    inline void update(float dt, sf::View& playerView, InputManager& im) {
        // keyboard key states
        float moveX = 0.0f;
        float moveY = 0.0f;
        if (im.keyState[sf::Keyboard::Key::W]) { moveY = -1.0f; }  // up
        if (im.keyState[sf::Keyboard::Key::A]) { moveX = -1.0f; }  // left
        if (im.keyState[sf::Keyboard::Key::S]) { moveY =  1.0f; }  // down
        if (im.keyState[sf::Keyboard::Key::D]) { moveX =  1.0f; }  // right

        // Do calculations if keys are held down
        if (moveX != 0.0f || moveY != 0.0f ) {
            const float MOVE_AMOUNT = PANNING_SPEED * dt;

            float x = moveX;
            float y = moveY;

            float length = std::sqrt(x * x + y * y);

            if (length != 0.0f) {
                x /= length;
                y /= length;
            }
            // Apply calculated movement to view
            playerView.move(sf::Vector2f(x * MOVE_AMOUNT, y * MOVE_AMOUNT));
        }

        //  -----> mouse wheel <-----
        if (im.mouseWheelScroll > 0.0f && playerView.getSize().x > viewMinX) { playerView.zoom(0.9f); }  // Zoom in 
        if (im.mouseWheelScroll < 0.0f && playerView.getSize().x < viewMaxX + 10000) { playerView.zoom(1.1f); }  // Zoom out
        //  reset mWheelScrollDelta
        im.mouseWheelScroll = 0.0f;

    };

};

#endif