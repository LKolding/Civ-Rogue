#include "World.hpp"

// ----------------------
// ----- DATA CLASS -----
// ----------------------

// Returns non-const pointer (direct access) to TileData of tile at passed Coord.
// "tileCoord" refers to the global grid and is not relative to some chunk.
const TileData* World::getTile(Coord tileCoord) {
    Coord chunkCoord = {
        (int32_t)std::floor(tileCoord.x / static_cast<float>(CHUNK_WIDTH)),
        (int32_t)std::floor(tileCoord.y / static_cast<float>(CHUNK_HEIGHT))
    };

    Coord localTile = {
        tileCoord.x % CHUNK_WIDTH,
        tileCoord.y % CHUNK_HEIGHT
    };

    // Fix for negative mod result
    if (localTile.x < 0) localTile.x += CHUNK_WIDTH;
    if (localTile.y < 0) localTile.y += CHUNK_HEIGHT;

    auto it = m_chunks.find(chunkCoord);
    if (it == m_chunks.end()) return nullptr;

    ChunkData& chunk = it->second;
    return &chunk.background_tiles[localTile.y * CHUNK_WIDTH + localTile.x];

};

// Returns nullptr if chunk is non-existant
const ChunkData* World::getChunk(Coord chunkCoord) {
    if (m_chunks.find(chunkCoord) == m_chunks.end())
        return nullptr; 
    return &m_chunks[chunkCoord];
};

// Returns false if chunk already exists
bool World::loadChunk(ChunkData& data) {
    if (m_chunks.find(data.position) != m_chunks.end())
        return false;

    m_chunks.emplace(data.position, data);//<--TODO: this is probably REALLY inefficient
    return true;
}


// -------------------
// ----- MANAGER -----
// -------------------

bool WorldManager::loadChunkTMX(const tmx::TileLayer::Chunk& tmxdata) {
    auto pchunk = this->world.getChunk({tmxdata.position.x, tmxdata.position.y});
    if (pchunk) {
        std::cout << "Chunk already exists!\n";
        return false;
    }

    ChunkData temp;//<-- construct temp object to store later
    temp.position.x = tmxdata.position.x / CHUNK_WIDTH;
    temp.position.y = tmxdata.position.y / CHUNK_HEIGHT;

    for (size_t i = 0; i < tmxdata.tiles.size(); ++i) {
        temp.background_tiles[i] = {tmxdata.tiles[i].ID, 0, true, 0.0f};//<-- GID, isWalkable, friction
    }
    // Store chunk
    return this->world.loadChunk(temp);

}

tmx::TileLayer::Chunk WorldManager::getChunkTMX(Coord chunkCoord) {
    auto pchunk = this->world.getChunk(chunkCoord);
    if (!pchunk)
        return {};

    tmx::TileLayer::Chunk tmxChunk;
    tmxChunk.position.x = pchunk->position.x;
    tmxChunk.position.y = pchunk->position.y;

    for (auto& tile : pchunk->background_tiles) {
        tmx::TileLayer::Tile tempTile;
        tempTile.ID = tile.ID;
        tempTile.flipFlags = tile.flipFlags;
        tmxChunk.tiles.push_back(tempTile);
    }
    return tmxChunk;
}

void WorldManager::initialize(std::weak_ptr<ResourceManager> allocator, const std::string& game_name) {
    // Performs an initialization of the World object incl.
    // tilesets, chunks and tiles. Extracts from game file
    tmx::Map map;
    this->resourceAllocatorPtr = allocator;

    if (!map.load(game_name)) {
        std::cout << std::filesystem::current_path() << "\n";
        throw std::runtime_error("Couldn't load. World is left uninitialized...");
    }
    // Extract tileset(s) from .tmx file
    const auto& tilesets = map.getTilesets();
    for(const auto& tileset : tilesets) {
        if (auto alloc = allocator.lock()) {
            alloc->addTileset(tileset);
        }
    }

    this->loadMap(map);
}

