#include "UiButton.h"
#include "../../sdlutils/InputHandler.h"
#include "../../GameObjects/UI/UIManager.h"

UiButton::UiButton(Game* mGame, string claveTextura, int x, int y, int w, int h) : GameObject(mGame)
{
	Vector2D<double> p;
	p.setX(x);
	p.setY(y);
	setPosition(p);
	setDimension(w, h);
	iniH=h;
	iniW = w;
	setTexture(claveTextura);
	textura = claveTextura;

	canHover = false;
	isHover = false;
}

UiButton::UiButton(Game* mGame, string texto, const string font, const SDL_Color& fgColor, const SDL_Color& bgColor,
	int x, int y) : GameObject(mGame)
{
	setPosition(Vector2D<double>(x, y));
	setTexture(texto, font, fgColor, bgColor);
	setDimension(texture->width(), texture->height());
}

UiButton::~UiButton()
{
}

void UiButton::execute(bool& exit)
{
	callback(mGame, exit);
}

bool UiButton::onClick(int mx, int my, bool& exit)
{
	SDL_Rect z = getCollider();
	SDL_Rect d = { mx,my,1,1 };
	if (SDL_HasIntersection(&z, &d))
	{
		execute(exit);
		return true;
	}
	else
		return false;
}

void UiButton::update()
{
	//Hover con el raton
	if (canHover) {
		if (!isHover && hover()) {
			mGame->getUIManager()->cambiaFoco(botonIterador);
			isHover = true;
		}
		else if (isHover && !hover()) {
			mGame->getUIManager()->quitaFoco();
			isHover = false;
		}
	}
}

void UiButton::setAction(function<void(Game* mGame, bool& exit)> action)
{
	callback = action;
}
void UiButton::setfocused()
{
	setDimension(1.5 * getInitialWidth(), 1.5 * getInitialHeight());
	
}
void UiButton::setunfocused()
{
	setDimension(getInitialWidth(), getInitialHeight());
}

bool UiButton::hover()
{
	//Devuelve si el raton esta encima del boton
	SDL_Rect z = getCollider();
	SDL_Rect d = { ih().getmx(), ih().getmy(), 1, 1 };
	
	return SDL_HasIntersection(&z, &d);
}

void UiButton::setIterador(list<UiButton*>::iterator it)
{
	botonIterador = it;
	canHover = true;
}

list<UiButton*>::iterator UiButton::getIterador()
{
	return botonIterador;
}

