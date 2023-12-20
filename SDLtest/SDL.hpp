#ifndef SDL_hpp
#define SDL_hpp

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>
using namespace std;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

// Start up SDL and creates window
bool init();
// Loads media
bool loadMedia();
// Frees media and shuts down SDL
void close();
// Loads individual image
SDL_Surface* loadSurface(string path);
SDL_Texture* loadTexture(string path);
#endif /* SDL_hpp */