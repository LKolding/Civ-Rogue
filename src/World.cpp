#include "World.hpp"


World::World(ResourceAllocator &allocator) {
    // Initializes a default chunk from .tmx file
    // and extracts all the tilesets provided
    tmx::Map map;
    // Firstly, load the tmx file containing the tileset(s)
    if (!this->loadMap("grass_chunk.tmx", map)) {
        std::cout << std::filesystem::current_path() << "\n";
        throw std::runtime_error("Couldn't load grass_chunk.tmx. World is left uninitialized...");
    }
    // Extract tileset from .tmx file
    const auto& tilesets = map.getTilesets();
    for(const auto& tileset : tilesets) {
        // Load grass texture
        if (tileset.getName() == "grass") {
            allocator.loadTexture(tileset.getImagePath());
            this->tilesets.insert({"grass", tileset});
        }
    }
    generateChunks(map);
}

bool World::loadMap(const std::string& mapName, tmx::Map& map) {
    const std::string mapsPath = "../assets/tmx/maps/";
    if (!map.load(mapsPath+mapName)) {
        return false;
    }
    return true;
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

    /*
    // Example: Fill the tilemap with dummy tile IDs (1 through width*height)
    std::string csvData;
    for (int y = 0; y < CHUNK_HEIGHT; ++y) {
        for (int x = 0; x < CHUNK_WIDTH; ++x) {
            csvData += std::to_string(1) + ",";
        }
        csvData.back() = '\n';  // Replace the last comma with a newline
    }
    dataNode.text().set(csvData.c_str());
    */
    // chunk functionality
    for (Chunk& chunk : this->chunks) {
        // Create chunk node
        pugi::xml_node chunkNode = dataNode.append_child("chunk");
        chunkNode.append_attribute("x") = chunk.position.x; // Set chunk position
        chunkNode.append_attribute("y") = chunk.position.y;
        chunkNode.append_attribute("width") = CHUNK_WIDTH;
        chunkNode.append_attribute("height") = CHUNK_HEIGHT;

    /*
        // Create the CSV string for this chunk
        std::ostringstream csvStream;
        for (int i = 0; i < CHUNK_HEIGHT; ++i) {
            for (int j = 0; j < CHUNK_WIDTH; ++j) {
                // Assuming you have a way to get the tile ID at (j, i) in this chunk
                csvStream << getTileIdAt(chunkX, chunkY, j, i);
                if (j < chunkWidth - 1) {
                    csvStream << ",";
                }
            }
            csvStream << "\n"; // New line for each row
        }*/

        std::ostringstream csvStream;
        for (Tile& tile : chunk.background_tiles) {
            csvStream << std::to_string(tile.ID) + ",";
        }

        chunkNode.text() = csvStream.str().c_str(); // Set chunk data
    }

    // Save to file
    doc.save_file(filePath.c_str());
}

bool World::saveGame(const std::string& gameName) {


    return true;

}

// VERY NICE FUNCTIONS BELOW
// creates a new sprite for a tile.
// allocator is needed to retrieve the shared ptr
// to the tilesheet texture
std::unique_ptr<sf::Sprite> createTileSprite(
    Tile& tile,                 // TILE STRUCT
    tmx::Tileset& tileset,      // TMX TILESET
    sf::Vector2f& pos,          // SPRITE POSITION
    ResourceAllocator& allocator
    ) {
    // Tile sprite
    auto ptileSprite = std::make_unique<sf::Sprite>(); 
    // Tileset tile
    const tmx::Tileset::Tile* tileset_tile = tileset.getTile(tile.ID);
    // extract size and position
    tmx::Vector2u tmx_texture_rect = tileset_tile->imagePosition;
    tmx::Vector2u image_size = tileset_tile->imageSize;
    // create temp IntRect
    sf::IntRect sf_texture_rect;
    sf_texture_rect.top  = tmx_texture_rect.y;
    sf_texture_rect.left = tmx_texture_rect.x;
    sf_texture_rect.width = image_size.x;
    sf_texture_rect.height = image_size.y;
    // update sprite
    ptileSprite->setPosition(pos);
    ptileSprite->setTexture(*allocator.loadTexture("../assets/textures/tilesheets/TX Tileset Grass.png"));
    ptileSprite->setTextureRect(sf_texture_rect);
    // return sprite
    return ptileSprite;
}

