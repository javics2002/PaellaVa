#include "GameObject.h"

#include "../Control/Game.h"

void GameObject::setPosition(double x, double y)
{
	pos = Point2D<double>(x, y);
}

void GameObject::setPosition(Vector2D<double> v)
{
	pos = v;
}

void GameObject::setDimension(int width, int height)
{
	w = width;
	h = height;
}

void GameObject::setTexture(string clave)
{
	texture = &sdlutils().images().at(clave);
}

SDL_Rect GameObject::getCollider()
{
	return { int(getX() - getWidth() / 2),
			 int(getY() - getHeight() / 2),
			  (getWidth()),
			  (getHeight()) };
}

SDL_Rect GameObject::getCenter()
{
	return { int(getX() - CENTER_TAM / 2),
			 int(getY() - CENTER_TAM / 2),
			CENTER_TAM,
			CENTER_TAM };
}

void GameObject::drawDebug(SDL_Rect* rect)
{
	SDL_Rect collider = getCollider();
	SDL_Rect center = getCenter();
	collider = { collider.x - rect->x, collider.y - rect->y, collider.w, collider.h };
	center = { center.x - rect->x, center.y - rect->y, center.w, center.h };

	SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 0, 0, 0);
	SDL_RenderDrawRect(sdlutils().renderer(), &collider);
	SDL_SetRenderDrawColor(sdlutils().renderer(), 0, 0, 255, 0);
	SDL_RenderFillRect(sdlutils().renderer(), &center);
	SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 255, 255, 0);
}

bool GameObject::collide(SDL_Rect other)
{
	SDL_Rect rect1 = getCollider();
	SDL_Rect rect2 = other;

	return SDL_HasIntersection(&rect1, &rect2);
}

void GameObject::render(SDL_Rect* cameraRect=nullptr)
{
	SDL_Rect c = getCollider();
	SDL_Rect textureBox;

	if (cameraRect != nullptr) {
		textureBox = { c.x - cameraRect->x, c.y - cameraRect->y, c.w, c.h };
	}
	else {
		textureBox = { c.x, c.y, c.w, c.h };
	}
	texture->render(textureBox);
}

