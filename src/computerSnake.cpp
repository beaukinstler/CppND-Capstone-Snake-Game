#include "computerSnake.h"


void ComputerSnake::UpdateLastKnownFoodPoint(const SDL_Point &food){
    _lastKnownFoodPoint.x = food.x;
    _lastKnownFoodPoint.y = food.y;
}


    // release the snake
void ComputerSnake::Release(){
    // start the hunting thread
    threads.emplace_back(std::thread(&ComputerSnake::Hunt, this));
        
    GameDebug::gameDebugMsg("realease the snake thread");
}


void ComputerSnake::Hunt(){

    // resusing code from my previous poject to generate random number for delay
    std::random_device rdev;
    std::default_random_engine randomTime(rdev());
    std::uniform_int_distribution<int> distrib(1, snakeVariablityRange); // 10 std::deci  == 1 second (std::ratio<1,10> == std::deci)
    int randomSeconds = 30; // just to start. Over 10 will be a longer than usual delay. 100 ~ 10 seconds
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::chrono::time_point<std::chrono::system_clock> stopTime = now + std::chrono::duration<int, std::deci>(randomSeconds);
    while(!_snakesGameOver && this->alive == true){
      now = std::chrono::system_clock::now();
      if (now > stopTime){
        std::lock_guard<std::mutex> lock(_mtx);
        // get location of food
        // UpdateLastKnownFoodPoint(TODO GET FOOD FROM GAME);
        // for now food will always be 1,1
        
        // make most reasonable move toward the food
        int xdiff = head_x - this->_lastKnownFoodPoint.x;
        int ydiff = head_y - this->_lastKnownFoodPoint.y;

        // bool yIsOnPoint = std::abs(ydiff * 10.0) <= 8 && std::abs(ydiff * 10.0) > 2;
        bool yIsOnPoint = int(head_y) == this->_lastKnownFoodPoint.y;
        // bool xIsOnPoint = std::abs(xdiff * 10.0) <= 8 && std::abs(xdiff * 10.0) > 2;
        bool xIsOnPoint = int(head_x) == this->_lastKnownFoodPoint.x;

        GameDebug::gameDebugMsg("Current x diff: " + std::to_string(xdiff));
        GameDebug::gameDebugMsg("Current  y diff: " + std::to_string(ydiff) ); 
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

  void ComputerSnake::ShrinkBody() { 
    shrinking = true; 
    if(body.size() < 2){
      this->_won = true;
    }
    else {
        shrinking = false;
        size--;
        // body.pop_back(); // this leaves a gap can be a fun way to play.
        body.erase(body.begin());
        // body.erase(body.begin());

        // increase rate of change allowed 
        if(this->snakeVariablityRange > 1){
          this->snakeVariablityRange -= 2;
        }
    }
  }

