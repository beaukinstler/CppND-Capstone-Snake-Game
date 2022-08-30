#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"
#include <mutex>
#include <iostream>
#include "gameDebug.h"

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };
  Snake(){};
  Snake(int grid_width, int grid_height, int playerNum)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2),
        player_num(playerNum) // 0 = computer {
        {
          for(int i = 0; i < size; i++){
            SDL_Point p{head_x, (head_y + (i * 1.0) + 1.0)};
            body.push_back(p);
          }
        }
  ~Snake()
  {
    std::cout << "Snake at " << this << "is passing on...\n";
    for( auto seg : body){
      seg.x = NULL;
      seg.y = NULL;
    }

  }

  void Update();

  void ShrinkBody();
  bool SnakeCell(int x, int y);
  int GetPlayerNum() const;
  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{10};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

  void SpillGuts() const;
  bool IsWinner() const;

 protected:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool shrinking{false};
  int grid_width;
  int grid_height;
  int player_num;
  bool _won{false};
private:
  std::mutex _mtx;

};

#endif