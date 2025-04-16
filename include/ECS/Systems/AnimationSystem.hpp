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
            // Get component pointers
            AnimationMapComponent* animapComp = cm.getComponent<AnimationMapComponent>(ent);
            AnimationComponent* animComp = cm.getComponent<AnimationComponent>(ent);
            StateComponent* stateComp = cm.getComponent<StateComponent>(ent);

            // Calculate
            textRect.position.x = textRect.size.x * animComp->animationIndex;
            textRect.position.y = textRect.size.y * animapComp->stateToAnimation[stateComp->currentState].rowIndex;

            // Update component(s)
            animComp->elapsedTime += deltaTime;

            if (animComp->elapsedTime >= animapComp->stateToAnimation[stateComp->currentState].frameTime) {
                animComp->elapsedTime = 0.0f;

                if (animComp->animationIndex >= animapComp->stateToAnimation[stateComp->currentState].frameCount)
                    animComp->animationIndex = 0;//<-- if reached end of animation, reset animationIndex
                else 
                    animComp->animationIndex++;
                
            }
        }
    }
};

#endif