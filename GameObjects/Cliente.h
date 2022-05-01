#pragma once

#include "GameObject.h"

class Game;

class Cliente : public GameObject
{
	const unsigned int DIMENSION = 150;

	vector<Texture*> anims;
	SDL_RendererFlip flip;
	SDL_Rect clip;
	int frameCounter;
	float lastFrameTime;
	float frameRate;
	int currAnim;

	bool gosht;

public:
	Cliente(Game* game);
	~Cliente() = default;

	void update() override;
	void render(SDL_Rect* cameraRect) override;

	void setAnimResources(int clientType);
	void animUpdate(Vector2D<double> axis);
	void setAnim(int animNum);

	Texture* getAnim();
	SDL_Rect getClip();


	SDL_Rect getCollider() override;
	SDL_Rect getOverlap() override;

	void setGoshtClient(bool b);
	void drawPickedClient(SDL_Rect* cameraRect, SDL_Rect rect);

	void setFlip(SDL_RendererFlip f);
};


