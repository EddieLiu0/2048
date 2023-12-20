#include "2048.hpp"
using namespace std;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern SDL_Renderer* gRenderer;

enum KeyPressSurfaces
{
    KEY_DEFAULT,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT
};

SDL_Texture* gTexture[16] {};

bool createTileTexture() {
    // Load font
    TTF_Font* font = TTF_OpenFont("new.ttf", 100);
    SDL_Color textColor = {0, 0, 0};
    SDL_Color backColor;
    SDL_Surface* surfaceMessage = NULL;
    
    if (font == NULL) {
        cout << "Unable to load font!" << endl;
        return false;
    } else {
        for (int i = 1; i < 17; ++i) {
            // set background color for different tile
            backColor = {(uint8_t)(rand() % 256),(uint8_t)(rand() % 256),(uint8_t)(rand() % 256)};
            string s = to_string((int)pow(2, i));
            // pad the string to make the text on tile smaller
            if (s.length() < 4) {
                s.insert(0, "  ");
                s.append("  ");
            } else {
                s.insert(0, " ");
                s.append(" ");
            }
            surfaceMessage = TTF_RenderUTF8_Shaded(font, s.c_str(), textColor, backColor);
            gTexture[i - 1] = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);
        }
        SDL_FreeSurface(surfaceMessage);
    }
    return true;
}

void removeTexture() {
    for (int i = 0; i < 16; ++i) {
        SDL_DestroyTexture(gTexture[i]);
        gTexture[i] = NULL;
    }
}

bool isFull(int* ary) {
    for (int i = 0; i < 16; ++i) {
        if (!ary[i]) {
            return false;
        }
    }
    return true;
}

bool gameOver(int* ary) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (ary[j + 4 * i] == ary[1 + j + 4 * i] || ary[i + j * 4] == ary[4 + i + j * 4]) {
                return false;
            }
        }
    }
    return true;
}
// add n random tile
void addRandom(int* ary, int n) {
    int randPos{rand() % 16};
    int randNum{};
    for (int i = 0; i < n; ++i) {
        while (ary[randPos]) {   // while not empty
            randPos = rand() % 16;
        }
        randNum = rand() % 2 + 1;
        ary[randPos] = randNum; // add random number
        if (isFull(ary)) {
            i = n;
        }
    }
}

bool merge(int& a, int& b, int& c, int& d) {
    bool updated = false;
    
    // remove empty block before merge
    if (!c || !b || !a){       // empty space in first three block
        if (!c && d) {         // c is empty and d is not
            c = d;
            d = 0;
            updated = true;
        }
        if (!b && (c || d)) { // b is empty, c or d is not
            b = c;
            c = d;
            d = 0;
            updated = true;
        }
        if (!a && (b || c || d)) { // a is empty, b or c or d is not
            a = b;
            b = c;
            c = d;
            d = 0;
            updated = true;
        }
    }
    // start merge
    if (a == b && a) {
        ++a;
        if (c == d && c) {
            b = ++c;
            c = 0;
        } else {
            b = c;
            c = d;
        }
        d = 0;
        updated = true;
    } else if (b == c && b) {
        ++b;
        c = d;
        d = 0;
        updated = true;
    } else {
        if (c == d && c) {
            ++c;
            d = 0;
            updated = true;
        }
    }
    return updated;
}

bool update(int* ary, int key) {
    bool updated = false;
    switch(key) {
        case KEY_UP:
            for (int i = 0; i < 4; ++i){
                // go to next column by adding i 
                if (merge(ary[0 + i], ary[4 + i], ary[8 + i], ary[12 + i])) {
                    updated = true;
                }
            }
            break;
        case KEY_DOWN:
            for (int i = 0; i < 4; ++i){
                if (merge(ary[12 + i], ary[8 + i], ary[4 + i], ary[0 + i])) {
                    updated = true;
                }
            }
            break;
        case KEY_LEFT:
            for (int i = 0; i < 4; ++i){
                // go to next row by adding 4 
                if (merge(ary[0 + 4 * i], ary[1 + 4 * i], ary[2 + 4 * i], ary[3 + 4 * i])) {
                    updated = true;
                }
            }
            break;
        case KEY_RIGHT:
            for (int i = 0; i < 4; ++i){
                if (merge(ary[3 + 4 * i], ary[2 + 4 * i], ary[1 + 4 * i], ary[0 + 4 * i])) {
                    updated = true;
                }
            }
            break;
        default:
            break;
    }
    return updated;
}

