#ifndef _TILE_
#define _TILE_
// sfml
#include <SFML/Graphics.hpp>
// my stuff
#include "constants.h"


struct Tile {
    unsigned int ID;
    bool isWalkable;
};

struct Chunk {
    sf::Vector2f position;
    Tile background_tiles[CHUNK_SIZE];
};
#endif