#ifndef _COMPONENTS_
#define _COMPONENTS_

#include <SFML/Graphics.hpp>  // for Sprite
struct PositionComponent { float x, y; };

struct VelocityComponent { 
    // constant values
    float xSpeed, ySpeed;
    float moveAmount; // amount in pixels
    // variables
    float xDir, yDir; 
};

struct SpriteComponent { sf::Sprite sprite; bool hasBeenFlipped = false; bool isVisible = true; };
//  when entities are walking left or right (flips the texture)
struct FlipComponent { bool isFlipped = false; }; 

struct HealthComponent { int currentHealth, maxHealth; };

struct AnimationComponent { float elapsedTime; float frameTime; int animationIndex; int animationIndexMax; };

struct EyeBugStateComponent { enum State { IDLE } state; };

struct NinjaStateComponent { enum State { IDLE, WALK, ACTIVE } state; };

struct CollisionComponent { sf::FloatRect bounds; };

struct StaticCollisionComponent { sf::FloatRect bounds; };

struct DeletableComponent { bool markedForDeletion; };

struct UUIDComponent { uint64_t ID; };

struct LifetimeComponent { float timeAlive; float deathTime; };

struct WeaponComponent { int damage; float useTime; float dmgMultiplier; bool isAttacking; };

#endif