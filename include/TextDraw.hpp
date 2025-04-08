#ifndef _TEXT_DRAW_
#define _TEXT_DRAW_

#include <SFML/Graphics.hpp>
#include "ResourceAllocator.hpp"

inline void drawString(std::unique_ptr<sf::RenderWindow> &ren, std::shared_ptr<ResourceAllocator> &allocator, std::string text, const int yoffset = 10) {
    sf::Text textToBeDrawn(text, allocator->default_font, 11);
    textToBeDrawn.setPosition(sf::Vector2f(10,yoffset));
    textToBeDrawn.setFillColor(sf::Color::White);
    ren->draw(textToBeDrawn);

}

#endif
