#pragma once

#include <list>
#include "../Utils/Texture.h"

class Collider {
    SDL_Rect objectRect;
   
    bool hasCollision(SDL_Rect rect1, SDL_Rect rect2);

public:
    Collider();

    void setColliderRect(SDL_Rect coll);
    SDL_Rect getCollider();

    void drawDebugColl(SDL_Rect* cameraRect);
    virtual bool collide(SDL_Rect other);
};
