#ifndef _COMPONENTS_
#define _COMPONENTS_

#include <SFML/Graphics.hpp>  // for Sprite

struct PositionComponent { float x, y; };

struct VelocityComponent { 
    // constant value
    float xSpeed, ySpeed, deceleration;
    // variable
    float xDir, yDir; 
    };

struct SpriteComponent { sf::Sprite sprite; };

struct HealthComponent { int currentHealth, maxHealth; };

struct AnimationComponent { float elapsedTime; float frameTime; int animationIndex; int animationIndexMax; };

struct MageStateComponent { enum State { IDLE, WALK, REST, ATTACK, ATTACK2, ATTACK3, TEMP, DEATH } state; };

struct NinjaStateComponent { enum State { IDLE, WALK } state; };
//  holds a location 
struct ObjectiveComponent { std::unique_ptr<sf::Vector2f> location; };

struct InventoryComponent { std::vector<std::string> items; };

struct InteractableComponent {};

struct SelectableComponent { float isSelected; };

struct CollisionComponent { sf::FloatRect bounds; };

struct UUIDComponent { uint64_t ID; };
#endif