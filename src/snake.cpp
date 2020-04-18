#include "snake.h"
#include <cmath>
#include <iostream>
#include <mutex>

Snake::Snake(const Snake &source)
{
    std::unique_lock<std::mutex> lck(_mutex);
    std::unique_lock<std::mutex> lck_src(source._mutex);
    speed = source.speed;
    grid_height = source.grid_height;
    grid_width = source.grid_width;
    size = source.size;
    alive = source.alive;
    head_x = source.head_x;
    head_y = source.head_y;
    body = source.body;
    growing = source.growing;
}


Snake::Snake(Snake &&source)
{
    std::unique_lock<std::mutex> lck(_mutex);
    std::unique_lock<std::mutex> lck_src(source._mutex);
    grid_height = source.grid_height;
    grid_width = source.grid_width;
    speed = source.speed;
    size = source.size;
    alive = source.alive;
    head_x = source.head_x;
    head_y = source.head_y;
    body = std::move(source.body);
    growing = source.growing;
}

Snake &Snake::operator=(const Snake &source)
{
  	if (this == &source)
        return *this;
    std::unique_lock<std::mutex> lck(_mutex);
    std::unique_lock<std::mutex> lck_src(source._mutex);
    grid_height = source.grid_height;
    grid_width = source.grid_width;
    speed = source.speed;
    size = source.size;
    alive = source.alive;
    head_x = source.head_x;
    head_y = source.head_y;
    body = source.body;
    growing = source.growing;
    return *this;
}


Snake &Snake::operator=(Snake &&source)
{ 
    if (this == &source) return *this;
    std::unique_lock<std::mutex> lck(_mutex);
    std::unique_lock<std::mutex> lck_src(source._mutex);
    grid_height = source.grid_height;
    grid_width = source.grid_width;
    speed = source.speed;
    size = source.size;
    alive = source.alive;
    head_x = source.head_x;
    head_y = source.head_y;
    body = std::move(source.body);
    growing = source.growing;
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