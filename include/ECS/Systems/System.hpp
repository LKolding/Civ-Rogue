#ifndef _SYSTEM_
#define _SYSTEM_

#include "ECS/Entities/Entity.hpp"

#include <vector>
#include <memory>

class System {
public:
    virtual void update(float deltaTime, std::vector<std::weak_ptr<Entity>> entities) = 0;
    inline virtual ~System() { };
};

#endif