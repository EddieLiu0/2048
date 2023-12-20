#ifndef SDL_hpp
#define SDL_hpp

#include <string>
#include <math.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
using namespace std;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

// Start up SDL and creates window
bool init();
// Frees media and shuts down SDL
void close();
#endif /* SDL_hpp */
