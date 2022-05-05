#include "ShowText.h"

ShowText::ShowText(Game* mGame,  string text, string font, int x, int y,  int w, int h) : GameObject(mGame)
{
	text_ = text;
	font_ = font;
	x_ = x;
	y_ = y;
	fgColor_ = { 255, 255, 255, 255 };
	bgColor_ = { 0, 0, 0, 0 };
	

	setPosition(x_ + getWidth() / 2, y_ + getHeight() / 2);
	setTexture(text_, font_, fgColor_, fgColor_);
	setDimension();
}

ShowText::ShowText(Game* mGame, string text, string font, SDL_Color fgColor, SDL_Color bgColor, int x, int y, int w, int h) : GameObject(mGame)
{
	text_ = text;
	font_ = font;
	x_ = x;
	y_ = y;
	fgColor_ = fgColor;
	bgColor_ = bgColor;


	setPosition(x_ + getWidth() / 2, y_ + getHeight() / 2);
	setTexture(text_, font_, fgColor_, fgColor_);
	setDimension();
}

ShowText::~ShowText()
{
}

void ShowText::setText(string text)
{
	setTexture(text, font_, fgColor_, bgColor_);
	setDimension();
}
