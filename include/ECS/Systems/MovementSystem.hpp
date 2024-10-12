#ifndef _MOVEMENT_SYSTEM_
#define _MOVEMENT_SYSTEM_

#include "ECS/Systems/System.hpp"
#include "ECS/Entities/Entity.hpp"
#include "ECS/Components/Components.hpp"

class MovementSystem: public System {
public:
    inline void update(float deltaTime, std::vector<std::weak_ptr<Entity>> entities) {
        for (auto& entity_p : entities) {
            if (auto entity = entity_p.lock()) {
                // Update position based on velocity
                if (entity->hasComponent<PositionComponent>() && entity->hasComponent<VelocityComponent>()) {
                    // Use getComponent and dereference the shared_ptr to access the underlying component
                    auto posPtr = entity->getComponent<PositionComponent>();
                    auto velPtr = entity->getComponent<VelocityComponent>();
                    // Update position based on direction, speed and deltatime
                    if (posPtr && velPtr) {
                        float x = velPtr->xDir;
                        float y = velPtr->yDir;

                        float length = std::sqrt(x * x + y * y);

                        if (length != 0.0f) {
                            x /= length;
                            y /= length;
                        }

                        posPtr->x += ((velPtr->xSpeed * velPtr->moveAmount) * deltaTime) * x;
                        posPtr->y += ((velPtr->ySpeed * velPtr->moveAmount) * deltaTime) * y;

                    }
                    // Mark the texture as flipped if traveling left or right
                    if (velPtr->xDir != 0.0f && entity->hasComponent<FlipComponent>()) {
                        if (velPtr->xDir == 1.0f) {
                            entity->getComponent<FlipComponent>()->isFlipped = false;
                        } else 
                        if (velPtr->xDir == -1.0f){
                            entity->getComponent<FlipComponent>()->isFlipped = true;
                        }
                    }
                }
                // update sprite position
                if (entity->hasComponent<PositionComponent>() && entity->hasComponent<SpriteComponent>()) {
                    entity->getComponent<SpriteComponent>()->sprite.setPosition(entity->getComponent<PositionComponent>()->x, entity->getComponent<PositionComponent>()->y);
                }
                // update collisionbox position
                if (entity->hasComponent<PositionComponent>() && entity->hasComponent<CollisionComponent>()) {
                    //entity->getComponent<CollisionComponent>()->bounds.left = entity->getComponent<PositionComponent>()->x - entity->getComponent<CollisionComponent>()->bounds.getSize().x;
                    //entity->getComponent<CollisionComponent>()->bounds.top = entity->getComponent<PositionComponent>()->y;
                    //entity->getComponent<CollisionComponent>()->bounds = sf::FloatRect(sf::Vector2f(entity->getComponent<PositionComponent>()->x - entity->getComponent<CollisionComponent>()->bounds.getSize().x, entity->getComponent<PositionComponent>()->y - entity->getComponent<CollisionComponent>()->bounds.getSize().y), sf::Vector2f(entity->getComponent<CollisionComponent>()->bounds.getSize()));
                }
                /*
                // update position
                // Use getComponent and dereference the shared_ptr to access the underlying component
                    auto colPtr = entity->getComponent<CollisionComponent>();
                    auto posPtr = entity->getComponent<PositionComponent>();
                    // half of texture (COLLISION BOX) width
                    auto texWidth = colPtr->bounds.width/2;
                    // half of texture (COLLISION BOX) height
                    auto texHeight = colPtr->bounds.height/2;
                    colPtr->bounds.left = posPtr->x - texWidth;
                    colPtr->bounds.top  = posPtr->y - texHeight;
                
                */
            }
        }
    }
};

#endif