#include "Text.h"

Text::Text(Game* game, const string text, const string font, const SDL_Color& fgColor, const SDL_Color& bgColor) 
	: GameObject(game)
{
	setDimension(50, 50);
	setPosition(200 + getWidth() / 2, 200 + getHeight() / 2);
	setTexture(text, font, fgColor, bgColor);
}

Text::~Text()
{
}
