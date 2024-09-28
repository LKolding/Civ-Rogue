#include "World.hpp"


void World::createWorld() {
    
    // Extract tileset from .tmx file
    const auto& tilesets = map.getTilesets();
    for(const auto& tileset : tilesets)
    {
        // Grass tileset
        if (tileset.getName() == "grass") {
            
        }
    }
}


bool World::loadMap(std::string path) {
    if (!this->map.load(path)) {
        return false;
    }
    return true;
}


void World::render(sf::RenderTarget &ren) {
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