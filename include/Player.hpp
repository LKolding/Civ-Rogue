#ifndef _PLAYER_
#define _PLAYER_

#include <SFML/Graphics.hpp>

#include "ECS/Entities/Entity.hpp"
#include "ECS/Entities/Ninja.hpp"

#include "input.hpp"


struct Player {
    static inline sf::View playerView;
    static inline sf::Vector2f m_mouse_pos;

    void update(float deltaTime, std::unique_ptr<InputManager>& inputManager, sf::Vector2f mouse_pos);
    // x and y simply indicate direction, and should be of
    // value 1.0, 0.0 or -1.0
    void move(float x, float y, float dt);

    void setPosition(float x = 0, float y = 0);
    sf::Vector2f getPosition();

    void selectUnit(std::shared_ptr<Entity> &entity);

    void deselectUnit(std::shared_ptr<Entity> &entity);

    void followUnit(std::shared_ptr<Entity>& entity);
    void stopFollow();
    bool isFollowingUnit();

private:
    static inline sf::Vector2f pos;
 
    static inline std::weak_ptr<Entity> entityp;
    const static inline float moveSpeed = 400.0f; // View panning speed
    void updateView();

};
#endif