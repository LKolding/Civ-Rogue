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
            // Check for Animation-, AnimationMap-, Sprite- and StateComponent
            if (
                !cm.getComponent<AnimationComponent>(ent) | 
                !cm.getComponent<SpriteComponent>(ent) | 
                !cm.getComponent<AnimationMapComponent>(ent) |
                !cm.getComponent<StateComponent>(ent)
            ) {
                return;
            }
            sf::IntRect& textRect = cm.getComponent<SpriteComponent>(ent)->textureRectangle;

            // Calculate
            textRect.position.x = textRect.size.x * cm.getComponent<AnimationComponent>(ent)->animationIndex;
            textRect.position.y = textRect.size.y * cm.getComponent<AnimationMapComponent>(ent)->stateToAnimation[cm.getComponent<StateComponent>(ent)->currentState].rowIndex;

            AnimationComponent* animComp = cm.getComponent<AnimationComponent>(ent);

            // Update components
            animComp->elapsedTime += deltaTime;

            if (animComp->elapsedTime >= cm.getComponent<AnimationMapComponent>(ent)->stateToAnimation[cm.getComponent<StateComponent>(ent)->currentState].frameTime) {
                animComp->elapsedTime = 0.0f;

                // TODO add below in the if clause
                // animationIndex >= (animationSheetWidth/textRect.width)-1) |
                if (animComp->animationIndex >= cm.getComponent<AnimationMapComponent>(ent)->stateToAnimation[cm.getComponent<StateComponent>(ent)->currentState].frameCount) {
                    animComp->animationIndex = 0;

                } else {
                    animComp->animationIndex++;
                }
            }
        }
    }
};

#endif