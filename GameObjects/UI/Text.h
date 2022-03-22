#pragma once
#include "../GameObject.h"
class Text : public GameObject
{
public:
	Text(Game* game, const string text, const string font, int x = 0, int y = 0, int w = 0, int h = 0,
		const SDL_Color& fgColor = { 255, 255, 255, 255 }, const SDL_Color& bgColor = { 0, 0, 0, 0 });
	~Text();
};

