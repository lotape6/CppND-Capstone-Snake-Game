#include "SDL.h"
#include "controller.h"
#include "snake.h"
#include <iostream>

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1)
    snake.direction = input;
  return;
}

void Controller::HandleInput(bool &running, Snake &snake) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {

      switch (e.key.keysym.sym) {
      case SDLK_UP:
        ChangeDirection(
            snake, inverted ? Snake::Direction::kDown : Snake::Direction::kUp,
            inverted ? Snake::Direction::kUp : Snake::Direction::kDown);
        break;

      case SDLK_DOWN:
        ChangeDirection(
            snake, inverted ? Snake::Direction::kUp : Snake::Direction::kDown,
            inverted ? Snake::Direction::kDown : Snake::Direction::kUp);
        break;

      case SDLK_LEFT:
        ChangeDirection(
            snake,
            inverted ? Snake::Direction::kRight : Snake::Direction::kLeft,
            inverted ? Snake::Direction::kLeft : Snake::Direction::kRight);
        break;

      case SDLK_RIGHT:
        ChangeDirection(
            snake,
            inverted ? Snake::Direction::kLeft : Snake::Direction::kRight,
            inverted ? Snake::Direction::kRight : Snake::Direction::kLeft);
        break;
      }
    }
  }
}