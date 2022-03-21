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
class NetworkManager;

using namespace std;


class Game {
    //Cambiar a scene actual
    Scene* currentScene;

    NetworkManager* nm;


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

    enum CanalesSonido { UI, JUGADOR, MUEBLES, CLIENTES };
};