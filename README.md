# CPPND: Capstone Snake Game 



This is an extended version of the Snake Game, a project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). 

As the original README states, "The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses."


## How I chose this project

Truthfully, I didn't intend to use this, but as I had experimented immediately after first seeing it, I had some ideas about changes I thought would be fun to try. Namely, adding another snake to compete against. 

## There's another snake...

I assumed I'd be able to find a way to use multiple threads in order to run a computer player, and starting poking that idea. Originally, I had started with the intent that there could perhaps be more than just one, but in the end, I stayed with just the one competitor. I just wanted to see if I could get a snake running and finding food autonimously. 

I also wondered how changing the goal would impact the way I needed to code it. Instead of growing, the snakes, get smaller (and faster). 

The game has a few ways to end, so I also strugged with how to send the right messages to the queue, and when, to ensure that threads were closed down as the program needed to end. I went down a few git branches trying various ideas, and backtracked a number of times, before make a version that is a bit simpler than some of my attempts, but I feel would still statify much of the projects goals.

## Running the programs

The original dependancies are still present. See [the dependency section below.](#dependencies-for-running-locally)

1. Clone this repo.
1. `cd` into the folder you cloned (it should have this README.md file).
1. `cd` into the build folder.
1. Use the command `"cmake .."` followed by `"make"` to compile the src folder into an executable.
1. There should now be an executable called `SnakeGame`.
1. Run the file from your terminal with the command `./SnakeGame`


## Code structure
Much of the oringal code structure remains, with the addition of class to extend the Snake class, to make a ComputerSnake. 

All source files are in the `{project root}/src` directory. 
Most classes have a distinct headder file(.h) and code file (.cpp). There's a simple `gameDebug.h` file that doesn't hava a `.cpp` file to go along. 

The `main.cpp` file contains the main function, and includes the `game.h` header, `render.h` and, and `controller.h`. 
Main initials the Game::Run function. 

The `game.h` file included most other headers for the rest of the classes.

## Satisfying the rubric

- [x] The project compiles and runs, not only on my local system, but also in the dev environment provided for the project. It compiles utilizing cmake and make, as suggested.

- [x] The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks.

### Loops and IO
  - [x] The project accepts input from a user as part of the necessary operation of the program.
    - Input is required to move the snake.

- [x] A variety of control structures are used in the project.
- [x] The project code is clearly organized into functions.
    - While I'm sure it could be inmproved, examples of this are found in the game.cpp. I separated the `Update` functions, breaking out smaller functions, and functiones needed for the computer snake, vs the player. See also the Game::GameOver() function, and Game::PlaceFood() function, as some examples.


### Object Orientated Programming

- [x] The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks.
- The Snake class holds a '_won' boolean value, with a public IsWinner() function to retrieve it. 

- [x] All class data members are explicitly specified as public, protected, or private.
  - The "Snake" class shows some example of this. protected was used to enable  ineheritance where needed. 

- [ ] All class members that are set to argument values are initialized through member initialization lists.
  - There are few left TODO here. 

- [x] Appropriate data and functions are grouped into classes. Member data that is subject to an invariant is hidden from the user. State is accessed via member functions.

- [x] One function is overloaded with different signatures for the same function name.
  - Renderer::Render() has two signatures.

- [x] Inheritance hierarchies are logical. Composition is used instead of inheritance when appropriate. Abstract classes are composed of pure virtual functions. Override functions are specified.
  - The ComputerSnake needs to use most of the features of the SnakeClass, and augment or change. 
    - It adds new functions, Release() and UpdateLastKnownFoodPoint().
    - However, it also overrides the ShrinkBody() function to effect the computer snake with unique changes.

- [x] One function is declared with a template that allows it to accept a generic parameter.
  - See the MessageQueue. Its utilized by the Game class to send and receive `gameMessages` of type FoodStatus. 




### Memory Managment

- [x] At least two variables are defined as references, or two functions use pass-by-reference in the project code.
  - ComputerSnake::UpdateLastKnownFoodPoint passes a reference to a `food` object.
  - Game::UpdateComp passes a reference to the Snake object.



### Concurancy

- [x] The project uses multiple threads in the execution.
  - See ComputerSnake::Release(), and Game::Run(), line 32. 

- [x] A mutex or lock (e.g. std::lock_guard or `std::unique_lock) is used to protect data that is shared across multiple threads in the project code.
  - See ComputerSnake::Hunt(), Game::Update().

- [x] A std::condition_variable is used in the project code to synchronize thread execution.
  - See the use of the MessageQueue. 
  - Also these functions
    Game::EatFood()
    Game::waitForFoodEaten()
    Game::GameOver()



## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.


## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg


This code was adapted and changed by Beau Kinstler for fun (and hopefully a nanodegree.)