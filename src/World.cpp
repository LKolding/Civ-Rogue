#include "World.hpp"


void World::initialize(std::weak_ptr<ResourceAllocator> allocator, std::weak_ptr<Player> p, std::string& game_name) {
    // Performs an initialization of the World object incl.
    // tilesets, chunks and tiles. Extracts from game file
    tmx::Map map;
    this->playerPtr = p;

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

    /*
    <tileset firstgid="1" name="grass" tilewidth="32" tileheight="32" tilecount="64" columns="8">
  <image source="../assets/textures/tilesheets/TX Tileset Grass.png" width="256" height="256"/>
    
    */

    // Add grass <tileset> element
    pugi::xml_node tilesetNode = mapNode.append_child("tileset");
    tilesetNode.append_attribute("firstgid") = 1;
    tilesetNode.append_attribute("name") = "grass";
    tilesetNode.append_attribute("tilewidth") = 32;
    tilesetNode.append_attribute("tileheight") = 32;
    tilesetNode.append_attribute("source")   = "../assets/tmx/tsx/grass.tsx";
    pugi::xml_node imageNode = tilesetNode.append_child("image");
    imageNode.append_attribute("source") = "../assets/textures/tilesheets/TX Tileset Grass.png";

    // Add health bar <tileset> element
    pugi::xml_node tilesetNode2 = mapNode.append_child("tileset");
    tilesetNode2.append_attribute("firstgid")  = 65;
    tilesetNode2.append_attribute("name") = "health_bars";
    tilesetNode2.append_attribute("tilewidth") = 48;
    tilesetNode2.append_attribute("tileheight")= 16;
    tilesetNode2.append_attribute("source")    = "../assets/tmx/tsx/health_bars.tsx";
    pugi::xml_node imageNode2 = tilesetNode2.append_child("image");
    imageNode2.append_attribute("source") = "../assets/textures/UI/health_bars.png";

    // Add melly_tileset <tileset> element
    pugi::xml_node tilesetNode3 = mapNode.append_child("tileset");
    tilesetNode3.append_attribute("firstgid")  = 129;
    tilesetNode3.append_attribute("name") = "health_bars";
    tilesetNode3.append_attribute("tilewidth") = 48;
    tilesetNode3.append_attribute("tileheight")= 16;
    tilesetNode3.append_attribute("source")    = "../assets/tmx/tsx/melly_tileset.tsx";
    pugi::xml_node imageNode3 = tilesetNode3.append_child("image");
    imageNode3.append_attribute("source") = "../assets/textures/tilesheets/melly_tileset.png";

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
    for (tmx::TileLayer::Chunk& chunk : this->chunks) {
        // Create chunk node
        pugi::xml_node chunkNode = dataNode.append_child("chunk");
        chunkNode.append_attribute("x") = chunk.position.x;
        chunkNode.append_attribute("y") = chunk.position.y;
        chunkNode.append_attribute("width") = CHUNK_WIDTH;
        chunkNode.append_attribute("height") = CHUNK_HEIGHT;

        std::ostringstream csvStream;
        for (tmx::TileLayer::Tile& tile : chunk.tiles) {
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
    playerLocationNode.append_attribute("x") = (int) this->playerPtr.lock()->playerView.getCenter().x;
    playerLocationNode.append_attribute("y") = (int) this->playerPtr.lock()->playerView.getCenter().y;

    // Save to file
    doc.save_file(filePath.c_str());
}

// pos argument is in amount of tiles. The tile to pixel
// conversion happens in this function
void World::generateRandomChunk(sf::Vector2f pos) {
    tmx::TileLayer::Chunk tchunk;
    tchunk.position = tmx::Vector2i(pos.x, pos.y);
    tchunk.size = tmx::Vector2i(CHUNK_WIDTH, CHUNK_HEIGHT);

    for (int i = 0; i<CHUNK_SIZE; i++) {
        tmx::TileLayer::Tile ttile;
        ttile.ID = rand() % 30 + 1;
        tchunk.tiles.push_back(ttile);
    }
    this->chunks.push_back(std::move(tchunk));
}

//  processes tmx::tile and returns a tile struct
//  generated from the tmx::tile information
Tile processTile(const tmx::TileLayer::Tile& tile,   const tmx::Tileset& tileset) {
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
    const auto& layers = map.getLayers();     // get layers

    for (const auto& layer : layers) {        // iterate layers
        switch (layer->getType()) {

        case tmx::Layer::Type::Tile: {
            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            for (const auto& chunk : tileLayer.getChunks()) {   // iterate chunks
                this->chunks.push_back(chunk);  
            }
            break;
        }

        case tmx::Layer::Type::Object: {
            const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
            if (objectLayer.getName() == "objects") {
                for (const auto& object : objectLayer.getObjects()) {  // iterate objects
                
                }
            }
            break;
        }
            
        default:
            break;
        }
    }
}

void World::createChunkSprites(std::shared_ptr<ResourceAllocator> allocator) {
    for (auto& chunk : this->chunks) {
        this->chunkSprites.push_back(createChunkSprite(
            chunk,
            allocator->getTileset("grass"),
            *allocator
            )
        );
    }
}

void World::render(std::unique_ptr<sf::RenderWindow>& ren) {    
    // render chunks
    for (auto& sprite : this->chunkSprites) {
        ren->draw(*sprite);
    }
}