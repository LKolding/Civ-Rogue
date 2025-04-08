#ifndef _COMPONENTS_
#define _COMPONENTS_

#include <SFML/Graphics.hpp>  // for Sprite

struct PositionComponent { float x, y; };

struct ChunkComponent {  };

struct MovementComponent {float speed; float xDir, yDir; };

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
//  when entities are walking left or right (flips the texture horizontically)
struct FlipComponent { bool isFlipped = false; }; 

struct HealthComponent { int currentHealth, maxHealth; };

struct EyeBugStateComponent { enum State { IDLE } state; };

struct NinjaStateComponent { enum State { IDLE, WALK, ACTIVE } state; };

struct CollisionComponent { sf::FloatRect bounds; };

struct StaticCollisionComponent { sf::FloatRect bounds; };

struct WeaponComponent { int damage; float dmgMultiplier; float useTime; float elapsedTime; bool isAttacking; };

struct FollowComponent { bool isFollowing; };

struct HoverComponent { bool isHovered; };

#endif