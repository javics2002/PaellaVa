#include "UiButton.h"
UiButton::UiButton(Game* game, string claveTextura, int x, int y, int w, int h):GameObject(game)
{
	Vector2D<double> p;
	p.setX(x);
	p.setY(y);
	setPosition(p);
	setDimension(w, h);
	setTexture(claveTextura);
	textura = claveTextura;
}
UiButton::~UiButton()
{
}
void UiButton::execute()
{
}
bool UiButton::OnClick(int mx, int my)
{
	SDL_Rect z=getCollider();
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

