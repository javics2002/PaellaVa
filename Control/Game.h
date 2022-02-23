#pragma once

#include "SDL.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "ObjectManager.h"
#include "../GameObjects/Player.h"

#include "../Utils/Texture.h"

using namespace std;

class Game{
    ObjectManager* objectManager;

public:
    Game();
    ~Game();

    void init();
    void start();

    void handleInput(SDL_Event& event, bool &exit);
    void update();
    void render();

    ObjectManager* getObjectManager();
};

