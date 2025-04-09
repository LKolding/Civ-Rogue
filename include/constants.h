#ifndef _CONSTANTS_
#define _CONSTANTS_

#define WINDOW_WIDTH  1920 * 0.7 // Size in pixels
#define WINDOW_HEIGHT 1080 * 0.7 // Size in pixels

#define TILE_WIDTH  32                   // Size in pixels
#define TILE_HEIGHT 32                   // Size in pixels
#define TILE_SIZE TILE_WIDTH*TILE_HEIGHT // Size in pixels

#define CHUNK_WIDTH  16                     // Size in (amount of) tiles
#define CHUNK_HEIGHT 16                     // Size in (amount of) tiles
#define CHUNK_SIZE CHUNK_WIDTH*CHUNK_HEIGHT // Size in (amount of) tiles

// sf::View (player view values)
#define PANNING_SPEED 300
#define viewMinX 1920/4    // View minimum X length
#define viewMaxX 1920/2    // View maximum X length
#define viewMinY 1080/4    // View minimum Y length
#define viewMaxY 1080/2    // View maximum Y length

//  ENTITY_ID
#define EntityID uint32_t

#endif