#ifndef _ANIMATION_SYSTEM_
#define _ANIMATION_SYSTEM_

#include <vector>

#include "ECS/Entities/Entity.hpp"


class AnimationSystem {
public:
    void update(float deltaTime, std::vector<std::unique_ptr<Entity>>& entities) {
        for (auto& entity : entities) {
            // Check if the entity has both components
            if (entity->hasComponent<AnimationComponent>()) {
                //  update sprite texture rectangle to match up with the animation index of AnimationComponent
                sf::IntRect textRect = entity->getComponent<SpriteComponent>()->sprite.getTextureRect(); // get
                textRect.left = 32 * entity->getComponent<AnimationComponent>()->animationIndex;         // update frame index     (x)
                textRect.top = 32 * entity->getComponent<StateComponent>()->state;                       // update animation index (y)
                entity->getComponent<SpriteComponent>()->sprite.setTextureRect(textRect);                // set
                //  update animation component's elapsed time
                entity->getComponent<AnimationComponent>()->elapsedTime += deltaTime;
                //  update animation component animationIndex (is enough time has passed)
                if (entity->getComponent<AnimationComponent>()->elapsedTime >= entity->getComponent<AnimationComponent>()->frameTime) {
                    entity->getComponent<AnimationComponent>()->elapsedTime = 0.0f;

                    const int animationSheetWidth = entity->getComponent<SpriteComponent>()->sprite.getTexture()->getSize().x;
                    const int animationIndex = entity->getComponent<AnimationComponent>()->animationIndex;
                    if (
                        (animationIndex >= (animationSheetWidth/32)-1) | (animationIndex >= entity->getComponent<AnimationComponent>()->animationIndexMax)
                        ) {
                        entity->getComponent<AnimationComponent>()->animationIndex = 0;  //  reset
                    }
                    else {
                        entity->getComponent<AnimationComponent>()->animationIndex += 1; //  increment
                    }
                }
            }
        }
    }
};

#endif