#pragma once

#include <list>
#include "../Utils/Texture.h"

class Player;

class PoolObject;

class Silla;
class Mesa;

class Collider {

    SDL_Rect objectRect;
    Texture* debugTexture = nullptr;

    bool hasCollision(SDL_Rect rect1, SDL_Rect rect2);

public:
    Collider();

    void setColliderRect(SDL_Rect coll);

    SDL_Rect getCollider2();

    void drawDebugColl(SDL_Rect* cameraRect);

    virtual std::pair<bool, std::list<PoolObject*>::const_iterator> colisionIngrediente() {
        return { false , std::list<PoolObject*>::const_iterator() };
    };
    virtual bool colisionClientes() {
        return false;
    }
    virtual bool colisionPlayer(Player* p) {
        return false;
    }
    virtual bool ratonEncima() {
        return false;
    }
    virtual Silla* initMesa(Mesa* mesa) {
        return nullptr;
    }

    virtual bool collide(SDL_Rect other);
};
