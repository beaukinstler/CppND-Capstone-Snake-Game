#ifndef COMPSNAKE_H
#define COMPSNAKE_H

#include <vector>
#include "SDL.h"
#include <random>
#include <thread>
#include <mutex>
#include <iostream>
#include "snake.h"
#include "gameDebug.h"

class ComputerSnake : public Snake {
 public:
    // enum class Direction { kUp, kDown, kLeft, kRight };
    // ComputerSnake();
    ComputerSnake(const ComputerSnake&){}; 
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
    ~ComputerSnake(){
      // clean up threads with a thread barrier. 
      _snakesGameOver = true;
      for(auto &t : threads)
      {

        t.join();
      }
      GameDebug::gameDebugMsg("killed the snake\n");
    }

    void UpdateLastKnownFoodPoint(const SDL_Point &food);

    // release the snake
    void Release();


      
  private:
    void Hunt();
    std::vector<std::thread> threads; 
    bool _snakesGameOver = false;

    
    // will go same direction this durration before changing directions
    // also impacted by the variablity of the randomness
    std::chrono::duration<int,std::centi> snakeMaxAgility{15}; // lower number == less sleep time on the thread
    int snakeVariablityRange{5}; // == random nubers from 1 to snakeVariablityRange will be used to alter the movements of the snake. lower numbers more consistant and faster moves

    int player_num{0}; // 0 = computer
    SDL_Point _lastKnownFoodPoint{1,1};
    std::mutex _mtx;

    
};

#endif