#pragma once
#include "../../Utils/Texture.h"
#include "../GameObject.h"
#include "../../Control/Game.h"
#include <functional>

class UiButton: public GameObject
{
	function<void(Game* game)> callback = [](Game* game) { cout << "funciona" << endl; };

protected:
	string textura;
	
public:
	//UiButton(int x, int y, Texture* texture,int w,int h) :x(x), y(y), textura(texture) ,w(w),h(h){};
	UiButton(Game* game, string claveTextura, int x, int y , int w, int h);
	UiButton(Game* game, string texto, const string font, const SDL_Color& fgColor, const SDL_Color& bgColor, 
		int x, int y);
	~UiButton();
	virtual bool  OnClick(int mx, int my) override;
	virtual void execute();
	void update() override;
	string getTextura() { return textura; };

	void setAction(function<void(Game* game)> action);
};

