#pragma once
#include "Scene.h"

class Lobby:public Scene
{

	UiButton* comenzar;
	Imagen* camarero;

public:
	Lobby(Game *game, string nombre, Scene *hostClient);

	Lobby(Game* game, string nombre, string nombreClient);

	void clienteUnido(std::string nombreCliente);
};

