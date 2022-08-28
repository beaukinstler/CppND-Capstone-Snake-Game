#include "game.h"
#include <iostream>
#include "SDL.h"
#include "messageQueue.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake1(grid_width, grid_height, 1),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      computerSnake(grid_width, grid_height, 1, 0) {
  // PlaceFood();
}

Game::~Game(){
    std::for_each(gameThreads.begin(), gameThreads.end(), [](std::thread &t) {
        t.join();
    });

}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;


  gameThreads.emplace_back(std::thread(&Game::DropFood, this));
  computerSnake.Release();

  while (running && !_gameOver) {
    frame_start = SDL_GetTicks();
    // if(computerSnakes.size() == 0) {
    //   computerSnake.Release();
    //   computerSnakes.emplace_back(std::make_shared<ComputerSnake>(this->computerSnake)) ;
    // };

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake1);
    Update(snake1);

    if(snake1.IsWinner()){
      std::cout << "You WON!\n";
      break;
    }

    UpdateComp(computerSnake);

    if (_gameOver) {
      std::cout << "You have been bested!\n";
      // break;
    }

    // renderer.Render(computerSnake, food);
    // renderer.Render(snake, food);
    renderer.Render(snake1, computerSnake, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }



    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if(frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }

}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.

    if (!this->snake1.SnakeCell(x, y) && !this->computerSnake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update(Snake &snake) {

  // lock
  std::lock_guard<std::mutex> lock(_mtx);

  // exit if snake 1 is not alive
  if (!snake.alive || !(this->snake1.alive))
  {
    GameOver();
    return;
  }
  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    if(snake.GetPlayerNum() != 0){
      score++;
    }
    // PlaceFood();
    EatFood();
    // Shrink snake and increase speed.
    snake.ShrinkBody();
    snake.speed += 0.02;

    if(snake.IsWinner()){
      GameOver();
      GameDebug::gameDebugMsg("Player is winner. Sent FoodStatus::gone message to gameMessages.");
    }

  }
}

void Game::UpdateComp(ComputerSnake &snake) {

  // lock
  std::lock_guard<std::mutex> lock(_mtx);

  // exit if snake 1 is not alive
  if (!snake.alive || !(this->snake1.alive)) return;

  snake.Update();


  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score--;
    // PlaceFood();
    EatFood();
    // Shrink snake and increase speed.
    snake.ShrinkBody();
    snake.speed += 0.02;

    if(snake.IsWinner()){
      GameOver();
      GameDebug::gameDebugMsg("Computer snake is winner. Sent FoodStatus::gone message to gameMessages.");
    }



  }
  else{
    snake.UpdateLastKnownFoodPoint(Game::food);
  }

}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake1.size; }


void Game::EatFood(){
  // std::lock_guard<std::mutex> lck(_mtx);
  Game::gameMessages.send(std::move(FoodStatus::eaten));
}

void Game::DropFood(){
  GameDebug::gameDebugMsg("DropFood started \n");
  while(true && !Game::_gameOver){
    Game::PlaceFood();
    GameDebug::gameDebugMsg("Food Placed \n");

    Game::waitForFoodEaten();
    GameDebug::gameDebugMsg("Debug: DropFood just saw waitForFoodEaten");
  }



}

void Game::waitForFoodEaten()
{
    // an infinite while-loop runs and repeatedly calls the receive function on the message queue.
    // Once it receives FoodStatus::Eaten, the method returns.

    while (true && !_gameOver)
    {
        auto msg = this->gameMessages.receive();
        if (Game::FoodStatus::eaten == msg)
        {
            GameDebug::gameDebugMsg("Food was eaten, so about to return from waitForFoodEaten");
            return;
        }
        else if (Game::FoodStatus::gone == msg)
        {
          GameDebug::gameDebugMsg("Someone won, so this should be the last time we call waitForFoodEaten");
          return;
        }
    }
}


void Game::GameOver(){

  this->gameMessages.send(Game::FoodStatus::gone);
  _gameOver = true;
  std::cout << "Game over!\n--------\n";
}