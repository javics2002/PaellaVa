#pragma once
#include "Scene.h"

class Lobby:public Scene
{

	UiButton* comenzar;
	Imagen* camarero;

public:
	Lobby(Game *game, string nombre, Scene *hostClient);

	void clienteUnido();
};

