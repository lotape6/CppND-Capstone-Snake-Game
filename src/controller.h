#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"

class Controller {
public:
  void HandleInput(bool &running, Snake &snake) const;
  void InvertControls() { inverted = true; }
  void NormalControls() { inverted = false; }

private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
  bool inverted = false;
};

#endif