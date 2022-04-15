#include "ShowText.h"

ShowText::ShowText(Game* game,  string text,  string font, int x, int y,  int w, int h) : GameObject(game)
{
	text_ = text;
	font_ = font;
	x_ = x;
	y_ = y;

	setPosition(x_ + getWidth() / 2, y_ + getHeight() / 2);
	setTexture(text_, font_, fgColor, fgColor);
	setDimension();
}

ShowText::~ShowText()
{
}

void ShowText::setText(string text)
{
	setTexture(text, font_, fgColor, bgColor);
	setDimension();
}
