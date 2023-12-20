#include "SDL.hpp"
// Screen dimension constants
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
// Declare global window, surface, image
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Surface* gScreen = NULL;
TTF_Font* gFont = NULL;
// SDL_Texture* gTexture = NULL;

bool init() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }
    
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        cout << "Warning: Linear texture filtering not enabled!" << endl;
        return false;
    }
    
    // initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
        return false;
    }
                
    // initialize SDL_ttf
    if (TTF_Init() == -1) {
        cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
        return false;
    }
    // Create Window
    gWindow = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (gWindow == NULL) {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }
    
    // Create renderer for window
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == NULL) {
        cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
        return false;
    }
    
    // initialize renderer color
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    return true;
}

bool loadMedia() {
    // Load default
    
    return true;
}

void close() {
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;
    
    // Destroy window, will take care window surface
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    
    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

SDL_Surface* loadSurface(string path) {
    // Final image
    SDL_Surface* optimizedSurface = NULL;
    // Load temp image
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        cout << "Unable to load image " << path.c_str() << "! SDL Error: " << SDL_GetError() << endl;
    } else {
        // Convert image to screen format
        //optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
        //if (optimizedSurface == NULL) {
        //    cout << "Unable to optimize image " << path.c_str() << "! SDL Error: " << SDL_GetError() << endl;
        //}
        // Remove temp image
        SDL_FreeSurface(loadedSurface);
    }
    return optimizedSurface;
}

/*
SDL_Texture* loadTexture(string path) {
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        cout << "Unable to load image " << path.c_str() << "! SDL_image Error: " << IMG_GetError() << endl;
    } else {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            cout << "Unable to create texture from " << path.c_str() << "! SDL Error: %s\n" << SDL_GetError() << endl;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}
*/
