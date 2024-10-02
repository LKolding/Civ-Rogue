#include "World.hpp"

#define GAME_NAME "../saveGames/game1_test.tmx" //  TODO: Temporary


void World::initialize(ResourceAllocator &allocator, std::shared_ptr<Player> p) {
    // Performs an initialization of the World object incl.
    // tilesets, chunks and tiles. Extracts from game file
    tmx::Map map;
    this->playerPtr = p;

    if (!map.load(GAME_NAME)) {
        std::cout << std::filesystem::current_path() << "\n";
        throw std::runtime_error("Couldn't load. World is left uninitialized...");
    }
    // Extract tileset(s) from .tmx file
    const auto& tilesets = map.getTilesets();
    for(const auto& tileset : tilesets) {
        // Load grass texture
        if (tileset.getName() == "grass") {
            allocator.loadTexture(tileset.getImagePath());
            this->tilesets.insert({"grass", tileset});
        }
        else {
            allocator.loadTexture(tileset.getImagePath());
            this->tilesets.insert({tileset.getName(), tileset});
        }
    }
    this->loadMap(map);
}

// TEMP FUNCTION BELOW
void World::saveMapToTMX(const std::string& filePath) {
    pugi::xml_document doc;

    // Create the <map> element
    pugi::xml_node mapNode = doc.append_child("map");
    mapNode.append_attribute("version") = "1.10";
    mapNode.append_attribute("tiledversion") = "1.11.0";  // Adjust for your Tiled version
    mapNode.append_attribute("orientation") = "orthogonal";
    mapNode.append_attribute("renderorder") = "right-down";
    mapNode.append_attribute("width") = CHUNK_WIDTH;
    mapNode.append_attribute("height") = CHUNK_HEIGHT;
    mapNode.append_attribute("tilewidth") = TILE_WIDTH;
    mapNode.append_attribute("tileheight") = TILE_HEIGHT;
    mapNode.append_attribute("infinite") = "1";

    // Add <tileset> element
    pugi::xml_node tilesetNode = mapNode.append_child("tileset");
    tilesetNode.append_attribute("firstgid") = 1;
    tilesetNode.append_attribute("source")   = "../assets/tmx/tsx/grass.tsx";

    // Add <layer> for tile data
    pugi::xml_node layerNode = mapNode.append_child("layer");
    layerNode.append_attribute("name")   = "background";
    layerNode.append_attribute("id")     = "1";
    layerNode.append_attribute("width")  = CHUNK_WIDTH;
    layerNode.append_attribute("height") = CHUNK_HEIGHT;

    // Add tile data as a CSV string inside a <data> node
    pugi::xml_node dataNode = layerNode.append_child("data");
    dataNode.append_attribute("encoding") = "csv";

    // chunks
    for (Chunk& chunk : this->chunks) {
        // Create chunk node
        pugi::xml_node chunkNode = dataNode.append_child("chunk");
        chunkNode.append_attribute("x") = chunk.position.x;
        chunkNode.append_attribute("y") = chunk.position.y;
        chunkNode.append_attribute("width") = CHUNK_WIDTH;
        chunkNode.append_attribute("height") = CHUNK_HEIGHT;

        std::ostringstream csvStream;
        for (Tile& tile : chunk.background_tiles) {
            csvStream << std::to_string(tile.ID) + ",";
        }

        chunkNode.text() = csvStream.str().c_str(); // Set chunk data
    }
    // objects
    pugi::xml_node objectsNode = mapNode.append_child("objectgroup");
    objectsNode.append_attribute("name") = "objects";
    objectsNode.append_attribute("id") = "1";
    pugi::xml_node playerLocationNode = objectsNode.append_child("object");
    playerLocationNode.append_attribute("id") = "1";
    playerLocationNode.append_attribute("name") = "playerLocation";
    playerLocationNode.append_attribute("x") = (int) this->playerPtr->playerView.getCenter().x;
    playerLocationNode.append_attribute("y") = (int) this->playerPtr->playerView.getCenter().y;

    // Save to file
    doc.save_file(filePath.c_str());
}

void World::generateRandomChunk(sf::Vector2f& pos) {
    Chunk tchunk;
    tchunk.position = pos;

    for (int i = 0; i<CHUNK_SIZE; i++) {
        Tile ttile;
        ttile.ID = rand() % 32 + 1;
        ttile.isWalkable = true;
        tchunk.background_tiles[i] = ttile;
    }
    this->chunks.push_back(tchunk);
}

//  processes tmx::tile and returns a tileModel
//  generated from the tmx::tile information
Tile   processTile(const tmx::TileLayer::Tile& tile,   const tmx::Tileset& tileset) {
    Tile tileModel;
    tileModel.ID = tile.ID;
    
    const auto& tileProperties = tileset.getTile(tile.ID)->properties;
    for (const auto& property : tileProperties) {
        if (property.getType() == tmx::Property::Type::Boolean) {
            tileModel.isWalkable = property.getBoolValue();
        }
    }

    return tileModel;
}
Chunk processChunk(const tmx::TileLayer::Chunk& chunk, const tmx::Tileset& tileset) {
    Chunk chunkModel;
    chunkModel.position.x = chunk.position.x;
    chunkModel.position.y = chunk.position.y;

    int tileCounter = 0;
    for (const auto& tile : chunk.tiles) {
        chunkModel.background_tiles[tileCounter] = std::move(processTile(tile, tileset));
        tileCounter++;
    }

    return chunkModel;
}

//  Extracts chunks from map parameter and
//  stores it in chunks vector of World.
//  Also tries to extract playerPosition
//  object and update the playerView pos.
void World::loadMap(tmx::Map& map) {
    if (!map.isInfinite()) {
        std::cout << "Maps of fixed sizes not supported.\n";
        return;
    }
    const auto& layers = map.getLayers(); // get layers

    for (const auto& layer : layers) {
        auto layerType = layer->getType();
        // Tile layer
        if (layerType == tmx::Layer::Type::Tile) {
            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            if (!map.isInfinite()) {
                std::cout << "Map is not infinite.\n";
                return;
            }
            if (tileLayer.getName() != "background") {
                std::cout << "Unknown tilelayer.\n";
                return;
            }
            //  Load background tilesheet
            tmx::Tileset tileset = this->tilesets.at("grass");
            for (const auto& chunk : tileLayer.getChunks()) {   // iterate chunks
                // Create new chunk struct directly in vector
                this->chunks.push_back(std::move(processChunk(chunk, tileset)));  
            }
        }
        // Object layer
        else if (layerType == tmx::Layer::Type::Object) {
            const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();

            if (objectLayer.getName() == "objects") {
                for (const auto& object : objectLayer.getObjects()) { // iterate objects
                    if (this->playerPtr == nullptr) {
                        continue;
                    }
                    if (object.getName() == "playerLocation") {
                        this->playerPtr->move(object.getPosition().x, object.getPosition().y);
                    }
                }
            }
        } else {
            std::cout << "Unknown layer type. \n";
        }
    }
}

// Loops over all chunks in chunks vector and calls createGrassTileSprite for each tile in
// chunk which adds the tilesprite to the tileSprites vector. It actually creates tons of 
// tile sprites and no "chunk" sprites
void World::createChunkSprites(ResourceAllocator& allocator) {
    for (auto& chunk : this->chunks) {
        this->chunkSprites.push_back(createChunkSprite(
            chunk,
            this->tilesets.at("grass"),
            allocator
            )
        );
    }
}

void World::render(sf::RenderWindow &ren) {

    // render chunks
    for (auto& sprite : this->chunkSprites) {
        ren.draw(*sprite);
    }
}