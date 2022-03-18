#pragma once
#include "GameObject.h"
#include "ObjetoPortable.h"
#include "../Control/Input.h"

class Ingrediente;
class Cliente;
class Paella;

enum orientation { N, S, E, O };
enum objectType { INGREDIENTE, CLIENTES, PAELLA};

class Player : public GameObject
{
	orientation orientation_;
	objectType objectType_;

	ObjetoPortable* pickedObject_;
	Input* input_;

	float aceleracion, deceleracion, maxVel;

	Vector2D<double> overlapPos;
	Vector2D<int> overlapDim;

	bool nearestObject(ObjetoPortable* go);

public:
	Player(Game* game);
	~Player();

	void handleInput();

	void update();

	SDL_Rect getOverlapCollider();
	void debugOverlap(SDL_Rect* rect);

	void drawDebug(SDL_Rect* rect) override;
	
};

