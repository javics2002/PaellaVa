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

void GameObject::setDimension() //pone la dimension al tama�o de la textura
{
	if (texture != nullptr) {
		w = texture->width();
		h = texture->height();
	}
}

void GameObject::setInitialDimension(int width, int height)
{
	iniW = width;
	iniH = height;
}

void GameObject::setTexture(string clave)
{
	texture = &sdlutils().images().at(clave);
}

void GameObject::setTexture(const string text, const string font, const SDL_Color& fgColor, const SDL_Color& bgColor)
{
	if (!sdlutils().messages().count(text))
		sdlutils().messages().emplace(text, Texture(sdlutils().renderer(), text, sdlutils().fonts().at(font), fgColor));

	texture = &sdlutils().messages().at(text);
}

SDL_Rect GameObject::getTexBox()
{
	return { int(getX() - getWidth() / 2),
			 int(getY() - getHeight() / 2),
			  (getWidth()),
			  (getHeight()) };
}

SDL_Rect GameObject::getCollider()
{
	return getTexBox();
}

SDL_Rect GameObject::getOverlap()
{
	return getTexBox();
}

SDL_Rect GameObject::getCenter()
{
	return { int(getX() - CENTER_TAM / 2),
			 int(getY() - CENTER_TAM / 2),
			CENTER_TAM,
			CENTER_TAM };
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
	SDL_Rect c = getTexBox();
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

void GameObject::drawRender(SDL_Rect* cameraRect, SDL_Rect rect, Texture* tex, SDL_Rect clip)
{
	tex->render(clip, { rect.x - cameraRect->x, rect.y - cameraRect->y, rect.w, rect.h }, 0.0);
}

void GameObject::drawRender(SDL_Rect* cameraRect, SDL_Rect rect, Texture* tex, SDL_Rect clip, SDL_RendererFlip flip)
{
	const SDL_Point* p = (const SDL_Point*)nullptr;
	tex->render(clip, { rect.x - cameraRect->x, rect.y - cameraRect->y, rect.w, rect.h }, 0.0, p, flip);
}

void GameObject::drawDebug(SDL_Rect* cameraRect)
{
	SDL_Rect overlap = getOverlap();
	SDL_Rect collider = getCollider();	
	SDL_Rect center = getCenter();	

	overlap = { overlap.x - cameraRect->x, overlap.y - cameraRect->y, overlap.w, overlap.h };
	collider = { collider.x - cameraRect->x, collider.y - cameraRect->y, collider.w, collider.h };
	center = { center.x - cameraRect->x, center.y - cameraRect->y, center.w, center.h };

	SDL_SetRenderDrawColor(sdlutils().renderer(), 0, 255, 0, 0);
	SDL_RenderDrawRect(sdlutils().renderer(), &overlap);
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


