// TMX
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

// SFML
#include <SFML/Graphics.hpp>

// SYSTEM
#include <string>
#include <vector>

// MY STUFF
#include <ResourceAllocator.hpp>


class World {

    tmx::Map map;

    World(ResourceAllocator &allocator);

    bool loadMap(const std::string& path);

    void render(sf::RenderTarget &ren) const;

};