#pragma once

#include "Game.h"

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

using namespace std;

const int FRAME_RATE = 60;

class ViewController
{
    Game* game;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    void initSDL();

public:
    ViewController(Game* game);
    ~ViewController();

    void run();
    void handleEvents();
    void clearBackground();

    unsigned int frameDuration();
};

