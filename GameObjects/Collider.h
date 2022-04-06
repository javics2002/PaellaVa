#pragma once

#include <list>
#include "../Utils/Texture.h"

class Collider {

protected:
    bool hasCollision(SDL_Rect rect1, SDL_Rect rect2);

public:
    Collider() {};

    virtual SDL_Rect getCollider() = 0;
    void drawDebugColl(SDL_Rect* cameraRect);
    virtual bool collide(SDL_Rect other);
};
