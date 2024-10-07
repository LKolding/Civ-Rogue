#ifndef _COORD_CALCULATORS_
#define _COORD_CALCULATORS_
#include <SFML/Graphics.hpp>

#include "constants.h"

inline sf::Vector2i getChunkCoords(sf::Vector2f global_coords) {
    // Calculate chunk coordinates using floor division
    int chunkX = static_cast<int>(std::floor(global_coords.x / (CHUNK_WIDTH  * TILE_WIDTH)));
    int chunkY = static_cast<int>(std::floor(global_coords.y / (CHUNK_HEIGHT * TILE_HEIGHT)));
    return sf::Vector2i(chunkX, chunkY);
}

inline sf::Vector2f getTileIndex(sf::Vector2f global_coords) {
    sf::Vector2i localInChunk = getChunkCoords(global_coords);
    // Determine the tile within the chunk
    int tileX = localInChunk.x / (TILE_WIDTH / 2);
    int tileY = localInChunk.y / (TILE_HEIGHT / 2);
    return sf::Vector2f(tileX, tileY);
}
#endif