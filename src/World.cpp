#include "World.hpp"


World::World(ResourceAllocator &allocator) {
    // Initializes a default chunk from .tmx file
    // and extracts all the tilesets provided

    // Firstly, load the tmx file containing the tileset(s)
    if (!this->loadMap("assets/tmx/maps/grass_chunk.tmx")) {
        throw std::runtime_error("Couldn't load grass_chunk.tmx. World is left uninitialized...");
    }

    // Extract tileset from .tmx file
    const auto& tilesets = map.getTilesets();
    for(const auto& tileset : tilesets) {
        // Load grass texture
        if (tileset.getName() == "grass") {
            allocator.loadTexture(tileset.getImagePath());
        }
    }
}


bool World::loadMap(const std::string& path) {
    if (!this->map.load(path)) {
        return false;
    }
    return true;
}


void World::render(sf::RenderTarget &ren) const {
    const auto& layers = map.getLayers();
    for(const auto& layer : layers)
    {
        if(layer->getType() == tmx::Layer::Type::Object)
        {
            const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
            const auto& objects = objectLayer.getObjects();
            for(const auto& object : objects)
            {
                //do stuff with object properties
            }
        }
        else if(layer->getType() == tmx::Layer::Type::Tile)
        {
            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            //read out tile layer properties etc...
        }
    }
}