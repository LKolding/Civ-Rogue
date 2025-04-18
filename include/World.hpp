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
#include "ResourceManager.hpp"
#include "Tile.hpp"
#include "Player.hpp"

#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"
#include "ECS/Components/Components.hpp"

#include "coordinate_calculators.hpp"
#include "constants.h"
#include "sprite_constructors.hpp"

// World data (basically just a wrapper for unordered_map of chunks)
class World {
public:
    std::unordered_map<Coord, ChunkData> m_chunks;
    // Returns nullptr if chunk is non-existent
    const ChunkData* getChunk(Coord chunkCoord);
    const TileData*  getTile(Coord tileCoord);

    bool loadChunk(ChunkData& data);
};

// WorldManager
class WorldManager {
public:
    World world;

    // TMX stuff
    // Loads chunk from tmxdata and converts it into internal ChunkData class
    bool loadChunkTMX(const tmx::TileLayer::Chunk& tmxdata);
    // Converts and returns chunk from internal ChunkData class into tmx::Chunk 
    tmx::TileLayer::Chunk getChunkTMX(Coord chunkCoord);
    void saveMapToTMX(const std::string& filePath, EntityManager& em, ComponentManager& cm) const;

    void initialize(std::weak_ptr<ResourceManager> allocator, const std::string& game_name);
    
    void generateRandomChunk(Coord pos);
    //  Render
    void createChunkSprites(std::shared_ptr<ResourceManager> allocator);
    void render(sf::RenderWindow &ren) const;

private:
    void createGrassTileSprite(unsigned int &ID, sf::Vector2f pos, ResourceManager& allocator);
    void loadMap(const tmx::Map& map);
    
    std::vector<std::unique_ptr<sf::Sprite>> chunkSprites;
    std::weak_ptr<ResourceManager> resourceAllocatorPtr;
};
#endif