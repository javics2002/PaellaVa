#pragma once

#include "SDL.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "ObjectManager.h"

#include "../View/TextureContainer.h"
#include "../View/Texture.h"

using namespace std;

class Game{
    string name;
    int width, height;
    bool doExit;

    SDL_Renderer* renderer = nullptr;
    //Font* font;

    bool debug = false;

    ObjectManager* objectManager;
    TextureContainer* textureContainer;

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
    SDL_Renderer* getRenderer();
    //void renderText(string text, int x, int y, SDL_Color color = { 0,0,0 });

    Texture* getTexture(TextureName name);

    pair<TextureName, int> getRandomIngridient();
    ObjectManager* getObjectManager();

};

