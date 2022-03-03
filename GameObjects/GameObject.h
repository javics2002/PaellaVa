#pragma once

#include "../Data/Posicion.h"
#include "Collider.h"

#include "../Utils/Texture.h"
#include "../Utils/Vector2D.h"
#include "../sdlutils/SDLUtils.h"

#include <string>

class Game;

using namespace std;

class GameObject : public Collider
{
    const int CENTER_TAM = 4;

protected:
    Vector2D<double> pos, vel;
    int w, h;

    Game* game;
    Texture* texture;

public:

    GameObject(Game* game) : game(game), texture(nullptr), w(0), h(0) {
    };
    GameObject(Game* game, string claveTextura) : game(game), texture(&sdlutils().images().at(claveTextura)), w(0), h(0) {
    };

    virtual ~GameObject() {};

    virtual void update() {
        pos = pos + vel;
    };

    virtual void render();
    virtual bool OnClick(int mx, int my) { return false; };

    void setPosition(double x, double y);
    void setPosition(Vector2D<double> v);
    void setDimension(int width, int height);
    void setTexture(string clave);

    int getWidth() { return w; };
    int getHeight() { return h; };

    double getX() { return pos.getX(); };
    double getY() { return pos.getY(); };
    Vector2D<double> getPosition() { return pos; }

    virtual SDL_Rect getCollider();
    virtual SDL_Rect getCenter();

    bool collide(SDL_Rect other);
};

