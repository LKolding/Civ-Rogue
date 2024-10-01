#include "World.hpp"


World::World(ResourceAllocator &allocator) {
    // Initializes a default chunk from .tmx file
    // and extracts all the tilesets provided
    tmx::Map map;
    // Firstly, load the tmx file containing the tileset(s)

    //   // previous value: ../assets/tmx/maps/grass_chunk.tmx
    if (!map.load("../saveGames/game1.tmx")) {
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
        chunkNode.append_attribute("x") = chunk.position.x * CHUNK_WIDTH;
        chunkNode.append_attribute("y") = chunk.position.y * CHUNK_HEIGHT;
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
    playerLocationNode.append_attribute("x") = this->playerPtr->playerView.getCenter().x;
    playerLocationNode.append_attribute("y") = this->playerPtr->playerView.getCenter().y;

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
    const auto& layers = map.getLayers();
    for (const auto& layer : layers) {
        auto layerType = layer->getType();
        // Tile layer
        if (layerType == tmx::Layer::Type::Tile) {
            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            if (!map.isInfinite()) {
                std::cout << "Map is not infinite. Undefined behavior is about to commence...\n";
            }
            for (const auto& chunk : tileLayer.getChunks()) {   // iterate chunks
                // find tileset
                tmx::Tileset tileset = this->tilesets.at("grass");
                this->chunks.push_back(std::move(processChunk(chunk, tileset)));
            }
        }
        // Object layer
        else if (layerType == tmx::Layer::Type::Object) {
            const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
            if (objectLayer.getName() == "objects") {
                for (const auto& object : objectLayer.getObjects()) {
                    if (this->playerPtr == nullptr) {
                        continue;
                    }
                    if (object.getName() == "playerLocation") {
                        float x = object.getPosition().x;
                        float y = object.getPosition().y;
                        this->playerPtr->playerView.setCenter(x,y);
                    }
                }
            }
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

void World::setPlayer(Player& p) {
    this->playerPtr = std::make_shared<Player>(p);
}

void World::render(sf::RenderWindow &ren) {

    // render chunks
    for (auto& sprite : this->chunkSprites) {
        ren.draw(*sprite);
    }
}