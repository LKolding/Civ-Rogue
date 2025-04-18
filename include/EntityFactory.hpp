#ifndef _ENTITY_FACTORY_
#define _ENTITY_FACTORY_
// system
#include <memory>
// sfml
#include <SFML/Graphics.hpp>
// my stuff
#include "ResourceManager.hpp"

#include "ECS/Components/Components.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"

#include "constants.h"


inline EntityID buildNinja(ComponentManager& cm, EntityManager& em, float x = 0, float y = 0) {
    EntityID id = em.createEntity();

    const float XPOSITION = x;
    const float YPOSITION = y;
    const int MAX_HEALTH = 100;
    const int TEX_SIZE = 32;
    const std::string TEX_PATH = "../assets/textures/characters/ninja_sheet.png";
    
    cm.addComponent<PositionComponent>(id, {XPOSITION, YPOSITION});
    cm.addComponent<HealthComponent>(id, {MAX_HEALTH, MAX_HEALTH});
    cm.addComponent<SpriteComponent>(id, {{{0,0},{TEX_SIZE,TEX_SIZE}}, {TEX_SIZE/2, TEX_SIZE/2}, TEX_PATH});
    cm.addComponent<StateComponent>(id, {"IDLE"});

    cm.addComponent<AnimationComponent>(id, {0, 0, 5});
    // construct animation map component (hopefully temporary solution)
    AnimationMapComponent animMap;
    animMap.textureHeightPerRow = TEX_SIZE;
    animMap.stateToAnimation["IDLE"] = { 0, 5, 0.22f };
    animMap.stateToAnimation["WALK"] = { 1, 5, 0.06f };
    animMap.stateToAnimation["ATCK"] = { 2, 0 };
    
    cm.addComponent<AnimationMapComponent>(id, animMap); //apply constructed component
    cm.addComponent<FlipComponent>(id, {});

    // Hitbox gets declared here
    cm.addComponent<BoundsComponent>(id, {{{XPOSITION, YPOSITION},{(float)TEX_SIZE-12, (float)TEX_SIZE-12}}});
    cm.addComponent<CollisionComponent>(id, {});
    
    cm.addComponent<MovementComponent>(id, {120.0f, 1.0f, 0.0f, 0.0f});//<-- speed, speedMult, xdir, ydir
    cm.addComponent<HoverComponent>(id, {});
    cm.addComponent<SelectComponent>(id, {});
    cm.addComponent<ObjectiveComponent>(id, {});
    cm.addComponent<ControlComponent>(id, {});

    return id;
}


inline EntityID buildWell(ComponentManager& cm, EntityManager& em, float x = 0, float y = 0) {
    EntityID id = em.createEntity();

    const float XPOSITION = x;
    const float YPOSITION = y;
    const int MAX_HEALTH = 100;
    const int TEX_SIZE = 64;
    const std::string TEX_PATH = "../assets/textures/objects/well.png";
    
    cm.addComponent<PositionComponent>(id, {XPOSITION, YPOSITION});
    cm.addComponent<HealthComponent>(id, {MAX_HEALTH, MAX_HEALTH});
    cm.addComponent<SpriteComponent>(id, {{{0,0},{TEX_SIZE,TEX_SIZE}}, {TEX_SIZE/2, TEX_SIZE/2}, TEX_PATH});

    // Hitbox gets declared here
    cm.addComponent<BoundsComponent>(id, {{{XPOSITION, YPOSITION},{(float)TEX_SIZE-20, (float)TEX_SIZE-38}}});
    cm.addComponent<CollisionComponent>(id, {true}); //<-- static body = true

    cm.addComponent<HoverComponent>(id, {});
    cm.addComponent<SelectComponent>(id, {false});

    return id;

}


inline EntityID buildTree(ComponentManager& cm, EntityManager& em, float x = 0, float y = 0) {
    EntityID id = em.createEntity();

    const float XPOSITION = x;
    const float YPOSITION = y;
    const int MAX_HEALTH = 100;
    const int TEX_WIDTH  = 128 + 32;
    const int TEX_HEIGHT = 128 + 32;
    const std::string TEX_PATH = "../assets/textures/tilesheets/TX Plant.png";
    
    cm.addComponent<PositionComponent>(id, {XPOSITION, YPOSITION});
    cm.addComponent<HealthComponent>(id, {MAX_HEALTH, MAX_HEALTH});
    cm.addComponent<SpriteComponent>(id, {{{0,0},{TEX_WIDTH,TEX_HEIGHT}}, {TEX_WIDTH/2, TEX_HEIGHT/2}, TEX_PATH});

    // Hitbox gets declared here
    cm.addComponent<BoundsComponent>(id, {{{XPOSITION, YPOSITION},{(float)TEX_WIDTH/12, (float)TEX_HEIGHT/4}}});
    cm.addComponent<CollisionComponent>(id, {true}); //<-- static body = true

    cm.addComponent<HoverComponent>(id, {});
    cm.addComponent<SelectComponent>(id, {false});

    return id;

} 

#endif