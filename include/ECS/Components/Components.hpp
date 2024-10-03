#ifndef _COMPONENTS_
#define _COMPONENTS_

#include <SFML/Graphics.hpp>

struct PositionComponent { float x, y; };

struct VelocityComponent { float xSpeed, ySpeed; };

struct SpriteComponent { sf::Sprite sprite; };

struct HealthComponent { int currentHealth, maxHealth; };

struct AIComponent { enum State { IDLE, PATROL, CHASE, ATTACK } state; };

struct InventoryComponent { std::vector<std::string> items; };

struct InteractableComponent {};

struct CollisionComponent { sf::FloatRect bounds; };
#endif