void runGame() {
    int game[16] {};
    bool quit = false;
    bool gameover = false;
    SDL_Event e;
    addRandom(game, 2);
    if (!init()) {
        cout << "Failed to initialize!" << endl;
    } else {
        if (!createTileTexture()) {
            cout << "Failed to create texture!" << endl;
        } else {
            SDL_Rect ary[16];
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    // set area of each tile
                    ary[i * 4 + j].x = 0 + j * SCREEN_WIDTH / 4;
                    ary[i * 4 + j].y = 0 + i * SCREEN_HEIGHT / 4;
                    ary[i * 4 + j].w = SCREEN_WIDTH / 4;
                    ary[i * 4 + j].h = SCREEN_HEIGHT / 4;
                }
            }
            while (!quit) {
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    } else if (e.type == SDL_KEYDOWN && !gameover) {
                        if (isFull(game) && gameOver(game)) {
                            gameover = true;
                            cout << "gameover" << endl;
                        } else {
                            switch ((e.key.keysym.sym)) {
                                case SDLK_UP:
                                    dir = KEY_UP;
                                    break;
                                case SDLK_DOWN:
                                    dir = KEY_DOWN;
                                    break;
                                case SDLK_LEFT:
                                    dir = KEY_LEFT;
                                    break;
                                case SDLK_RIGHT:
                                    dir = KEY_RIGHT;
                                    break;
                                default:
                                    dir = KEY_DEFAULT;
                                    break;
                            }
                            if (update(game, dir)) {
                                addRandom(game, 1);
                            }
                        }
                    }
                }
                if (!gameover) {
                    // clear screen
                    SDL_RenderClear(gRenderer);
                
                    // update screen
                    for (int i = 0; i < 16; ++i) {
                        SDL_RenderCopy(gRenderer, gTexture[game[i] - 1], NULL, &ary[i]);
                    }
                    SDL_RenderPresent(gRenderer);
                }
            }
        }
    }
    removeTexture();
    close();
}

/* old version
bool merge(int& a, int& b, int& c, int& d) {
    int tmp;
    bool updated = false;
    if (a) {
        if (b) {
            if (a == b) {
                ++a;
                b = 0;
                updated = true;
                if (c && d) {      // a, c, d
                    if (c == d) {  // c = d
                        b = ++c; // merge and move to b
                        c = 0;
                        updated = true;
                    } else {
                        b = c;     // move forward
                        c = d;
                        updated = true;
                    }
                } else {
                    if (c || d) {        // a and another one
                        b = (c) ? c : d; // move forward
                        c = 0;
                        updated = true;
                    }
                }
                d = 0;
            } else {
                if (c && d) {       // a, b, c, d
                    if (b == c) {
                        ++b;
                        c = d;
                        d = 0;
                        updated = true;
                    } else {
                        if (c == d) {
                            ++c;
                            d = 0;
                            updated = true;
                        }
                    }
                } else {
                    if (c || d) {
                        tmp = (c) ? c : d;
                        if (b == tmp) {
                            ++b;
                            c = 0;
                            updated = true;
                        } else {
                            if (c != tmp) {
                                c = tmp;
                                updated = true;
                            }
                        }
                        d = 0;
                    }
                }
            }
        } else if (c && d) {        // a, c, d
            if (a == c) {           // a = c
                ++a;             // merge
                b = d;              // move d to b
                c = 0;
                updated = true;
            } else if (c == d) {    // c = d
                b = ++c;          // merge and move to b
                c = 0;
                updated = true;
            } else {
                b = c;              // move forward
                c = d;
                updated = true;
            }
            d = 0;
        } else {
            if (c || d) {            // a and another one
                tmp = (c) ? c : d;
                if (a == tmp) {      // if equal
                    ++a;        // merge
                    updated = true;
                } else {
                    b = tmp;         // else move forward
                    updated = true;
                }
                c = 0;
                d = 0;
            }
        }
    } else if (b) {
        if (c && d) {            // b, c, d
            if (b == c) {        // b equal c
                a = ++b;       // merge and move to a
                b = d;           // move the remaining to b
                c = 0;
                updated = true;
            } else if (c == d) { // c equal d
                a = b;           // move b to a
                b = ++c;       // merge and move to b
                c = 0;
                updated = true;
            } else {
                a = b;
                b = c;
                c = d;
                updated = true;
            }
            d = 0;
        } else if (c || d) {      // b and another one
            tmp = (c) ? c : d;
            if (b == tmp) {       // equal
                a = ++b;      // merge and move to a
                b = 0;
                updated = true;
            } else {              // not equal
                a = b;            // move b to a
                b = tmp;          // move tmp to b
                updated = true;
            }
            c = 0;
            d = 0;
        } else {      // only b
            a = b;    // move b to a
            b = 0;
            updated = true;
        }
    } else if (c) {
        if (d) {           // c and d
            if (c == d) {  // c = d
                a = ++c; // merge and move to a
                updated = true;
            } else {
                a = c;     // move c to a
                b = d;     // move d to b
                updated = true;
            }
            d = 0;
        } else {    // only c
            a = c;  // move c to a
            updated = true;
        }
        c = 0;
    } else if (d) { // only d
        a = d;      // move d to a
        d = 0;
        updated = true;

    }
    return updated;
}
*/
