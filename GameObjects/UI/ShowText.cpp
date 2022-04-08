#include "ShowText.h"

ShowText::ShowText(Game* game, const string text, const string font, int x, int y, int w, int h, const SDL_Color& fgColor, const SDL_Color& bgColor)
	: GameObject(game)
{
	setPosition(x + getWidth() / 2, y + getHeight() / 2);
	setTexture(text, font, fgColor, bgColor);
	setDimension();
}

ShowText::~ShowText()
{
}
