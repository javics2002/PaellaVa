#pragma once

#include "GameObject.h"

class Game;

const vector<string> texturasClientes = { "clienteBase", "clienteBase2", "clienteBase3" };

class Cliente : public GameObject
{
	const unsigned int DIMENSION = 100;

	vector<Texture*> anims;
	SDL_RendererFlip flip;
	SDL_Rect clip;
	int frameCounter;
	float lastFrameTime;
	float frameRate;
	int currAnim;

public:
	Cliente(Game* game);
	~Cliente() = default;

	void update() override;
	void render(SDL_Rect* cameraRect) override;

	void cambiaTextura(string textureN);

	void setAnimResources(int clientType);
	void animUpdate(Vector2D<double> axis);
	void setAnim(int animNum);

	Texture* getAnim();
	SDL_Rect getClip();
	SDL_RendererFlip getFlip();

	void drawPickedClient(SDL_Rect* cameraRect, SDL_Rect rect);

};


