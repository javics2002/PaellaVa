#pragma once
#include "GameObject.h"
#include "../Control/Input.h"

class Ingrediente;
class Cliente;

enum orientacion { N, S, E, O };

class Player : public GameObject
{
	orientacion orientacion;

	GameObject* objetoCargado_;
	Input* input_;

	float aceleracion, deceleracion, maxVel;

	Ingrediente* myIng = nullptr;
	Cliente* myClient = nullptr;
	//Configuraci�n de skin

	Vector2D<double> overlapPos;
	Vector2D<int> overlapDim;

public:
	Player(Game* game);
	~Player();

	void handleInput();

	void update();

	void moverObjeto(GameObject* objeto);

	SDL_Rect getOverlapCollider();
	void debugOverlap(SDL_Rect& rect);

	//void drawDebug(SDL_Rect& rect) override;
	
};

