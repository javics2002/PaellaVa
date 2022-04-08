#pragma once

#include "SDL.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

class Restaurante;
class ObjectManager;
class Scene;
class UIManager;
class NetworkManager;

using namespace std;

class Game {
    //Cambiar a scene actual
    Scene* currentScene = nullptr;
    NetworkManager* nm = nullptr;

public:
    Game();
    ~Game();

    void init(); 
    void start();
    void changeScene(Scene* scene);
    Scene* getCurrentScene() { return currentScene; }

    void handleInput(SDL_Event& event, bool& exit);
    void update();
    void refresh();
    void render();

    ObjectManager* getObjectManager();
    UIManager* getUIManager();
    NetworkManager* getNetworkManager();

    enum CanalesSonido { UI, JUGADOR, MUEBLES, CLIENTES };
};