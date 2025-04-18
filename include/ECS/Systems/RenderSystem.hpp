#ifndef _RENDER_SYSTEM_
#define _RENDER_SYSTEM_

#include <memory>
#include <typeindex>

#include <SFML/Graphics.hpp>

#include "ECS/Components/Components.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"

#include "TextDraw.hpp"


struct DrawableSprite {
    sf::Sprite sprite;
    sf::RectangleShape hitbox;
    float y; // Usually position.y + sprite.getGlobalBounds().height
};

// ------------------
// ----- System -----
// ------------------

class RenderSystem {
private:
    std::vector<DrawableSprite> spritesToBeDrawn;

public:
    inline void update(sf::RenderWindow& renderWindow, std::shared_ptr<ResourceManager>& allocator, EntityManager& em, ComponentManager& cm) {
        for (auto& ent : em.getAllEntities()) {
            // --------------------------------
            // ----- Check for components -----
            // --------------------------------
            if ( cm.getComponent<PositionComponent>(ent) && cm.getComponent<SpriteComponent>(ent) ) {
                // --------------------------------------
                // ----- Update bounds if necessary -----
                // --------------------------------------
                if (cm.getComponent<BoundsComponent>(ent)) {
                    BoundsComponent* pbounds = cm.getComponent<BoundsComponent>(ent);
                    // Get position
                    if (PositionComponent* pposition = cm.getComponent<PositionComponent>(ent)) {
                        sf::Vector2f pos {pposition->x - pbounds->bounds.size.x/2,  // x 
                                          pposition->y - pbounds->bounds.size.y/2}; // y
                        pbounds->bounds.position = pos; // apply
                    }
                }

                // Pointers to components
                PositionComponent* cposition = cm.getComponent<PositionComponent>(ent);
                SpriteComponent* csprite = cm.getComponent<SpriteComponent>(ent);

                if (!csprite->isVisible)
                    continue;//<-- skip entirely if sprite isn't visible

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

                // create drawable shape from BoundsComponent
                sf::RectangleShape hitbox {{(float)cm.getComponent<BoundsComponent>(ent)->bounds.size.x, (float)cm.getComponent<BoundsComponent>(ent)->bounds.size.y}};
                hitbox.setPosition({cm.getComponent<BoundsComponent>(ent)->bounds.position.x, cm.getComponent<BoundsComponent>(ent)->bounds.position.y});
                // appearance
                hitbox.setFillColor(sf::Color::Transparent);

                if (cm.getComponent<HoverComponent>(ent)) {
                    hitbox.setOutlineColor(cm.getComponent<HoverComponent>(ent)->isHovered ? sf::Color::Green : sf::Color::Black);//<- green if hovered
                    hitbox.setOutlineThickness(cm.getComponent<HoverComponent>(ent)->isHovered ? 1.5f : 1.0f);
                }
                else {
                    hitbox.setOutlineColor(sf::Color::Black);
                    hitbox.setOutlineThickness(1.0f);
                }
                // Set color of hitbox to blue if selected
                if (cm.getComponent<SelectComponent>(ent) && cm.getComponent<SelectComponent>(ent)->isSelected) {
                    hitbox.setOutlineColor(sf::Color::Blue);
                }
                // Store sprite so it can be drawn after processing all sprites
                this->spritesToBeDrawn.push_back( {sprite, hitbox, cposition->y} );
            }
        } // for loop

        // Sort all DrawableSprites by Y coordinate (emulate Z index)
        std::sort(spritesToBeDrawn.begin(), spritesToBeDrawn.end(), [](const DrawableSprite& a, const DrawableSprite& b) {
            return a.y < b.y;
        });

        for (const auto& [sprite, hb, y] : this->spritesToBeDrawn) {
            // Draw each sprite
            renderWindow.draw(sprite); 
            // Draw hitbox if applicable
            renderWindow.draw(hb);
        }

        // reset system-specific sprite container
        this->spritesToBeDrawn.clear();
        this->spritesToBeDrawn.shrink_to_fit();

    }

};

#endif