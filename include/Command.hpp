#ifndef _COMMAND_
#define _COMMAND_

#include <string>
#include <iostream>

#include "ECS/Entities/Entity.hpp"
#include "ECS/Components/Components.hpp"

//  Interface
class Command {
public:
  ~Command() = default;
  virtual void execute(Entity& receiver) const = 0;

};

//  ------------------------------
//        Concrete commands
//  ------------------------------

class MoveCommand: public Command {
private:
  sf::Vector2f direction;

public: 
  MoveCommand(const sf::Vector2f& dir): direction(dir) {}

  //  virtual function implementation
  void execute(Entity& receiver) const override {
    if (receiver.hasComponent<PositionComponent>() && receiver.hasComponent<MovementComponent>()) {
      return;
    }
  }
};

#endif