// creates a RenderTexture with all tiles drawn to it
std::unique_ptr<sf::RenderTexture> createChunkTexture(
    Chunk& chunk, 
    tmx::Tileset& tileset,
    ResourceAllocator& allocator
    ) {
    // init RenderTexture
    auto rchunkTexture = std::make_unique<sf::RenderTexture>();
    rchunkTexture->create(CHUNK_HEIGHT*TILE_HEIGHT, CHUNK_WIDTH*TILE_WIDTH);
    // iterator vars (used for tilepos)
    sf::Vector2f tileCoords = {0,0}; 
    for (Tile& tile : chunk.background_tiles) {
        auto tileSprite = createTileSprite(
            tile, 
            tileset,
            tileCoords,
            allocator
            );

        rchunkTexture->draw(*tileSprite);
        // update iterator vars
        tileCoords.x += TILE_WIDTH;
        if (tileCoords.x >= CHUNK_WIDTH*TILE_WIDTH) {
            tileCoords.x = 0;
            tileCoords.y += TILE_HEIGHT;
        }

    }
    return rchunkTexture;
}

std::unique_ptr<sf::Sprite> createChunkSprite(
    Chunk& chunk, 
    tmx::Tileset& tileset,
    ResourceAllocator& allocator
) {
    // create texture and store pointer to it
    auto pchunkTex = std::make_shared<sf::Texture>(createChunkTexture(chunk, tileset, allocator)->getTexture());
    std::pair<float,float> chunkLocation = {chunk.position.x, chunk.position.y};
    allocator.addChunkTexturePointer(pchunkTex, chunkLocation);

    // sprite position calculation
    sf::Vector2f spritePos;
    spritePos.x = chunk.position.x * CHUNK_WIDTH  * TILE_WIDTH;
    spritePos.y = chunk.position.y * CHUNK_HEIGHT * TILE_HEIGHT;

    auto pchunkSprite = std::make_unique<sf::Sprite>();
    pchunkSprite->setTexture(*pchunkTex);
    pchunkSprite->setPosition(spritePos);
    return pchunkSprite;
}

Chunk World::generateRandomChunk(sf::Vector2f& pos) {
    Chunk tchunk;
    tchunk.position = pos;

    for (int i = 0; i<CHUNK_SIZE; i++) {
        Tile ttile;
        ttile.ID = rand() % 32 + 1;
        ttile.isWalkable = true;
        tchunk.background_tiles[i] = ttile;
    }
    return tchunk;
}

// Generates chunks from map
void World::generateChunks(tmx::Map& map) {
    // Generate chunk
    Chunk tchunk;
    
    const auto& layers = map.getLayers();
    for(const auto& layer : layers) {
        // Tile layer
        if(layer->getType() == tmx::Layer::Type::Tile) {
            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            int tileCounter = 0;
            for (const auto& tile : tileLayer.getTiles()) {
                if (tileCounter > CHUNK_SIZE) { break; } // shouldn't ever happen, but just in case
                // tile struct to be moved into chunk struct
                Tile tilet;
                tilet.ID = tile.ID;
                // find tile properties
                auto grass_tileset = this->tilesets.at("grass");
                const auto& tileProperties = grass_tileset.getTile(tile.ID)->properties;
                for (const auto& property : tileProperties) {
                    if (property.getType() == tmx::Property::Type::Boolean) {
                        tilet.isWalkable = property.getBoolValue();
                    }
                }
                tchunk.background_tiles[tileCounter] = std::move(tilet);
                tileCounter++;

            }
        }
    }
    this->chunks.push_back(std::move(tchunk));
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