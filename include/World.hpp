#ifndef _WORLD_
#define _WORLD_
// TMX
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

// SFML
#include <SFML/Graphics.hpp>

// SYSTEM
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>

// MY STUFF
#include "ResourceAllocator.hpp"
#include "constants.h"


// Chunk (& tile) definition
// TODO: Move to sperate file
struct Tile {
    unsigned int ID;
    bool isWalkable;
};

struct Chunk {
    Tile background_tiles[CHUNK_SIZE];
};


class World {
public:
    // Loading, parsing, storing etc. of tmx

    std::unordered_map<std::string, tmx::Tileset> tilesets;
    tmx::Map map; // todo: remove
    World(ResourceAllocator &allocator);
    bool loadMap(const std::string& mapName);

    // Other
    void createChunkSprites(Chunk& chunk, ResourceAllocator& allocator);
    void render(sf::RenderWindow &ren);
    void generateChunks(void);

    // test stuff
    // sprites
    std::vector<sf::Sprite>  tileSprites;
    std::vector<sf::Sprite> chunkSprites;
    // chunk models
    std::vector<Chunk> chunks;

private:
    void createGrassTileSprite(unsigned int &ID, sf::Vector2f pos, ResourceAllocator& allocator);

};
#endif