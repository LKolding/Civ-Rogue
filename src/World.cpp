#include "World.hpp"

World::World(ResourceAllocator &allocator) {
    // Initializes a default chunk from .tmx file
    // and extracts all the tilesets provided

    // Firstly, load the tmx file containing the tileset(s)
    if (!this->loadMap("grass_chunk.tmx")) {
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
}


bool World::loadMap(const std::string& mapName) {
    const std::string mapsPath = "../assets/tmx/maps/";
    if (!this->map.load(mapsPath+mapName)) {
        return false;
    }
    return true;
}

// VERY NICE FUNCTION BELOW
// creates a new sprite for a tile.
// allocator is needed to retrieve the shared ptr
// to the tilesheet texture
sf::Sprite createTileSprite(
    Tile tile,                 // TILE STRUCT
    tmx::Tileset tileset,      // TMX TILESET
    sf::Vector2f pos,          // SPRITE POSITION
    ResourceAllocator& allocator
    ) {
    // Tile sprite
    sf::Sprite tileSprite; 
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
    tileSprite.setPosition(pos);
    tileSprite.setTexture(*allocator.loadTexture("../assets/textures/tilesheets/TX Tileset Grass.png"));
    tileSprite.setTextureRect(sf_texture_rect);
    // return sprite
    return tileSprite;
}

// Generates chunks from "map" member of World
void World::generateChunks(void) {
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

// Loops over all chunks in chunks vector and
// calls createGrassTileSprite for each tile in
// chunk which adds the tilesprite to the (other) vector
void World::createChunkSprites(Chunk& chunk, ResourceAllocator& allocator) {
    int y = 0;
    int x = 0;

    for (Tile& tile : chunk.background_tiles) {
        
        sf::Vector2f pos = {(float)x*TILE_WIDTH,(float)y*TILE_HEIGHT};

        // create sprite at sprites vector
        this->tileSprites.push_back(createTileSprite(
            tile,
            this->tilesets.at("grass"),
            pos,
            allocator
        ));

        x++;
        if (x >= CHUNK_WIDTH) {
            x = 0;
            y++;
        }
    }
}

void World::render(sf::RenderWindow &ren) {
    for (auto& sprite : this->tileSprites) {
        ren.draw(sprite);
    }
}