#ifndef _ANIMATION_SYSTEM_
#define _ANIMATION_SYSTEM_

#include <vector>

#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"
#include "ECS/Components/Components.hpp"

#include "constants.h"

class AnimationSystem {
public:
    inline void update(float deltaTime, EntityManager& em, ComponentManager& cm) {
        for (auto& ent : em.getAllEntities()) {
            // Check for AnimationComponent and SpriteComponent
            if (!cm.getComponent<AnimationComponent>(ent) | !cm.getComponent<SpriteComponent>(ent)) {
                return;
            }
            // Create texture rectangle (sf::IntRect) to manipulate and later apply to the component
            sf::IntRect textRect = cm.getComponent<SpriteComponent>(ent)->textureRectangle;

            // Calculate
            if (cm.getComponent<AnimationComponent>(ent)) {
                textRect.left = textRect.width * cm.getComponent<AnimationComponent>(ent)->animationIndex;
            }
            if (cm.getComponent<NinjaStateComponent>(ent)) {
                textRect.top = textRect.height * cm.getComponent<NinjaStateComponent>(ent)->state;
            }

            // Update components
            cm.getComponent<SpriteComponent>(ent)->textureRectangle = textRect;
            cm.getComponent<AnimationComponent>(ent)->elapsedTime += deltaTime;

            if (cm.getComponent<AnimationComponent>(ent)->elapsedTime >= cm.getComponent<AnimationComponent>(ent)->frameTime) {
                cm.getComponent<AnimationComponent>(ent)->elapsedTime = 0.0f;

                // TODO add below in the if clause
                // animationIndex >= (animationSheetWidth/textRect.width)-1) |
                if (cm.getComponent<AnimationComponent>(ent)->animationIndex >= cm.getComponent<AnimationComponent>(ent)->animationIndexMax) {
                    cm.getComponent<AnimationComponent>(ent)->animationIndex = 0;

                } else {
                    cm.getComponent<AnimationComponent>(ent)->animationIndex++;
                }
            }
        }
    }
    /*
    inline void update2(float deltaTime, std::vector<std::weak_ptr<Entity>> entities) {
        for (auto& entity_p : entities) {
            // Check if the entity has both components
            if (auto entity = entity_p.lock()) {
                if (entity->hasComponent<AnimationComponent>()) {
                //  update sprite texture rectangle to match up with the animation index of AnimationComponent
                sf::IntRect textRect = entity->getComponent<SpriteComponent>()->sprite.getTextureRect(); // get
                textRect.left = textRect.width * entity->getComponent<AnimationComponent>()->animationIndex;         // update frame index     (x)

                if (entity->hasComponent<NinjaStateComponent>()) {
                    textRect.top = textRect.height * entity->getComponent<NinjaStateComponent>()->state;             // update animation index (y)
                }

                entity->getComponent<SpriteComponent>()->sprite.setTextureRect(textRect);                // set
                //  update animation component's elapsed time
                entity->getComponent<AnimationComponent>()->elapsedTime += deltaTime;
                //  update animation component animationIndex (is enough time has passed)
                if (entity->getComponent<AnimationComponent>()->elapsedTime >= entity->getComponent<AnimationComponent>()->frameTime) {
                    entity->getComponent<AnimationComponent>()->elapsedTime = 0.0f;

                    // -> below: ADD THIS FUNCTIONALITY
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
    }*/
};

#endif