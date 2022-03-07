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
class Scene;
class UIManager;

using namespace std;

//senum escenasJuego { MENU, RESTAURANTE };

class Game {
    //Cambiar a scene actual
    Scene* currentScene;

public:
    Game();
    ~Game();

    void init();
    void start();
    void changeScene(Scene* scene);

    void handleInput(SDL_Event& event, bool& exit);
    void update();
    void render();

    ObjectManager* getObjectManager();
    UIManager* getUIManager();
};