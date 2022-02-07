#pragma once

#include "Game.h"

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

