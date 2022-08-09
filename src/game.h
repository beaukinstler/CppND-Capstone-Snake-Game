#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "computerSnake.h"
#include "messageQueue.h"

#include <thread>
#include <mutex>
#include <memory>
#include <assert.h>
#include <algorithm>
#include "gameDebug.h"

// const bool GAME_TESTING{true};


class Game {
 public:
  enum class FoodStatus { eaten, placed, gone } ;
  Game(std::size_t grid_width, std::size_t grid_height);
  ~Game();
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  void EatFood();
  void GameOver();

 private:
  std::mutex _mtx;
  Snake snake1;
  ComputerSnake computerSnake;
  std::vector<std::shared_ptr<ComputerSnake>> computerSnakes;
  SDL_Point food;
  void DropFood();

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void Update(Snake& snake);
  void UpdateComp(ComputerSnake& snake);
  MessageQueue<FoodStatus> gameMessages;
  void waitForFoodEaten();
  std::vector<std::thread> gameThreads;
  bool _gameOver{false};


};



#endif