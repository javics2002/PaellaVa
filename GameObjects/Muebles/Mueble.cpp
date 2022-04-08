#include "Mueble.h"
#include "../../Control/Game.h"

Mueble::Mueble(Game* game, Vector2D<double> position, int width, int height, string claveTextura) : GameObject(game)
{
	setDimension(width, height);
	setPosition(position.getX() + getWidth() / 2, position.getY() + getHeight() / 2);
	setTexture(claveTextura);

	timerTexture = &sdlutils().images().at("timer");
}

SDL_Rect Mueble::getCollider()
{
	SDL_Rect rect = getTexBox();

	return {
		rect.x,
		rect.y + rect.h / 3,
		rect.w,
		rect.h / 3 * 2
	};
}

SDL_Rect Mueble::getOverlap()
{
	SDL_Rect rect = getTexBox();

	return {
		rect.x,
		rect.y,
		rect.w,
		rect.h / 2
	};
}
