#include "Collider.h"

bool Collider::hasCollision(SDL_Rect rect1, SDL_Rect rect2)
{
	return SDL_HasIntersection(&rect1, &rect2);
}

bool Collider::collide(SDL_Rect other)
{
	return hasCollision(getCollider(), other);
}

bool Collider::overlap(SDL_Rect other)
{
	return hasCollision(getOverlap(), other);
}