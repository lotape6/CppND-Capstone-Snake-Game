#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <future>
#include <mutex>

#include <random>

#define MIN_SPAWN_SECS 1
#define MAX_SPAWN_SECS 3

#define ITEM_EFFECT_DURATION_SECONDS 3

enum SpecialItemType : unsigned {
  directionChange = 0,
  speedUp,
  solidBorders,
  slowDown,
  noTail
};

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

class Game {
public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

private:
  Snake snake;
  SDL_Point food;
  SDL_Point specialItem;

  std::mutex mtx;
  std::future<SpecialItemType> futureItem;
  std::future<void> undoFuture;

  bool undoItemEffect;

  bool itemSpawned;
  SpecialItemType itemType;

  int delay;

  bool solidBorders;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  std::uniform_int_distribution<int> specialItemsSpawn;
  std::uniform_int_distribution<int> specialItemsSpawnDelay;

  int score{0};

  void PlaceFood();
  SpecialItemType PlaceSpecialItem();
  void Update(Controller &controller);

  void ApplyItem(Controller &controller);
};

#endif