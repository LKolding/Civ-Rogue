#ifndef _PLAYER_
#define _PLAYER_

#include <SFML/Graphics.hpp>

#include "ECS/Entities/Entity.hpp"


//  Basically only holds the playerView
struct Player {
    static inline sf::View playerView;

    // x and y simply indicate direction, and should be of
    // value 1.0, 0.0 or -1.0
    void move(float x, float y, float dt);

    void setPosition(float x, float y);

    void selectUnit(std::shared_ptr<Entity> &entity);

    void deselectUnit(std::shared_ptr<Entity> &entity);

    void addObjectiveToSelectedUnits(sf::Vector2i pos);

private:
    static inline sf::Vector2f pos;
    static inline std::unordered_map<uint32_t, std::weak_ptr<Entity>> selectedEntities;
    const static inline float moveSpeed = 300.0f; // View panning speed
    void updateView();

};
#endif