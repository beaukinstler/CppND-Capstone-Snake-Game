#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {
          for(int i = 0; i < size; i++){
            SDL_Point p{head_x, (head_y + (i * 1.0) + 1.0)};
            body.push_back(p);
          }
        }
  Snake(int grid_width, int grid_height, int offset, int player_num)
      : grid_width(grid_width),
        grid_height(grid_height),
        player_num(player_num),
        head_x(grid_width / 2  - offset ),
        head_y(grid_height / 2  - offset ) {
          for(int i = 0; i < size; i++){
            SDL_Point p{head_x, (head_y + (i * 1.0) + 1.0)};
            body.push_back(p);
          }
        }

  void Update();

  void GrowBody();
  bool SnakeCell(int x, int y);
  int GetPlayerNum() const;
  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{30};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;
  int player_num{0}; // 0 = computer
};

#endif