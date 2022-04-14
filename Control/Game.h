#pragma once

#include "SDL.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <queue>

class Restaurante;
class ObjectManager;
class Scene;
class UIManager;
class NetworkManager;

using namespace std;

struct MessageChangeScene
{
    Scene* newScene;
};

class Game {
    //Cambiar a scene actual
    Scene* currentScene = nullptr;
    NetworkManager* nm = nullptr;

    queue<MessageChangeScene> qScene;

public:
    Game();
    ~Game();

    void init(); 
    void start();

    Scene* getCurrentScene() { return currentScene; }

    void handleInput(SDL_Event& event, bool& exit);
    void update();
    void refresh();
    void render();
    
    ObjectManager* getObjectManager();
    UIManager* getUIManager();
    NetworkManager* getNetworkManager();

    enum CanalesSonido { UI, JUGADOR, MUEBLES, CLIENTES };

    void sendMessageScene(Scene* newScene);
    void recvMessageScene();
};