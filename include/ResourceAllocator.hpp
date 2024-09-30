#ifndef _RESOURCE_ALLOC_
#define _RESOURCE_ALLOC_
#include <SFML/Graphics.hpp>

#include <memory>
#include <string>
#include <vector>

#include "constants.h" // for tile and chunk sizes

class ResourceAllocator {
public:
    // Load a texture; if it's already loaded, return a shared pointer to it
    std::shared_ptr<sf::Texture> loadTexture(const std::string& filename);
    sf::Vector2u getSizeOfTexture(std::string filename);

    sf::IntRect locateTexOnSheet(int ID, std::string filename);

private:
    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;
};
#endif