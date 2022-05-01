#pragma once
#include "../GameObject.h"

class ShowText : public GameObject
{
	string text_;
	string font_;
	int x_;
	int y_;

	SDL_Color bgColor_ ;
	SDL_Color fgColor_ ;

public:
	ShowText(Game* game,  string text,  string font, int x = 0, int y = 0, int w = 0, int h = 0);
	ShowText(Game* game, string text, string font, SDL_Color fgColor, SDL_Color bgColor, int x = 0, int y = 0, int w = 0, int h = 0);

	~ShowText();

	void setText(string	text_);
};

