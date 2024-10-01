#ifndef _WORLD_
#define _WORLD_
// TMX
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

// pugi
#include "../src/tmxlite/detail/pugixml.hpp"

// SFML
#include <SFML/Graphics.hpp>

// SYSTEM
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <random>

// MY STUFF
#include "ResourceAllocator.hpp"
#include "constants.h"
#include "Player.h"


// Chunk (& tile) definition
// TODO: Move to sperate file
struct Tile {
    unsigned int ID;
    bool isWalkable;
};

struct Chunk {
    sf::Vector2f position;
    Tile background_tiles[CHUNK_SIZE];
};


class World {
public:
    // Loading, parsing, storing etc. of tmx

    std::unordered_map<std::string, tmx::Tileset> tilesets;
    World(ResourceAllocator &allocator);
    bool saveGame(const std::string& gameName);
    void saveMapToTMX(const std::string& filePath);
    // Other
    void createChunkSprites(ResourceAllocator& allocator);
    void render(sf::RenderWindow &ren);

    void generateRandomChunk(sf::Vector2f& pos);

    void setPlayer(Player& p);

private:
    void createGrassTileSprite(unsigned int &ID, sf::Vector2f pos, ResourceAllocator& allocator);
    void loadMap(tmx::Map& map);
    // test stuff
    std::vector<std::unique_ptr<sf::Sprite>> chunkSprites;
    // chunk models
    std::vector<Chunk> chunks;

    std::shared_ptr<Player> playerPtr;
};
#endif