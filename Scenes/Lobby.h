#pragma once
#include "Scene.h"

class UIButton;
class Imagen;

class Lobby : public Scene
{
	UiButton* mStart;
	Imagen* mCamarero;
	ShowText* mSearching;
	bool mConnected;

public:
	Lobby(Game *mGame);
	Lobby(Game* mGame, string nombreHost);

	void clienteUnido(std::string nombreCliente);
};

