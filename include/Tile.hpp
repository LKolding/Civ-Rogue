#ifndef _TILE_
#define _TILE_
// sfml
#include <SFML/Graphics.hpp>
// my stuff
#include "constants.h"


// Coordinate definition
struct Coord {
    int32_t x, y;
    bool operator==(const Coord& other) const { return x == other.x && y == other.y; }
};

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