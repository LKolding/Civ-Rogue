#ifndef _RESOURCE_ALLOC_
#define _RESOURCE_ALLOC_
#include <SFML/Graphics.hpp>

#include <tmxlite/Map.hpp>

// pugi
#include "../src/tmxlite/detail/pugixml.hpp"

#include <memory>
#include <string>
#include <vector>

#include "hash_function.h"
#include "constants.h" // for tile and chunk sizes

class ResourceAllocator {
public:
    sf::Font default_font;
    ResourceAllocator(); // inits font
    // Load a texture; if it's already loaded, return a shared pointer to it
    std::shared_ptr<sf::Texture> loadTexture(const std::string& filename);
    sf::Vector2u getSizeOfTexture(std::string filename);

    sf::IntRect locateTexOnSheet(int ID, std::string filename);

    void loadTilesetsFromTMX(std::string filepath);
    void addChunkTexturePointer(std::shared_ptr<sf::Texture> tex, std::pair<float,float>& pos);
    void addTileset(const tmx::Tileset &tileset);
    tmx::Tileset& getTileset(std::string name);

private:
    // holds pointers to pre-rendered chunk textures
    std::unordered_map<std::pair<float,float>, std::shared_ptr<sf::Texture>, hash_pair> chunksTextures;
    std::unordered_map<std::string,  std::shared_ptr<tmx::Tileset>> tilesets;
    std::unordered_map<std::string,  std::shared_ptr<sf::Texture>>  textures;
};
#endif