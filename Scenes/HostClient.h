#pragma once
#include "Scene.h"
class HostClient : public Scene
{
	int maxCaracteres = 15;
	string ip_ = "";
	UiButton* introIp;

	SDL_Rect clip;
	Texture* libretaTexture = &sdlutils().images().at("libreta");
public:
	HostClient(Game* game);
};

