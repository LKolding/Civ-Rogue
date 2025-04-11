#ifndef _COMPONENTS_
#define _COMPONENTS_

#include <SFML/Graphics.hpp>  // for Sprite
// World space
struct PositionComponent { float x, y; };
// A component containing the coordinates of the chunk in which
// the entity is currently within. Maintained by MovementSystem
struct ChunkComponent { int x, y; };

struct MovementComponent { float speed; float xDir, yDir; };

struct HealthComponent { int currentHealth, maxHealth; };

struct WeaponComponent { int damage; float dmgMultiplier; float useTime; float elapsedTime; bool isAttacking; };

struct BoundsComponent { sf::FloatRect bounds; };  // maintained by RenderSystem

struct SpriteComponent {
    sf::IntRect textureRectangle; // rect of the tilesheet
    sf::Vector2f origin;
    std::string texturePath;      // path to tilesheet
    bool hasBeenFlipped = false; 
    bool isVisible = true; 
};
// ---------------------
// ----- Animation -----
// ---------------------
struct AnimationComponent { 
    float elapsedTime; 
    float frameTime; 
    int animationIndex; 
    int animationIndexMax; 
};
struct AnimationFrameData { // single animation (notice: this isn't a component!)
    int rowIndex;
    int frameCount;
};
struct AnimationMapComponent { //maps states to animations (AnimationFrameData)
    std::unordered_map<std::string, AnimationFrameData> stateToAnimation;
    int textureHeightPerRow;
};
// When entities are walking left or right 
// (flips the texture horizontically)
struct FlipComponent { bool isFlipped = false; }; 

// ----------------------------
// ----- State components -----
// ----------------------------
// State components crucial for, and directly tied to, sprite animations 
struct EyeBugStateComponent { enum State { IDLE } state; };
struct StateComponent { std::string currentState; };

// -------------------------------
// ----- Collision component -----
// -------------------------------
// (bounds is stored in BoundsComponent)
struct CollisionComponent { bool isStaticBody = false; };

// --------------------------------------
// ----- Entity behavior components -----
// --------------------------------------
// Makes entity controllable
struct ControlComponent {};
// View can follow entity
struct FollowComponent { bool isFollowing = false; };
// Can be hovered
struct HoverComponent { bool isHovered = false; };
// Entities can have an objective (a point in world space),
// and will move towards the point, if hasObjective
struct ObjectiveComponent { float x, y; bool hasObjective = false; };

#endif