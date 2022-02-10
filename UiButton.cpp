#include "UiButton.h"
UiButton::UiButton(Game* game,TextureName texturename, int x, int y, int w, int h):GameObject(game)
{
	Vector2D<double> p;
	p.setX(x);
	p.setY(y);
	setPosition(p);
	setDimension(w, h);
	textureName = texturename;
}
UiButton::~UiButton()
{
}
bool UiButton::OnClick(int mx, int my)
{
	SDL_Rect z=getCollider();
	SDL_Rect d = { mx,my,1,1 };
	if (SDL_HasIntersection(&z, &d))
	{
		cout << "PULSADO";
		return true;
	}
	else
	return false;
}
void UiButton::update()
{
}

