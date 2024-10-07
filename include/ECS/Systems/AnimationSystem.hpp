#ifndef _ANIMATION_SYSTEM_
#define _ANIMATION_SYSTEM_

#include <vector>

#include "ECS/Systems/System.hpp"
#include "ECS/Entities/Entity.hpp"


class AnimationSystem: public System {
public:
    inline void update(float deltaTime, std::vector<std::weak_ptr<Entity>> entities) override {
        for (auto& entity_p : entities) {
            // Check if the entity has both components
            if (auto entity = entity_p.lock()) {
                if (entity->hasComponent<AnimationComponent>()) {
                //  update sprite texture rectangle to match up with the animation index of AnimationComponent
                sf::IntRect textRect = entity->getComponent<SpriteComponent>()->sprite.getTextureRect(); // get
                textRect.left = textRect.width * entity->getComponent<AnimationComponent>()->animationIndex;         // update frame index     (x)
                
                if (entity->hasComponent<MageStateComponent>()) {
                    textRect.top = textRect.height * entity->getComponent<MageStateComponent>()->state;               // update animation index (y)
                }

                if (entity->hasComponent<NinjaStateComponent>()) {
                    textRect.top = textRect.height * entity->getComponent<NinjaStateComponent>()->state;               // update animation index (y)
                }

                entity->getComponent<SpriteComponent>()->sprite.setTextureRect(textRect);                // set
                //  update animation component's elapsed time
                entity->getComponent<AnimationComponent>()->elapsedTime += deltaTime;
                //  update animation component animationIndex (is enough time has passed)
                if (entity->getComponent<AnimationComponent>()->elapsedTime >= entity->getComponent<AnimationComponent>()->frameTime) {
                    entity->getComponent<AnimationComponent>()->elapsedTime = 0.0f;

                    const int animationSheetWidth = entity->getComponent<SpriteComponent>()->sprite.getTexture()->getSize().x;
                    const int animationIndex = entity->getComponent<AnimationComponent>()->animationIndex;
                    if (
                        (animationIndex >= (animationSheetWidth/textRect.width)-1) | (animationIndex >= entity->getComponent<AnimationComponent>()->animationIndexMax)
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
    }
};

#endif