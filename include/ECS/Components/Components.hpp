#ifndef _COMPONENTS_
#define _COMPONENTS_

#include <SFML/Graphics.hpp>  // for Sprite
// World space
struct PositionComponent { float x, y; };
// A component containing the coordinates of the chunk in which
// the entity is currently within. Maintained by MovementSystem
struct ChunkComponent { int x, y; };

struct MovementComponent {float speed; float xDir, yDir; };

struct HealthComponent { int currentHealth, maxHealth; };

struct WeaponComponent { int damage; float dmgMultiplier; float useTime; float elapsedTime; bool isAttacking; };

struct BoundsComponent { sf::FloatRect bounds; };  // maintained by RenderSystem

struct SpriteComponent {
    sf::IntRect textureRectangle;
    sf::Vector2f origin;
    std::string texturePath;
    bool hasBeenFlipped = false; 
    bool isVisible = true; 
};

struct AnimationComponent { 
    float elapsedTime; 
    float frameTime; 
    int animationIndex; 
    int animationIndexMax; 
};
// When entities are walking left or right 
// (flips the texture horizontically)
struct FlipComponent { bool isFlipped = false; }; 

// State components crucial for, and directly tied to, sprite animations 
struct EyeBugStateComponent { enum State { IDLE } state; };
struct NinjaStateComponent { enum State { IDLE, WALK, ACTIVE } state; };

// Collision components
struct CollisionComponent { bool isStaticBody = false; };

// Entity behavior components
struct ControlComponent {};
// View can follow entity
struct FollowComponent { bool isFollowing; };
// Can be hovered
struct HoverComponent { bool isHovered; };

#endif