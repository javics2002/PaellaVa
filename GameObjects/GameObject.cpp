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

