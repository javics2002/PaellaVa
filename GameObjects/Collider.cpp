#include "Collider.h"
#include "../sdlutils/SDLUtils.h"

Collider::Collider()
{
}

void Collider::setColliderRect(SDL_Rect coll)
{
    objectRect = coll;
}

SDL_Rect Collider::getCollider()
{
	return { int(objectRect.x - objectRect.w / 2),
			 int(objectRect.y - objectRect.h / 2),
			  (objectRect.w),
			  (objectRect.h) };
}

bool Collider::collide(SDL_Rect other)
{
	return hasCollision(getCollider(), other);
}

bool Collider::hasCollision(SDL_Rect rect1, SDL_Rect rect2)
{
	return SDL_HasIntersection(&rect1, &rect2);
}

void Collider::drawDebugColl(SDL_Rect* cameraRect)
{
	//SDL_Rect center = getCenter();
	SDL_Rect collider = getCollider();
	SDL_Rect drawColl = { collider.x - cameraRect->x, collider.y - cameraRect->y, collider.w, collider.h };
	//center = { center.x - cameraRect->x, center.y - cameraRect->y, center.w, center.h };

	SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 0, 0, 0);
	SDL_RenderDrawRect(sdlutils().renderer(), &drawColl);
	SDL_SetRenderDrawColor(sdlutils().renderer(), 0, 0, 255, 0);
	//SDL_RenderFillRect(sdlutils().renderer(), &center);
	//SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 255, 255, 0);
}