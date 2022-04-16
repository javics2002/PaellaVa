#pragma once
#include "Scene.h"

class Lobby:public Scene
{

	UiButton* comenzar;
	Imagen* camarero;

public:
	Lobby(Game *game);

	Lobby(Game* game, string nombreHost);

	void clienteUnido(std::string nombreCliente);
};

