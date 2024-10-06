#ifndef _HEALTH_BAR_
#define _HEALTH_BAR_

#include "ECS/Entities/Entity.hpp"
#include "ECS/Components/Components.hpp"

class HealthbarEntity : public Entity {
public:

    HealthbarEntity(std::shared_ptr<Entity>& entity) {
        this->entity_ptr = std::weak_ptr<Entity>(entity); // set owner
        this->addComponent(std::make_shared<PositionComponent>());
        this->addComponent(std::make_shared<SpriteComponent>());
        this->addComponent(std::make_shared<DeletableComponent>());
        this->addComponent(std::make_shared<UUIDComponent>());

    }

    
    void update(float dt) {
        if (!entity_ptr.lock()) {
            this->getComponent<DeletableComponent>()->markedForDeletion = true;
            return;
        }
        if (auto ent_ptr = entity_ptr.lock()) {
            if (auto spr_ptr = this->getComponent<SpriteComponent>()) {
                if (auto hlt_ptr = ent_ptr->getComponent<HealthComponent>()) {
                    float health_percentage =  (float)hlt_ptr->currentHealth / (float)hlt_ptr->maxHealth;
                    int x_tile_amount = (spr_ptr->sprite.getTexture()->getSize().x / spr_ptr->sprite.getTextureRect().width) -1;// -1 skips the first healthbar container
                    int healthbar_index = ceil((1.0f - health_percentage) * (x_tile_amount)); 
                    int x_offset = (healthbar_index * spr_ptr->sprite.getTextureRect().width) + (spr_ptr->sprite.getTextureRect().width); // add extra offset to account for healthbar container

                    sf::IntRect textRect;
                    textRect = spr_ptr->sprite.getTextureRect();
                    textRect.left = x_offset;

                    spr_ptr->sprite.setTextureRect(textRect);

                    this->getComponent<PositionComponent>()->x = ent_ptr->getComponent<PositionComponent>()->x - (textRect.width/2);
                    this->getComponent<PositionComponent>()->y = ent_ptr->getComponent<PositionComponent>()->y - (textRect.height*2);

                }
            }
        }

    }

private:
    std::weak_ptr<Entity> entity_ptr;
};

#endif