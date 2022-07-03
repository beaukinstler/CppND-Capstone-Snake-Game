#ifndef COMPSNAKE_H
#define COMPSNAKE_H

#include <vector>
#include "SDL.h"
#include <random>
#include <thread>
#include <mutex>

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

      void UpdateLastKnownFoodPoint(const SDL_Point &food){
        _lastKnownFoodPoint.x = food.x;
        _lastKnownFoodPoint.y = food.y;
      }

      // release the snake
      void Release(){
        // start the hunting thread
        threads.emplace_back(std::thread(&ComputerSnake::Hunt, this));
      }

      void Hunt(){

        // resusing code from my previous poject to generate random number for delay
        std::random_device rdev;
        std::default_random_engine randomTime(rdev());
        std::uniform_int_distribution<int> distrib(1, 3);
        int randomSeconds = distrib(randomTime);
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        std::chrono::time_point<std::chrono::system_clock> stopTime = now + std::chrono::duration<int>(randomSeconds);
        while(true){
          if (now > stopTime){
            std::lock_guard<std::mutex> lock(_mtx);
            // get location of food
            // UpdateLastKnownFoodPoint(TODO GET FOOD FROM GAME);
            // for now food will always be 1,1
            
            // make most reasonable move toward the food
            int xdiff = head_x - this->_lastKnownFoodPoint.x;
            int ydiff = head_y - this->_lastKnownFoodPoint.y;
            bool yIsOnPoint = ydiff == 0;
            bool xIsOnPoint = ydiff == 0;
            if(yIsOnPoint){
              if(xdiff > 0){
                this->direction = Direction::kRight;
              }
              else{
                this->direction = Direction::kLeft;
              }
            }
            else if(xIsOnPoint){
              if(ydiff > 0){
                this->direction = Direction::kDown;
              }
              else{
                this->direction = Direction::kUp;
              }
            }

            

            // delay for amount of time, set as snake quickness.
          }
          else{
            now = std::chrono::system_clock::now();
          }
          // sleep at every iteration to reduce CPU usage

          std::this_thread::sleep_for(std::chrono::milliseconds(this->snakeSlowness));
        }
      }
      
  private:
    std::vector<std::thread> threads; 

    int snakeSlowness{500}; // will god same direction(sleep) for millisecond before changing directions
    int player_num{0}; // 0 = computer
    SDL_Point _lastKnownFoodPoint{1,1};
    std::mutex _mtx;

    
};

#endif