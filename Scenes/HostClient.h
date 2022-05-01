#pragma once
#include "Scene.h"
#include "../Utils/ParticleExample.h"

class HostClient : public Scene
{
	int maxCaracteres = 16;
	string ip_ = "";
	ParticleExample *light;

	ShowText* ip;
	ShowText* cursor;
	ShowText* ipNoValida;
	UiButton* clientButton;
	UiButton* hostButton;

	int posYOver = 0;
	int posYNotOver = 0;

	bool escribiendo = false;
	bool escrito = false;

	float frameRate = 800;
	float tiempo;

	float frameRateIpNV = 500;
	float tiempoIpNV;

	bool esValida(string ipText);
	vector<string>split(string ipText);

public:
	HostClient(Game* game);

	void update() override;
	void render() override;
};

