#ifndef _RENDER_SYSTEM_
#define _RENDER_SYSTEM_

#include <memory>
#include <typeindex>

#include <SFML/Graphics.hpp>

#include "ECS/Components/Components.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"

#include "TextDraw.hpp"

#define DRAW_BOUNDS true

class RenderSystem {
public:
    inline void update(std::unique_ptr<sf::RenderWindow>& renderWindow, std::shared_ptr<ResourceAllocator>& allocator, EntityManager& em, ComponentManager& cm) {
        for (auto& ent : em.getAllEntities()) {
            // --------------------------------
            // ----- Check for components -----
            // --------------------------------
            if ( cm.getComponent<PositionComponent>(ent) && cm.getComponent<SpriteComponent>(ent) ) {
                // --------------------------------------
                // ----- Update bounds if necessary -----
                // --------------------------------------
                if ( auto pbounds = cm.getComponent<BoundsComponent>(ent)) {
                    // Get position
                    sf::Vector2f pos {cm.getComponent<PositionComponent>(ent)->x - pbounds->bounds.size.x/2,  // x //  
                                      cm.getComponent<PositionComponent>(ent)->y - pbounds->bounds.size.y/2}; // y
                    
                    // Get size
                    // sf::Vector2f size{(float)pbounds->bounds.size.x, (float)pbounds->bounds.size.y};
                    // Update bounds
                    pbounds->bounds.position = pos;
                    // pbounds->bounds.size = size;
                }

                // Pointers to components
                PositionComponent* cposition = cm.getComponent<PositionComponent>(ent);
                SpriteComponent* csprite = cm.getComponent<SpriteComponent>(ent);
                
                // Sprite to be drawn
                sf::Sprite sprite(*allocator->loadTexture(csprite->texturePath));
                sprite.setOrigin(csprite->origin); // Necessary ?
                sprite.setTextureRect(csprite->textureRectangle);
                sprite.setPosition({cposition->x, cposition->y});

                // FLIP BEHAVIOR
                if ( cm.getComponent<FlipComponent>(ent)) {
                    FlipComponent* flip = cm.getComponent<FlipComponent>(ent);
                    // apply scale transformation regardless of change in value
                    sprite.setScale({flip->isFlipped ? -1.f : 1.f, 1.f}); 
                }
                
                // Draw sprite
                if (csprite->isVisible) {
                    renderWindow->draw(sprite);
                }
                // Draw hitbox
                if (DRAW_BOUNDS) {
                    // create drawable shape from BoundsComponent
                    sf::RectangleShape hitbox {{(float)cm.getComponent<BoundsComponent>(ent)->bounds.size.x, (float)cm.getComponent<BoundsComponent>(ent)->bounds.size.y}};
                    // hitbox.setPosition({cposition->x, cposition->y});
                    hitbox.setPosition({cm.getComponent<BoundsComponent>(ent)->bounds.position.x, cm.getComponent<BoundsComponent>(ent)->bounds.position.y});
                    // appearance
                    hitbox.setFillColor(sf::Color::Transparent);
                    hitbox.setOutlineColor(cm.getComponent<HoverComponent>(ent)->isHovered ? sf::Color::Green : sf::Color::Black);

                    if (cm.getComponent<FollowComponent>(ent) && cm.getComponent<FollowComponent>(ent)->isFollowing)
                        hitbox.setOutlineColor(sf::Color::Blue); // <---- Set hitbox color to blue if being followed

                    hitbox.setOutlineThickness(0.8f);
                    renderWindow->draw(hitbox);
                }
            }
        }
    }
};

#endif