#ifndef _MINIMAP_SYSTEM_
#define _MINIMAP_SYSTEM_

#include <SFML/Graphics.hpp>

#include "World.hpp"

#include "constants.h"

class MinimapSystem {
public:
    inline void update(sf::RenderWindow& ren, Player& player, WorldManager& wm) {
        sf::RectangleShape minimapBackground;
        minimapBackground.setSize({(float)(WINDOW_WIDTH)*(player.minimapView.getViewport().size.x), (float)(WINDOW_HEIGHT)*(player.minimapView.getViewport().size.y)});
        minimapBackground.setPosition({(WINDOW_WIDTH)*0.75f, 0});
        minimapBackground.setFillColor(BLUE_COLOR);
        minimapBackground.setOutlineColor(sf::Color::White);
        minimapBackground.setOutlineThickness(5.0f);
        ren.draw(minimapBackground);

        player.minimapView.setCenter(player.playerView.getCenter()); // syncronize views

        ren.setView(player.minimapView);
        wm.render(ren);    // render minimap

        ren.setView(player.playerView);  // reset view
    }
};


#endif
/*
// Minimap
    

*/