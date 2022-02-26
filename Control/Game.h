#pragma once

#include "SDL.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "../GameObjects/Player.h"

#include "../Utils/Texture.h"

class Restaurante;
class ObjectManager;

using namespace std;

class Game{
    //Cambiar a scene actual
    Restaurante* restaurante;

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

