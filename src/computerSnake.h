#ifndef COMPSNAKE_H
#define COMPSNAKE_H

#include <vector>
#include "SDL.h"
#include <random>
#include <thread>
#include <mutex>
#include <iostream>

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
    // clean up thread swith thread barrier. 
    _gameOver = true;
    for(auto &t : threads)
    {

      t.join();
    }
    std::cout << "killed the snake\n";
  }

  void UpdateLastKnownFoodPoint(const SDL_Point &food){
    _lastKnownFoodPoint.x = food.x;
    _lastKnownFoodPoint.y = food.y;
  }

  // release the snake
  void Release(){
    // start the hunting thread
    threads.emplace_back(std::thread(&ComputerSnake::Hunt, this));
        
    std::cout << "realease the snake thread\n";
  }

  void Hunt(){

    // resusing code from my previous poject to generate random number for delay
    std::random_device rdev;
    std::default_random_engine randomTime(rdev());
    std::uniform_int_distribution<int> distrib(1, snakeVariablityRange); // 10 std::deci  == 1 second (std::ratio<1,10> == std::deci)
    int randomSeconds = 30; // just to start. Over 10 will be a longer than usual delay. 100 ~ 10 seconds
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::chrono::time_point<std::chrono::system_clock> stopTime = now + std::chrono::duration<int, std::deci>(randomSeconds);
    while(!_gameOver){
      now = std::chrono::system_clock::now();
      if (now > stopTime){
        std::lock_guard<std::mutex> lock(_mtx);
        // get location of food
        // UpdateLastKnownFoodPoint(TODO GET FOOD FROM GAME);
        // for now food will always be 1,1
        
        // make most reasonable move toward the food
        int xdiff = head_x - this->_lastKnownFoodPoint.x;
        int ydiff = head_y - this->_lastKnownFoodPoint.y;
        bool yIsOnPoint = ydiff == 0;
        bool xIsOnPoint = xdiff == 0;
        std::cout << "Current x an dy diff: " << xdiff << " " << ydiff << "\n"; 
        if(yIsOnPoint){
          if(xdiff > 0 && this->direction != Direction::kRight){

            this->direction = Direction::kLeft;
          }
          else if( xdiff < 0 && this->direction != Direction::kLeft){
            this->direction = Direction::kRight;
          }
        }
        else if(xIsOnPoint){
          if(ydiff < 0){
            if(this->direction != Direction::kUp ){
              this->direction = Direction::kDown;
            }
          }
          else if(ydiff > 0){
            if(this->direction != Direction::kDown){
              this->direction = Direction::kUp;
            }
          }
        }
        else{
          if(this->direction == Direction::kDown || this->direction == Direction::kUp){
            this->direction = Direction::kLeft;
          }
          else if(this->direction == Direction::kLeft || this->direction == Direction::kRight){
            this->direction = Direction::kDown;
          }
        }
        std::cout << "Current Direction: " <<   "\n"; 


        

        // delay for amount of time, set as snake quickness.
        // reset the clock
        randomSeconds = distrib(randomTime);
        stopTime = now + std::chrono::duration<int, std::deci>(randomSeconds);
      }
      else{
        //now = std::chrono::system_clock::now();
      }
      // sleep at every iteration to reduce CPU usage

      std::this_thread::sleep_for(snakeMaxAgility);

    }
  }
      
  private:
    std::vector<std::thread> threads; 
    bool _gameOver = false;

    
    // will go same direction this durration before changing directions
    // also impacted by the variablity of the randomness
    std::chrono::duration<int,std::deci> snakeMaxAgility{2}; 
    int snakeVariablityRange{7};

    int player_num{0}; // 0 = computer
    SDL_Point _lastKnownFoodPoint{1,1};
    std::mutex _mtx;

    
};

#endif