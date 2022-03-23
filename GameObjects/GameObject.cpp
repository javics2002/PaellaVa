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


void GameObject::setDimension() //pone la dimension al tamaño de la textura
{
	if (texture != nullptr) {
	w = texture->width();
	h = texture->height();
	}
	
}


void GameObject::setTexture(string clave)
{
	texture = &sdlutils().images().at(clave);
}

void GameObject::setTexture(const string text, const string font, const SDL_Color& fgColor, const SDL_Color& bgColor)
{
	if (!sdlutils().msgs().count(text))
		sdlutils().msgs().emplace(text, Texture(sdlutils().renderer(), text, sdlutils().fonts().at(font), fgColor));

	texture = &sdlutils().msgs().at(text);
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
	return hasCollision(getCollider(), other);
}

bool GameObject::hasCollision(SDL_Rect rect1, SDL_Rect rect2)
{
	return SDL_HasIntersection(&rect1, &rect2);
}

void GameObject::renderDebug(SDL_Rect* cameraRect)
{
	drawDebug(cameraRect);
}

void GameObject::render(SDL_Rect* cameraRect)
{
	drawRender(cameraRect);
}

void GameObject::drawRender(SDL_Rect* cameraRect)
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

void GameObject::drawRender(SDL_Rect* cameraRect, SDL_Rect rect, Texture* tex)
{
	tex->render({ rect.x - cameraRect->x, rect.y - cameraRect->y, rect.w, rect.h });
}

void GameObject::drawRender(SDL_Rect rect, Texture* tex)
{
	tex->render(rect);
}

void GameObject::drawDebug(SDL_Rect* cameraRect)
{
	SDL_Rect collider = getCollider();
	SDL_Rect center = getCenter();
	collider = { collider.x - cameraRect->x, collider.y - cameraRect->y, collider.w, collider.h };
	center = { center.x - cameraRect->x, center.y - cameraRect->y, center.w, center.h };

	SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 0, 0, 0);
	SDL_RenderDrawRect(sdlutils().renderer(), &collider);
	SDL_SetRenderDrawColor(sdlutils().renderer(), 0, 0, 255, 0);
	SDL_RenderFillRect(sdlutils().renderer(), &center);
	SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 255, 255, 0);
}

void GameObject::drawDebug(SDL_Rect* cameraRect, SDL_Rect rect)
{
	SDL_Rect collider = { rect.x - cameraRect->x, rect.y - cameraRect->y, rect.w, rect.h };

	SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 255, 0, 0);
	SDL_RenderDrawRect(sdlutils().renderer(), &collider);
	SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 255, 255, 0);
}

