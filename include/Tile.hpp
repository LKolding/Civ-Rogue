#ifndef _TILE_
#define _TILE_
// sfml
#include <SFML/Graphics.hpp>
// my stuff
#include "constants.h"


struct TileData {
    unsigned int ID;
    uint8_t flipFlags;
    bool isWalkable;
    float friction;
};

struct ChunkData {
    Coord position;
    TileData background_tiles[CHUNK_SIZE];
};


// Custom hash function
namespace std {
    template <>
    struct hash<Coord> {
        std::size_t operator()(const Coord& c) const {
            return std::hash<int>()(c.x) ^ (std::hash<int>()(c.y) << 1);
        }
    };
}

#endif