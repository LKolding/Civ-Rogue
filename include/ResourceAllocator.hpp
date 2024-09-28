#include <SFML/Graphics.hpp>

#include <memory>
#include <string>
#include <vector>


class ResourceAllocator {
public:
    // Load a texture; if it's already loaded, return a shared pointer to it
    std::shared_ptr<sf::Texture> loadTexture(const std::string& filename);

private:
    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;
};