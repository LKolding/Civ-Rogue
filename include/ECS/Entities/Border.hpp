#include "ECS/Entities/Entity.hpp"



class BorderEntity: public Entity {
public:
    BorderEntity() {
        this->addComponent(std::make_shared<PositionComponent>());
        this->addComponent(std::make_shared<SpriteComponent>());
        this->addComponent(std::make_shared<AnimationComponent>());
        this->addComponent(std::make_shared<UUIDComponent>());

    }
    
    void update(float dt) {

    }
};