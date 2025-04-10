#ifndef _STATE_SYSTEM_
#define _STATE_SYSTEM_

    // SYSTEM RESPONSIBLE FOR:
    // managing the StateComponents of all entities
    // and make sure to switch and transition between
    // states when applicable. 
    // This is important for the animatation system.

#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"
#include "ECS/Components/Components.hpp"


class StateSystem {
public:
    inline void update() {
        
    }
};

#endif