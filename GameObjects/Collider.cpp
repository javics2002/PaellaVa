#include "Collider.h"
#include "../sdlutils/SDLUtils.h"


bool Collider::collide(SDL_Rect other)
{
	return hasCollision(getCollider(), other);
}

bool Collider::hasCollision(SDL_Rect rect1, SDL_Rect rect2)
{
	return SDL_HasIntersection(&rect1, &rect2);
}
