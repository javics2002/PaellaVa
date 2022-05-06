#pragma once
#include "../../Utils/Texture.h"
#include "../GameObject.h"
#include "../../Control/Game.h"
#include <functional>

class UiButton: public GameObject
{
	function<void(Game* mGame, bool& exit)> callback = [](Game* mGame, bool& exit) { };
	bool canHover; //Si el botón reacciona al ratón pasando por él
	bool isHover; //Si el ratón está encima de el
	list<UiButton*>::iterator botonIterador;

protected:
	string textura;
//ool focused = true;
	
public:
	//UiButton(int x, int y, Texture* texture,int w,int h) :x(x), y(y), textura(texture) ,w(w),h(h){};
	UiButton(Game* mGame, string claveTextura, int x, int y , int w, int h);
	UiButton(Game* mGame, string texto, const string font, const SDL_Color& fgColor, const SDL_Color& bgColor, 
		int x, int y);
	~UiButton();
	virtual bool onClick(int mx, int my, bool& exit) override;
	virtual void execute(bool& exit);
	void update() override;
	string getTextura() { return textura; };

	void setAction(function<void(Game* mGame, bool& exit)> action);
	void setfocused();
	void setunfocused();
//ool getfocused() { return focused; };
	
	bool hover();
	void setIterador(list<UiButton*>::iterator it);
	list<UiButton*>::iterator getIterador();
};

