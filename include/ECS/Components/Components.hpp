#ifndef _COMPONENTS_
#define _COMPONENTS_

#include <SFML/Graphics.hpp>  // for Sprite

struct PositionComponent { float x, y; };

struct VelocityComponent { float xSpeed, ySpeed; };

struct SpriteComponent { sf::Sprite sprite; };

struct HealthComponent { int currentHealth, maxHealth; };

struct AnimationComponent { float elapsedTime; int animationIndex; };

struct StateComponent { enum State { IDLE, WALK, REST, ATTACK, ATTACK2, ATTACK3, TEMP, DEATH } state; };

struct InventoryComponent { std::vector<std::string> items; };

struct InteractableComponent {};

struct SelectableComponent { float isSelected; };

struct CollisionComponent { sf::FloatRect bounds; };
#endif