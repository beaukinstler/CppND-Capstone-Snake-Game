#ifndef COMPSNAKE_H
#define COMPSNAKE_H

#include <vector>
#include "SDL.h"

class ComputerSnake : public Snake {
 public:
  // enum class Direction { kUp, kDown, kLeft, kRight };
  // ComputerSnake();
  ComputerSnake(int grid_width, int grid_height, int offset, int player_num)
      {
        this->grid_width = grid_width;
        this->grid_height = grid_height;
        this->player_num =  player_num;
        head_x = grid_width / 2  - offset;
        head_y = grid_height / 2  - offset;
        for(int i = 0; i < size; i++){
            SDL_Point p{head_x, (head_y + (i * 1.0) + 1.0)};
            body.push_back(p);
        }
      }
      
  private:

    int player_num{0}; // 0 = computer
};

#endif