#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width), screen_height(screen_height),
      grid_width(grid_width), grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(Snake const snake, SDL_Point const &food,
                      const bool borders, SDL_Point const &specialItem,
                      const bool isGood) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render snake's body
  if (!snake.HideTail()) {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for (SDL_Point const &point : snake.body) {
      block.x = point.x * block.w;
      block.y = point.y * block.h;
      SDL_RenderFillRect(sdl_renderer, &block);
    }
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render Item
  if (specialItem.x != -1 && specialItem.y != -1) {
    DrawCircle(specialItem.x * block.w + block.w / 2,
               specialItem.y * block.w + block.w / 2, block.w / 2, isGood);
  }

  // Render borders
  if (borders) {
    SDL_SetRenderDrawColor(sdl_renderer, 0xF0, 0x00, 0x00, 0x10);
    SDL_Rect vertical, horizontal;
    horizontal.w = 5;
    horizontal.h = screen_height;

    vertical.w = screen_width;
    vertical.h = 5;

    // Up
    horizontal.x = 0;
    horizontal.y = 0;
    SDL_RenderFillRect(sdl_renderer, &horizontal);

    // Left
    vertical.x = 0;
    vertical.y = 0;
    SDL_RenderFillRect(sdl_renderer, &vertical);

    // Down
    horizontal.x = screen_height - 5;
    horizontal.y = 0;
    SDL_RenderFillRect(sdl_renderer, &horizontal);

    // Right
    vertical.x = 0;
    vertical.y = screen_width - 5;
    SDL_RenderFillRect(sdl_renderer, &vertical);
  }
  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

// Method found in:
// https://iq.opengenus.org/getting-started-2d-graphics-in-cpp-sdl2/
void Renderer::DrawCircle(int center_x, int center_y, int radius_,
                          bool isGreen) {

  const int red = isGreen ? 0 : 255;
  const int green = isGreen ? 255 : 0;

  SDL_SetRenderDrawColor(sdl_renderer, red, green, 0, 255);

  // Drawing circle
  for (int x = center_x - radius_; x <= center_x + radius_; x++) {
    for (int y = center_y - radius_; y <= center_y + radius_; y++) {
      if ((std::pow(center_y - y, 2) + std::pow(center_x - x, 2)) <=
          std::pow(radius_, 2)) {
        SDL_RenderDrawPoint(sdl_renderer, x, y);
      }
    }
  }
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) +
                    " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
