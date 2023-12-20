#ifndef _2048_hpp
#define _2048_hpp

#include "SDL.hpp"

bool createTileTexture();
void removeTexture();
bool isFull(int* ary);
bool gameOver(int* ary);
void addRandom(int* ary, int n);
bool merge(int& a, int& b, int& c, int& d);
bool update(int* ary, int key);
void runGame();

#endif /* _2048_hpp */
