#ifndef _VIEWZOOM_SYSTEM_
#define _VIEWZOOM_SYSTEM_

#include <SFML/Graphics.hpp>

#include "Player.hpp"
#include "input.hpp"


class ViewzoomSystem {
public:
    inline void update(sf::View& playerView, InputManager& im) {
        //  -----> mouse wheel <-----
        if (im.mouseWheelScroll > 0.0f && playerView.getSize().x > viewMinX) { playerView.zoom(0.9f); }  // Zoom in 
        if (im.mouseWheelScroll < 0.0f && playerView.getSize().x < viewMaxX) { playerView.zoom(1.1f); }  // Zoom out
    };

};


#endif