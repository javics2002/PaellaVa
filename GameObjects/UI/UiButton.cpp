#include "UiButton.h"
UiButton::UiButton(Game* game, string claveTextura, int x, int y, int w, int h) :GameObject(game)
{
	Vector2D<double> p;
	p.setX(x);
	p.setY(y);
	setPosition(p);
	setDimension(w, h);
	setTexture(claveTextura);
	textura = claveTextura;
}

UiButton::UiButton(Game* game, string texto, const string font, const SDL_Color& fgColor, const SDL_Color& bgColor,
	int x, int y) : GameObject(game)
{
	setPosition(Vector2D<double>(x, y));
	setTexture(texto, font, fgColor, bgColor);
	setDimension(texture->width(), texture->height());
}

UiButton::~UiButton()
{
}

void UiButton::execute()
{
	callback(game);
}

bool UiButton::OnClick(int mx, int my)
{
	SDL_Rect z = getCollider();
	SDL_Rect d = { mx,my,1,1 };
	if (SDL_HasIntersection(&z, &d))
	{
		execute();
		cout << "PULSADO";
		return true;
	}
	else
		return false;
}

void UiButton::update()
{
}

void UiButton::setAction(function<void(Game* game)> action)
{
	callback = action;
}

