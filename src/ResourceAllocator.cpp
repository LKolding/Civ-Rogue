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