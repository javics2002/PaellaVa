#include "GameObject.h"

#include "../Control/Game.h"

void GameObject::drawTexture(Texture* texture)
{
	SDL_Rect c = getCollider();
	SDL_Rect textureBox = { c.x, c.y, c.w, c.h };
	texture->render(textureBox);
}

void GameObject::setPosition(double x, double y)
{
	pos = Point2D<double>(x, y);
}


void GameObject::setDimension(double width, double height)
{
	w = width;
	h = height;
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

//bool GameObject::collide(SDL_Rect other)
//{
//	return SDL_HasIntersection(&getCollider(), &other);
//}

void GameObject::drawDebug()
{
	SDL_Rect collider = getCollider();
	SDL_Rect center = getCenter();
	collider = { collider.x, collider.y, collider.w, collider.h };
	center = { center.x, center.y, center.w, center.h };

	SDL_SetRenderDrawColor(game->getRenderer(), 255, 0, 0, 0);
	SDL_RenderDrawRect(game->getRenderer(), &collider);
	SDL_SetRenderDrawColor(game->getRenderer(), 0, 0, 255, 0);
	SDL_RenderFillRect(game->getRenderer(), &center);
	SDL_SetRenderDrawColor(game->getRenderer(), 255, 255, 255, 0);
}

void GameObject::draw()
{
	drawTexture(game->getTexture(textureName));
}

