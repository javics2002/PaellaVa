#pragma once

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
    bool active = true;

    void drawRender(SDL_Rect* cameraRect);   
    void drawRender(SDL_Rect* cameraRect, SDL_Rect rect, Texture* tex);
    void drawRender(SDL_Rect rect, Texture* tex);
    void drawDebug(SDL_Rect* cameraRect);
    void drawDebug(SDL_Rect* cameraRect, SDL_Rect rect);

    bool hasCollision(SDL_Rect rect1, SDL_Rect rect2);

public:

    GameObject(Game* game) 
        : game(game), texture(nullptr), w(0), h(0) {};
    GameObject(Game* game, string claveTextura) 
        : game(game), texture(&sdlutils().images().at(claveTextura)), w(0), h(0) {};

    virtual ~GameObject() {};

    virtual void update() {
        pos = pos + vel;
    };

    virtual void init() {};

    virtual void render(SDL_Rect* cameraRect);
    virtual void renderDebug(SDL_Rect* cameraRect);

    virtual bool OnClick(int mx, int my) { return false; };

    void setPosition(double x, double y);
    void setPosition(Vector2D<double> v);
    void setDimension(int width, int height);
    void setTexture(string clave);
    void setTexture(const string text, const string font, const SDL_Color& fgColor, const SDL_Color& bgColor);

    int getWidth() { return w; };
    int getHeight() { return h; };

    double getX() { return pos.getX(); };
    double getY() { return pos.getY(); };
    Vector2D<double> getPosition() { return pos; }

    bool isActive() { return active; };
    void setActive(bool a) { active = a; };

    virtual SDL_Rect getCollider();
    virtual SDL_Rect getCenter();

    virtual bool collide(SDL_Rect other);
};