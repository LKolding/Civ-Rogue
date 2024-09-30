#include "ResourceAllocator.hpp"


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
    
    loc.width  = TILE_WIDTH;
    loc.height = TILE_HEIGHT;
    
    int row  = 1 + (tileID / xTilesAmount);
    int tile = 1 + (tileID % xTilesAmount);
    
    loc.left = (TILE_WIDTH * tile);
    loc.top  = (TILE_HEIGHT * row);
    
    return loc;
}