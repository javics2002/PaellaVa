#pragma once

#include <list>
#include "../Utils/Texture.h"

class Collider {

protected:
    bool hasCollision(SDL_Rect rect1, SDL_Rect rect2);
    Vector2D<double> getRectCenter(SDL_Rect rect);

public:
    Collider() {};

    virtual SDL_Rect getCollider() = 0;
    virtual bool collide(SDL_Rect other);

    virtual SDL_Rect getOverlap() = 0;
    virtual bool overlap(SDL_Rect other);
};
