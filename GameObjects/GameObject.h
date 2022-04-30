#pragma once

#include "../Utils/Texture.h"
#include "../Utils/Vector2D.h"
#include "../Utils/Collider.h"
#include "../sdlutils/SDLUtils.h"

#include <string>

class Game;
class ObjectManager;

using namespace std;

class GameObject : public Collider
{
    const int CENTER_TAM = 4;
    bool active = true;

protected:
    Vector2D<double> pos, vel;
    int w, h;
    int iniW, iniH;
    double iniX, iniY;
    int z; // para el render

    Game* game;
    Texture* texture;
   
    int canalSonido;

    int mId; // Id del objeto

    void drawRender(SDL_Rect* cameraRect);
    void drawRender(SDL_Rect* cameraRect, SDL_Rect rect, Texture* tex);
    void drawRender(SDL_Rect rect, Texture* tex);
    void drawRender(SDL_Rect* cameraRect, SDL_Rect rect, Texture* tex, SDL_Rect clip);
    void drawRender(SDL_Rect* cameraRect, SDL_Rect rect, Texture* tex, SDL_Rect clip, SDL_RendererFlip flip);
    void drawRender(SDL_Rect* cameraRect, Uint8 alpha);

    void drawDebug(SDL_Rect* cameraRect); 
    void drawDebug(SDL_Rect* cameraRect, SDL_Rect rect);    

public:

    GameObject(Game* game) 
        : game(game), texture(nullptr), w(0), h(0), z(1) {};
    GameObject(Game* game, string claveTextura) 
        : game(game), texture(&sdlutils().images().at(claveTextura)), w(0), h(0), z(1) {};

    virtual ~GameObject() {};

    virtual void update() {
        //pos = pos + vel;
    };


    

    void setVel(Vector2D<double> vel_) { vel = vel_; };

    virtual void init(ObjectManager* objectManager) {};

    virtual void render(SDL_Rect* cameraRect);
    virtual void renderDebug(SDL_Rect* cameraRect); 

    virtual bool onClick(int mx, int my, bool& exit) { return false; };

    void setPosition(double x, double y);
    void setPosition(Vector2D<double> v);
    void setDimension(int width, int height);
    void setDimension();

    void setInitialPosition(int xPos, int yPos);
    int getInitialPositionX() { return iniX; }
    int getInitialPositionY() { return iniY; }

    void setInitialDimension(int width, int height);
    int getInitialWidth() { return iniW; }
    int getInitialHeight() { return iniH; }

    void setTexture(string clave);
    void setTexture(const string text, const string font, const SDL_Color& fgColor, const SDL_Color& bgColor);

    int getWidth() { return w; };
    int getHeight() { return h; };

    void setDepth(int depth) { z = depth; }
    int getDepth() { return z; }

    double getX() { return pos.getX(); };
    double getY() { return pos.getY(); };
    Vector2D<double> getPosition() { return pos; }

    virtual void onActivate() {}
    virtual void onDeactivate() {}

    virtual SDL_Rect getTexBox();
    virtual SDL_Rect getCenter();

    bool isActive() { return active; }
    void setActive(bool a) { active = a; }

    SDL_Rect getCollider() override;
    SDL_Rect getOverlap() override;

    void anim(SDL_Rect* cameraRect, SDL_Rect rect, Texture* tex, SDL_Rect clip);

    void setId(int newId) { mId = newId; }
    int getId() { return mId; }
};