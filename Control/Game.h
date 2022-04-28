#pragma once

#include "SDL.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <queue>
#include "../Utils/Vector2D.h"

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

    string nombre = "elbaginon";

    Vector2D<double> slideSon = {0,0};
    Vector2D<double> slideMus = {0,0};

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

    void setNombre(string nombre_);
    string getNombre();

    void setSlidesSon(Vector2D<double> slideSon_);
    void setSlidesMus(Vector2D<double> slideMus_);
    Vector2D<double> getSlideSon();
    Vector2D<double> getSlideMus();
    
};