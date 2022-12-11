#include "SDL.h"
#include "game.h"
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
using namespace std::chrono_literals;

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height), undoItemEffect(false), itemSpawned(false),
      solidBorders(false), engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      specialItemsSpawn(static_cast<int>(SpecialItemType::directionChange),
                        static_cast<int>(SpecialItemType::noTail)),
      specialItemsSpawnDelay(MIN_SPAWN_SECS, MAX_SPAWN_SECS) {
  PlaceFood();
}

void Game::Run(Controller &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  delay = specialItemsSpawnDelay(engine);
  futureItem = std::async(std::launch::async, [this]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * delay));
    return PlaceSpecialItem();
  });

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update(controller);

    { // Critical Section
      std::unique_lock<std::mutex>(mtx);

      if (undoItemEffect &&
          undoFuture.wait_for(1ms) == std::future_status::ready) {
        undoFuture.get();
        undoItemEffect = false;
      }
      if (!itemSpawned &&
          futureItem.wait_for(1ms) == std::future_status::ready) {
        itemType = futureItem.get();
        futureItem = std::async(std::launch::async, [this]() {
          std::this_thread::sleep_for(std::chrono::milliseconds(1000 * delay));
          return PlaceSpecialItem();
        });
      }

      if (itemSpawned) {
        renderer.Render(
            snake, food, solidBorders, specialItem,
            static_cast<unsigned>(itemType) >
                static_cast<unsigned>(SpecialItemType::solidBorders));
      } else {
        renderer.Render(snake, food, solidBorders);
      }
    }

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      std::unique_lock<std::mutex>(mtx);
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  std::unique_lock<std::mutex>(mtx);

  while (true) {

    x = random_w(engine);
    y = random_h(engine);

    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

SpecialItemType Game::PlaceSpecialItem() {
  std::unique_lock<std::mutex>(mtx);
  int x, y;

  // update next delay time (seconds)
  delay = specialItemsSpawnDelay(engine);

  while (true) {

    x = random_w(engine);
    y = random_h(engine);

    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      specialItem.x = x;
      specialItem.y = y;

      itemSpawned = true;
      return static_cast<SpecialItemType>(
          rand() % (static_cast<int>(SpecialItemType::noTail) + 1));
    }
  }
}

void Game::Update(Controller &controller) {
  if (!snake.alive)
    return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }

  // Check if item was reached
  if (itemSpawned && specialItem.x == new_x && specialItem.y == new_y) {
    std::unique_lock<std::mutex>(mtx);

    ApplyItem(controller);

    itemSpawned = false;
  }
}

void Game::ApplyItem(Controller &controller) {

  switch (itemType) {
  case SpecialItemType::directionChange:
    controller.InvertControls();

    // launch delayed action to undo effect
    undoFuture = std::async(std::launch::async, [this, &controller]() {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(1000 * ITEM_EFFECT_DURATION_SECONDS));
      {
        std::unique_lock<std::mutex>(mtx);
        controller.NormalControls();
      }
      return;
    });
    undoItemEffect = true;
    break;
  case SpecialItemType::speedUp:
    snake.speed = 1.75 * snake.speed;
    // launch delayed action to undo effect
    undoFuture = std::async(std::launch::async, [this]() {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(1000 * ITEM_EFFECT_DURATION_SECONDS));
      {
        std::unique_lock<std::mutex>(mtx);
        snake.speed = snake.speed / 1.75;
      }
      return;
    });
    undoItemEffect = true;
    break;
  case SpecialItemType::solidBorders:
    snake.WrapBorders(false);
    solidBorders = true;
    // launch delayed action to undo effect
    undoFuture = std::async(std::launch::async, [this]() {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(1000 * ITEM_EFFECT_DURATION_SECONDS));
      {
        std::unique_lock<std::mutex>(mtx);
        snake.WrapBorders(true);
      }
      solidBorders = false;
      undoItemEffect = true;
    });
    break;
  case SpecialItemType::slowDown:
    snake.speed = 0.75 * snake.speed;

    // launch delayed action to undo effect
    undoFuture = std::async(std::launch::async, [this]() {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(1000 * ITEM_EFFECT_DURATION_SECONDS));
      {
        std::unique_lock<std::mutex>(mtx);
        snake.speed = snake.speed / 0.75;
      }
      return;
    });
    undoItemEffect = true;
    break;
  case SpecialItemType::noTail:
    snake.HideTail(true);
    // launch delayed action to undo effect
    undoFuture = std::async(std::launch::async, [this]() {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(1000 * ITEM_EFFECT_DURATION_SECONDS));
      {
        std::unique_lock<std::mutex>(mtx);
        snake.HideTail(false);
      }
      return;
    });
    undoItemEffect = true;
    break;
  default:
    break;
  };
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }
