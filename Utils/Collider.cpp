#include "Collider.h"

bool Collider::hasCollision(SDL_Rect rect1, SDL_Rect rect2)
{

	return SDL_HasIntersection(&rect1, &rect2);
}

Vector2D<double> Collider::getRectCenter(SDL_Rect rect)
{
	return Vector2D<double>(rect.x + rect.w / 2, rect.y + rect.h / 2);
}

bool Collider::collide(SDL_Rect other)
{
	return hasCollision(getCollider(), other);
}

bool Collider::overlap(SDL_Rect other)
{
	return hasCollision(getOverlap(), other);
}