// TEMP FUNCTION BELOW
void WorldManager::saveMapToTMX(const std::string& filePath, EntityManager& em, ComponentManager& cm) const {
    pugi::xml_document doc;

    // Create the <map> element
    pugi::xml_node mapNode = doc.append_child("map");
    mapNode.append_attribute("version") = "1.10";
    mapNode.append_attribute("tiledversion") = "1.11.0";
    mapNode.append_attribute("orientation") = "orthogonal";
    mapNode.append_attribute("renderorder") = "right-down";
    mapNode.append_attribute("width") = CHUNK_WIDTH;
    mapNode.append_attribute("height") = CHUNK_HEIGHT;
    mapNode.append_attribute("tilewidth") = TILE_WIDTH;
    mapNode.append_attribute("tileheight") = TILE_HEIGHT;
    mapNode.append_attribute("infinite") = "1";

    // Add grass <tileset> element
    pugi::xml_node tilesetNode = mapNode.append_child("tileset");
    tilesetNode.append_attribute("firstgid") = 1;
    tilesetNode.append_attribute("name") = "grass";
    tilesetNode.append_attribute("tilewidth") = 32;
    tilesetNode.append_attribute("tileheight") = 32;
    tilesetNode.append_attribute("source")   = "../assets/tilesets/grass.tsx";
    pugi::xml_node imageNode = tilesetNode.append_child("image");
    imageNode.append_attribute("source") = "../assets/tilesets/grass.png";

    // Add <layer> for tile data
    pugi::xml_node layerNode = mapNode.append_child("layer");
    layerNode.append_attribute("name")   = "background";
    layerNode.append_attribute("id")     = "1";
    layerNode.append_attribute("width")  = CHUNK_WIDTH; // <-- necessary?
    layerNode.append_attribute("height") = CHUNK_HEIGHT;// <-- necessary?

    // Add tile data as a CSV string inside a <data> node
    pugi::xml_node dataNode = layerNode.append_child("data");
    dataNode.append_attribute("encoding") = "csv";

    // chunks
    for (auto& [coord, chunk] : this->world.m_chunks) {
        // Create chunk node
        pugi::xml_node chunkNode = dataNode.append_child("chunk");
        chunkNode.append_attribute("x") = coord.x * CHUNK_WIDTH;
        chunkNode.append_attribute("y") = coord.y * CHUNK_HEIGHT;
        chunkNode.append_attribute("width")  = CHUNK_WIDTH;
        chunkNode.append_attribute("height") = CHUNK_HEIGHT;

        std::ostringstream csvStream;
        uint16_t tileCounter = 0;
        for (const TileData& tile : chunk.background_tiles) {
            if (tileCounter == 0)
                csvStream << "\n";
            
            csvStream << std::to_string(tile.ID) + ",";

            if (tileCounter == 16)
                tileCounter = 0;
            else
                tileCounter++;
        }

        chunkNode.text() = csvStream.str().c_str(); // Set chunk data
    }
    // objects
    pugi::xml_node objectsNode = mapNode.append_child("objectgroup");
    objectsNode.append_attribute("name") = "objects";
    
    // pugi::xml_node playerLocationNode = objectsNode.append_child("object");
    // // playerLocationNode.append_attribute("id") = "1";
    // playerLocationNode.append_attribute("name") = "playerLocation";
    // playerLocationNode.append_attribute("x") = (int) this->rplayer.playerView.getCenter().x;
    // playerLocationNode.append_attribute("y") = (int) this->rplayer.playerView.getCenter().y;
    
    pugi::xml_node entitiesNode = mapNode.append_child("objectgroup");
    entitiesNode.append_attribute("name") = "entities";

    for (EntityID& ent : em.getAllEntities()) {
        pugi::xml_node entityNode = entitiesNode.append_child("object");
        entityNode.append_attribute("x") = cm.getComponent<PositionComponent>(ent)->x;
        entityNode.append_attribute("y") = cm.getComponent<PositionComponent>(ent)->y;

        // TODO: add properties to each object with component data
        // SpriteComponent* pspritedata = cm.getComponent<SpriteComponent>(ent);
        // if (pspritedata) {
        //     pspritedata->origin;
        // }

    }

    // Save to file
    doc.save_file(filePath.c_str());
}

void WorldManager::generateRandomChunk(Coord pos) {
    const ChunkData* pchunk = world.getChunk(pos);
    if (pchunk) {
        std::cout << "Tried to generate new chunk at existing position (" << pos.x << ", " << pos.y << ")\n";
        return; // return if chunk already exists
    }

    ChunkData tempChunk;
    tempChunk.position = pos;

    for (int i = 0; i < CHUNK_SIZE; ++i) {
        tempChunk.background_tiles[i].ID = rand() % 30 + 1;
        tempChunk.background_tiles[i].flipFlags = 0;
        tempChunk.background_tiles[i].isWalkable = true;
        tempChunk.background_tiles[i].friction = 0.0f;
    }

    world.loadChunk(tempChunk);
    this->chunkSprites.push_back(createChunkSprite(tempChunk, this->resourceAllocatorPtr.lock()->getTileset("grass"), *resourceAllocatorPtr.lock()));
}

// Extracts chunks from map parameter and stores it in chunks vector of World.
void WorldManager::loadMap(const tmx::Map& map) {
    for (const auto& layer : map.getLayers()) { // iterate layers
        switch (layer->getType()) {
            // TileLayer
            case tmx::Layer::Type::Tile: {  
                const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
                for (const auto& chunk : tileLayer.getChunks()) {
                    // Load chunk
                    if (!this->loadChunkTMX(chunk))
                        std::cout << "Couldn't load chunk\n";
                }
                break;
            }
            // ObjectLayer
            case tmx::Layer::Type::Object: {  
                const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
                if (objectLayer.getName() == "objects") {
                    for (const auto& object : objectLayer.getObjects()) {  // iterate objects
                        // Player location
                        // if (object.getName() == "playerLocation") {
                        //     this->rplayer.playerView.setCenter({object.getPosition().x, object.getPosition().y});
                        // }
                    }
                break;
                }
            }
            default:
                break;
        }
    }
}

void WorldManager::createChunkSprites(std::shared_ptr<ResourceManager> allocator) {
    for (auto& chunk : this->world.m_chunks) {
        this->chunkSprites.push_back(createChunkSprite(
            chunk.second,
            allocator->getTileset("grass"),
            *allocator
        ));
    }
}

void WorldManager::render(sf::RenderWindow& ren) const {    
    // render chunks
    for (auto& sprite : this->chunkSprites) {
        ren.draw(*sprite);
    }
}