#pragma once
#include "Scene.h"

class UIButton;
class Imagen;

class Lobby:public Scene
{

	UiButton* comenzar;
	Imagen* camarero;
	ShowText* buscando;
public:
	Lobby(Game *game);

	Lobby(Game* game, string nombreHost);

	void clienteUnido(std::string nombreCliente);
};

