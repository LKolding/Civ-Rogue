#include "ResourceAllocator.hpp"

void ResourceAllocator::loadTilesetsFromTMX(std::string filepath) {
    tmx::Map map;

    if (!map.load(filepath)) {
        throw std::runtime_error("Couldn't load. World is left uninitialized...");
    }
    // Extract tileset(s) from .tmx file
    const auto& tilesets = map.getTilesets();
    for(const auto& tileset : tilesets) {
        this->addTileset(tileset);
    }
}

ResourceAllocator::ResourceAllocator() {
    if (!this->default_font.openFromFile("../assets/fonts/BD_Cartoon_Shout.ttf"))
        printf("[Error] Couldn't load font\n");

    loadTilesetsFromTMX("../assets/tmx/maps/tilesets.tmx");
}

std::shared_ptr<sf::Texture> ResourceAllocator::loadTexture(const std::string& filename) {
        auto it = textures.find(filename);
        if (it != textures.end()) {
            return it->second; // Return existing texture if already loaded
        }

        // Otherwise, load a new texture
        auto texture = std::make_shared<sf::Texture>();
        if (texture->loadFromFile(filename)) {
            textures[filename] = texture; // Store and return the texture
            return texture;
        }

        return nullptr; // Return null if loading failed
    }

sf::Vector2u ResourceAllocator::getSizeOfTexture(std::string filename) {
    return this->textures[filename].get()->getSize();
}

sf::IntRect ResourceAllocator::locateTexOnSheet(int tileID, std::string filename) {
    sf::IntRect loc;
    
    sf::Vector2u sheet_size = this->getSizeOfTexture(filename);
    const int xTilesAmount = sheet_size.x / TILE_WIDTH;
    
    loc.size.x  = TILE_WIDTH;
    loc.size.y = TILE_HEIGHT;
    
    int row  = 1 + (tileID / xTilesAmount);
    int tile = 1 + (tileID % xTilesAmount);
    
    loc.position.x  = (TILE_WIDTH * tile);
    loc.position.y  = (TILE_HEIGHT * row);
    
    return loc;
}

void ResourceAllocator::addTileset(const tmx::Tileset &tileset) {
    this->loadTexture(tileset.getImagePath());
    this->tilesets.insert({tileset.getName(), std::make_shared<tmx::Tileset>(tileset)});
}

tmx::Tileset& ResourceAllocator::getTileset(std::string name) {
    return *this->tilesets[name];
}

void ResourceAllocator::addChunkTexturePointer(std::shared_ptr<sf::Texture> tex, std::pair<float,float>& pos) {
    chunksTextures[pos] = tex;
}