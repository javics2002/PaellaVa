#include "Mueble.h"
#include "../../Control/Game.h"

Mueble::Mueble(Game* game, Vector2D<double> position, int width, int height, string claveTextura) : GameObject(game)
{
	setDimension(width, height);
	setPosition(position.getX() + getWidth() / 2, position.getY() + getHeight() / 2);
	setTexture(claveTextura);
	setColliderRect({ (int)getX(), (int)getY(), w, h });

	timerTexture = &sdlutils().images().at("timer");
}
