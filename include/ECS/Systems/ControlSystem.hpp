#ifndef _CONTROL_SYSTEM_
#define _CONTROL_SYSTEM_

    // SYSTEM RESPONSIBLE FOR:
    // "controlling" entities that have the ControlComponent eg. makes sure
    // keystrokes are converted into MovementComponent of the controlled entity

#include "input.hpp"
#include "Player.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"
#include "ECS/Components/Components.hpp"


class ControlSystem {
public:
    inline void update(EntityManager& em, ComponentManager& cm, InputManager& im, Player& player) {
        if (!player.isFollowingUnit())
            return;

        // Get position of entity
        const PositionComponent* entityPosition = cm.getComponent<PositionComponent>(player.entityFollow.value());
        // Set position of view to entity
        player.playerView.setCenter({entityPosition->x, entityPosition->y});
        // Get movement component pointer
        auto pentityMove = cm.getComponent<MovementComponent>(player.entityFollow.value());
        pentityMove->xDir = 0.0f;
        pentityMove->yDir = 0.0f;

        // Keyboard
        if (im.keyState[sf::Keyboard::Key::LShift] && pentityMove->speedMult < 1.5f) 
            pentityMove->speedMult += 0.1;
        else
            pentityMove->speedMult = 1.0f;

        if (im.keyState[sf::Keyboard::Key::W]) pentityMove->yDir -= 1.f;
        if (im.keyState[sf::Keyboard::Key::S]) pentityMove->yDir += 1.f;
        if (im.keyState[sf::Keyboard::Key::A]) pentityMove->xDir -= 1.f;
        if (im.keyState[sf::Keyboard::Key::D]) pentityMove->xDir += 1.f;

        //if (im.keyState[sf::Keyboard::Key::Escape]) player.stopFollow();

       

    }
};


#endif