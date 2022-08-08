#include "snake.h"
#include <cmath>
#include <iostream>

void Snake::Update() {

  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  std::lock_guard(Snake::_mtx);
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
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!shrinking) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    shrinking = false;
    size--;
    // body.pop_back(); // this leaves a gap can be a fun way to play.
    body.erase(body.begin());
    body.erase(body.begin());
  }

  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      SpillGuts();
      alive = false;
    }
  }
}

void Snake::GrowBody() { shrinking = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
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

int Snake::GetPlayerNum() const
{
  return this->player_num;
}


void Snake::SpillGuts() const{
  std::cout << "SNAKE GUTS\n";
  std::cout << "~~~~~~~~~~~\n";
  std::cout << static_cast<std::underlying_type<Direction>::type>(this->direction) << "\n";
  std::cout << "~~~~~~~~~~~\n";
  std::cout << "~~~~~~~~~~~\n";
}