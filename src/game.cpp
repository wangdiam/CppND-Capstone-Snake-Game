#include "game.h"
#include <iostream>
#include <algorithm>
#include <future>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height), foodcount(0),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height)),
      random_bonus(0,2) {
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update(renderer);
    renderer.Render(snake, foods);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
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
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    std::cout<<"Food count: " << foodcount << std::endl;
    if (!snake.SnakeCell(x, y)) {
      SDL_Point food = {x,y};
      foodcount++;
      foods.emplace_back(food);
      return;
    }
  }
}

void Game::Update(Renderer &renderer) {
  if (!snake.alive) return;
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  snake.Update();

  int bonus = random_bonus(engine);
  if (foodcount == 0) {
    if (bonus == 0) {
      PlaceFood();
      PlaceFood();
      PlaceFood();
    } else {
      PlaceFood();
    }
  }
  for (int i=0;i<foods.size();i++) {
    SDL_Point food = foods.at(i);
    int new_x = static_cast<int>(snake.head_x);
    int new_y = static_cast<int>(snake.head_y);
    if (food.x == new_x && food.y == new_y) {
      score++;
      std::cout<<"Food count: " << foodcount << std::endl;
      foodcount--;
      foods.erase(foods.begin() + i);
      snake.GrowBody();
      snake.speed += 0.02;
      break;
    }
  }
  // Check if there's food over here
    // Grow snake and increase speed.
}

int Game::GetScore() const { 
  return score; 
}
int Game::GetSize() const { 
  return snake.size; 
}