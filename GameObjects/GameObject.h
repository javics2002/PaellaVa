#pragma once

#include "../Data/Posicion.h"
#include "Collider.h"

#include "../View/Texture.h"
#include "../View/TextureContainer.h"
#include "../Utils/Vector2D.h"

#include <string>

class Game;

using namespace std;

class GameObject : public Collider
{
    const int CENTER_TAM = 4;

    Point2D<double> pos;
    int w, h;

protected:
    Game* game;
    Texture* texture;
    TextureName textureName;

    void drawTexture(Texture* texture);
public:

    GameObject(Game* game) : game(game), texture(nullptr), w(0), h(0) {
        textureName = berenjenaTexture;
    };

    virtual ~GameObject() {};

    virtual void draw();
    virtual void drawDebug();
    virtual void update() = 0;
    virtual bool OnClick(int mx, int my) { return false; };

    void setPosition(double x, double y);
    void setPosition(Vector2D<double> v);
    void setDimension(double width, double height);

    int getWidth() { return w; };
    int getHeight() { return h; };

    int getX() { return pos.getX(); };
    int getY() { return pos.getY(); };
    Vector2D<double> getPosition() { return pos; }

    virtual SDL_Rect getCollider();
    virtual SDL_Rect getCenter();

    bool collide(SDL_Rect other);
};

