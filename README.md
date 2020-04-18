# CPPND: Capstone Snake Game Example

<img src="snake_game.gif"/>

A simple snake game modified on top of existing codebase with bonus modes.

## The project uses Object Oriented Programming techniques.

Objects are created using OOP.

## The project demonstrates an understanding of C++ functions and control structures.

Functions and loops are used for control structures throughout the project.

## Classes use appropriate access specifiers for class members.

Variables and methods for every class are explicitly specified as private or public.

## A mutex or lock is used in the project.

Mutexes are used in the Snake class to prevent data races.

## The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.

Usage of std::unique_lock so that the mutex will be unlocked when it goes out of scope.

## The project follows the Rule of 5.

Snake class possesses copy constructor, copy assignment operator, move constructor, move assignment operator, and destructor.


## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  * Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source.
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.
