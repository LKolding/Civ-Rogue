#include "ResourceManager.hpp"

// May throw exception
void ResourceManager::loadTilesetsFromTMX(const std::string& filepath) {
    tmx::Map map;

    if (!map.load(filepath))
        throw std::runtime_error("[GAME ERROR] Couldn't load. World is left uninitialized...");
    
    // Extract tileset(s) from .tmx file
    const auto& tilesets = map.getTilesets();
    for(const auto& tileset : tilesets) {
        this->addTileset(tileset);
    }
}

ResourceManager::ResourceManager() {
    if (!this->default_font.openFromFile("../assets/fonts/BD_Cartoon_Shout.ttf"))
        throw std::runtime_error("[GAME ERROR] Couldn't load font");

    loadTilesetsFromTMX("../assets/tilesets/tilesets.tmx");
}

std::shared_ptr<sf::Texture> ResourceManager::loadTexture(const std::string& filename) {
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

sf::Vector2u ResourceManager::getSizeOfTexture(const std::string& filename) {
    return this->textures[filename].get()->getSize();
}

sf::IntRect ResourceManager::locateTexOnSheet(int tileID, std::string filename) {
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

void ResourceManager::addShader(const std::string& filename) {
    std::shared_ptr<sf::Shader> shader = std::make_shared<sf::Shader>();
     
    if (!shader->loadFromFile(filename, sf::Shader::Type::Fragment))
        std::cout << "Could not load shaderfile\n";

    this->m_shaders[filename] = shader;
}
// Should probably return weak ptr ?
sf::Shader* ResourceManager::getShader(const std::string& filename) {
    return this->m_shaders[filename].get();
}

void ResourceManager::addTileset(const tmx::Tileset &tileset) {
    this->loadTexture(tileset.getImagePath());
    this->tilesets.insert({tileset.getName(), std::make_shared<tmx::Tileset>(tileset)});
}

tmx::Tileset& ResourceManager::getTileset(const std::string& name) {
    return *this->tilesets[name];
}

void ResourceManager::addChunkTexturePointer(std::shared_ptr<sf::Texture> tex, std::pair<float,float>& pos) {
    chunksTextures[pos] = tex;
}