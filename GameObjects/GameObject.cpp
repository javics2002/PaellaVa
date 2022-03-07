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

void GameObject::drawDebug()
{
	SDL_Rect collider = getCollider();
	SDL_Rect center = getCenter();
	collider = { collider.x, collider.y, collider.w, collider.h };
	center = { center.x, center.y, center.w, center.h };

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

void GameObject::render()
{
	SDL_Rect c = getCollider();
	SDL_Rect textureBox = { c.x, c.y, c.w, c.h };
	texture->render(textureBox);
}

void GameObject::render(SDL_Rect& cameraRect)
{
	SDL_Rect c = getCollider();
	SDL_Rect textureBox = { c.x, c.y, c.w, c.h };
	texture->render(cameraRect, textureBox);
}

