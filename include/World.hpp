#ifndef _WORLD_
#define _WORLD_
// TMX
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/Types.hpp>

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
#include "Tile.hpp"
#include "Player.hpp"

#include "coordinate_calculators.hpp"
#include "constants.h"
#include "sprite_constructors.hpp"


class World {
public:
    std::unordered_map<Coord, ChunkData> m_chunks;
    // Returns nullptr if chunk is non-existant
    const ChunkData* getChunk(Coord chunkCoord);
    TileData*  getTile(Coord tileCoord);

    bool loadChunk(ChunkData& data);
};

// WorldManager
class WorldManager {
public:
    // NEW
    bool loadChunkTMX(const tmx::TileLayer::Chunk& tmxdata);
    // Converts and returns chunk from internal ChunkData class into tmx::Chunk 
    tmx::TileLayer::Chunk getChunkTMX(Coord chunkCoord);


    void initialize(std::weak_ptr<ResourceAllocator> allocator, std::weak_ptr<Player> p, const std::string& game_name);
    void saveMapToTMX(const std::string& filePath) const;
    
    void generateRandomChunk(Coord pos);
    //  Render
    void createChunkSprites(std::shared_ptr<ResourceAllocator> allocator);
    void render(std::unique_ptr<sf::RenderWindow> &ren) const;

private:
    void createGrassTileSprite(unsigned int &ID, sf::Vector2f pos, ResourceAllocator& allocator);
    void loadMap(const tmx::Map& map);
    
    std::vector<std::unique_ptr<sf::Sprite>> chunkSprites;

    // NEW
    World world;

    std::weak_ptr<Player> playerPtr;
};
#endif