#pragma once
#include "View/Texture.h"
class UiButton
{
private:
	int x;
	int y;
	int w;
	int h;
	Texture* textura;
public:
	UiButton(int x, int y, Texture* texture,int w,int h) :x(x), y(y), textura(texture) ,w(w),h(h){};
	~UiButton();
	bool OnClick(int x, int y);
};

