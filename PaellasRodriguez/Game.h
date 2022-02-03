#pragma once

#include "SDL.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

class Game{
    string name;
    bool doExit;
    int width, height;

    //TextureContainer* textureContainer;
    SDL_Renderer* renderer = nullptr;
    //Font* font;

    bool debug = false;

public:
    Game(int width, int height);
    ~Game();

    string getGameName();

    void startGame();
    void update();
    void draw();

    void setUserExit();
    bool isUserExit();

    int getWindowWidth();
    int getWindowHeight();
    //Font* getFont() { return font; };

    void setRenderer(SDL_Renderer* renderer);
    void loadTextures();
    //Texture* getTexture(TextureName name);
    SDL_Renderer* getRenderer();
    void renderText(string text, int x, int y, SDL_Color color = { 0,0,0 });
};

