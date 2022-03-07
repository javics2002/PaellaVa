#pragma once
#include "../../Utils/Texture.h"
#include "../GameObject.h"
//#include "Utils/Vector2D.h"
class UiButton: public GameObject
{
private:
	//int x;
	//int y;
	//int w;
	//int h;
	//Texture* textura;
	
public:
	//UiButton(int x, int y, Texture* texture,int w,int h) :x(x), y(y), textura(texture) ,w(w),h(h){};
	UiButton(Game* game, string claveTextura, int x, int y , int w, int h);
	~UiButton();
	virtual bool  OnClick(int mx, int my) override;
	virtual void execute();
	void update() override;
};

