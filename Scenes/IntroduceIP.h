#pragma once
#include "Scene.h"
class UiButton;
class  IntroduceIP :public Scene
{
public:
	IntroduceIP(Game* game, string nombre);
	void handleInput(bool& exit) override;
private:
	string textIP=" ";

	UiButton* IP;
};