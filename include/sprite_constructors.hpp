#ifndef _SPRITE_CONSTRUCTORS_
#define _SPRITE_CONSTRUCTORS_
// system
#include <memory>
// sfml
#include <SFML/Graphics.hpp>
//tmx
#include <tmxlite/TileLayer.hpp>
// my stuff
#include "ResourceManager.hpp"
#include "World.hpp"


inline std::unique_ptr<sf::Sprite> createTileSprite(
    TileData& tile,             // TILE STRUCT
    tmx::Tileset& tileset,      // TMX TILESET
    sf::Vector2f& pos,          // SPRITE POSITION
    ResourceManager& allocator
    ) {
    // Tile sprite
    auto ptileSprite = std::make_unique<sf::Sprite>(*allocator.loadTexture(tileset.getImagePath()));//<- texture
    // Tileset tile
    const tmx::Tileset::Tile* tileset_tile = tileset.getTile(tile.ID);
    // extract size and position
    tmx::Vector2u tmx_texture_rect = tileset_tile->imagePosition;
    tmx::Vector2u image_size = tileset_tile->imageSize;
    // update sprite
    ptileSprite->setPosition(pos);
    ptileSprite->setTextureRect({{(int)tmx_texture_rect.x, (int)tmx_texture_rect.y}, {(int)image_size.x, (int)image_size.y}});
    // return sprite
    return ptileSprite;
}

// creates a RenderTexture with all tiles drawn to it
inline std::unique_ptr<sf::RenderTexture> createChunkTexture(
    ChunkData& chunk, 
    tmx::Tileset& tileset,
    ResourceManager& allocator
    ) {
    // init RenderTexture
    auto rchunkTexture = std::make_unique<sf::RenderTexture>();
    if (!rchunkTexture->resize({CHUNK_HEIGHT*TILE_HEIGHT, CHUNK_WIDTH*TILE_WIDTH}))
        printf("Something went wrong\n");
    // iterator vars (used for tilepos)
    sf::Vector2f tileCoords = {0,0}; 
    for (TileData& tile : chunk.background_tiles) {
        auto tileSprite = createTileSprite(
            tile, 
            tileset,
            tileCoords,
            allocator
            );

        rchunkTexture->draw(*tileSprite);
        // update iterator vars
        tileCoords.x += TILE_WIDTH;
        if (tileCoords.x >= CHUNK_WIDTH*TILE_WIDTH) {
            tileCoords.x = 0;
            tileCoords.y += TILE_HEIGHT;
        }

    }
    return rchunkTexture;
}

inline std::unique_ptr<sf::Sprite> createChunkSprite(
    ChunkData& chunk, 
    tmx::Tileset& tileset,
    ResourceManager& allocator
) {
    // create texture and store pointer to it
    auto pchunkTex = std::make_shared<sf::Texture>(createChunkTexture(chunk, tileset, allocator)->getTexture());
    std::pair<float,float> chunkLocation = {chunk.position.x, chunk.position.y};
    allocator.addChunkTexturePointer(pchunkTex, chunkLocation);

    // sprite position calculation
    sf::Vector2f spritePos;
    spritePos.x = chunk.position.x * (TILE_WIDTH * CHUNK_WIDTH);
    spritePos.y = chunk.position.y * (TILE_HEIGHT* CHUNK_HEIGHT);

    auto pchunkSprite = std::make_unique<sf::Sprite>(*pchunkTex);
    //pchunkSprite->setTexture();
    pchunkSprite->setPosition(spritePos);
    return pchunkSprite;
}


#endif