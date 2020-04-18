#include "snake.h"
#include <cmath>
#include <iostream>
#include <mutex>

Snake::Snake(const Snake &src) {
    std::unique_lock<std::mutex> lck(_mutex);
    std::unique_lock<std::mutex> lck_src(src._mutex);
    speed = src.speed;
    grid_height = src.grid_height;
    grid_width = src.grid_width;
    size = src.size;
    alive = src.alive;
    head_x = src.head_x;
    head_y = src.head_y;
    body = src.body;
    growing = src.growing;
}


Snake::Snake(Snake &&src) {
    std::unique_lock<std::mutex> lck(_mutex);
    std::unique_lock<std::mutex> lck_src(src._mutex);
    grid_height = src.grid_height;
    grid_width = src.grid_width;
    speed = src.speed;
    size = src.size;
    alive = src.alive;
    head_x = src.head_x;
    head_y = src.head_y;
    body = std::move(src.body);
    growing = src.growing;
}

Snake &Snake::operator=(const Snake &src) {
  	if (this == &src)
        return *this;
    std::unique_lock<std::mutex> lck(_mutex);
    std::unique_lock<std::mutex> lck_src(src._mutex);
    grid_height = src.grid_height;
    grid_width = src.grid_width;
    speed = src.speed;
    size = src.size;
    alive = src.alive;
    head_x = src.head_x;
    head_y = src.head_y;
    body = src.body;
    growing = src.growing;
    return *this;
}


Snake &Snake::operator=(Snake &&src) { 
    if (this == &src) return *this;
    std::unique_lock<std::mutex> lck(_mutex);
    std::unique_lock<std::mutex> lck_src(src._mutex);
    grid_height = src.grid_height;
    grid_width = src.grid_width;
    speed = src.speed;
    size = src.size;
    alive = src.alive;
    head_x = src.head_x;
    head_y = src.head_y;
    body = std::move(src.body);
    growing = src.growing;
    return *this;
}

void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::ChangeDirection(Snake::Direction new_dir) {
  Snake::Direction opposite;
  switch(new_dir) {
    case Snake::Direction::kDown: 
      opposite = Snake::Direction::kUp; 
      break;
    case Snake::Direction::kUp: 
      opposite = Snake::Direction::kDown;
      break;
    case Snake::Direction::kLeft: 
      opposite = Snake::Direction::kRight; 
      break;
    case Snake::Direction::kRight: 
      opposite = Snake::Direction::kLeft;
      break;
  }
  std::unique_lock<std::mutex> lck(_mutex);
  if (direction != opposite || size == 1) 
    direction = new_dir;
  return;
}

void Snake::UpdateHead() {
  std::unique_lock<std::mutex> lck(_mutex);
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  std::unique_lock<std::mutex> lck(_mutex);
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}

void Snake::GrowBody() { 
  std::unique_lock<std::mutex> lck(_mutex);
  growing = true; 
}

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  std::unique_lock<std::mutex> lck(_mutex);
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}