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


class World {

    tmx::Map map;

    void createWorld();

    bool loadMap(std::string path);

    void render(sf::RenderTarget &ren);

};