# CPPND: Selected Capstone -> Snake Game

This is a fork repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.


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


## File structure

The File structure is the same as the propose example repository. Nevertheless, all the files (unless the main.cpp file) have been modified.

## Added functionality

The modification proposed for this project was to add the aparition of random special items. These items could be either good (green) or bad (red) items.

The items are randomly spawned as circles over the map. There are 5 tipes of special items implemented. Their type is defined in the "game.h". There the SpecialItemType enum is defined with this types:

  * DirectionChange (red): temporarily inverts the controls (up is down, left is right and viceversa).
  * Speed Up (red): temporarily increases the velocity of the snake in a factor of 1.75. 
  * Solid Borders (red): temporarily sets the boundaries of the map to solid (rendering red walls). If the snake hits this walls will die.
  * Slow Down (green): temporarily decreases the velocity of the snake in a factor of 1.75.
  * No Tail (green): temporarily deactivates the snake body, thus the snake couldn't hit itself and the body is not rendered.

All this item will be appearing randomly in the map and once the snake hits it with its head, the effect is applied for ITEM_EFFECT_DURATION_SECONDS (3 seconds so far).

For this functionality all the files over src have been changed.

 * *controller*.* -> Added the capacity of inverting the inputs

 * *game*.* -> Added the special item logic of spawning, applying effects and undo effects.
 * *render*.* -> have been modified to render the spawned items, to hide/show the snake body and to render red lines in the border when respective item is eaten.
 * *snake*.* added the posibilty of having no body and not to wrap the map boundaries.


## Rubric achieved

* The project demonstrates an understanding of C++ functions and control structures.
   * src/game.cpp:111, 164

* The project uses Object Oriented Programming techniques. 
  
  * src/snake.h:40, 41
  * src/renderer.cpp:118
  * src/game.cpp:111, 164
  * src/game.h: from 39 to 52 and 65 and 68  
  * src/controller.h:9, 10, 15


* Class constructors utilize member initialization lists.
  * src/game.cpp:9

* The project makes use of references in function declarations.

  * src/game.h:66, 68

* The project uses multithreading.
  * src/game.cpp:29, 52, 164 onwards

* A promise and future is used in the project.
  * src/game.cpp:29, 52

* A mutex or lock is used in the project.
  * src/game.cpp: 42, 77, 94, 112, 156, 164 onwards lambdas' bodies.
## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
