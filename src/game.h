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



class Game {
 public:
  enum class FoodStatus { eaten, placed } ;
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  void EatFood();

 private:
  std::mutex _mtx;
  Snake snake1;
  ComputerSnake computerSnake;
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
};

#endif