#pragma once
#include "GameObject.h"
#include "../Control/Input.h"

class Ingrediente;
class Cliente;

class Player : public GameObject
{
	GameObject* objetoCargado_;
	Input* input_;

	float aceleracion, deceleracion, maxVel;

	Ingrediente* myIng = nullptr;
	Cliente* myClient = nullptr;
	//Configuraci�n de skin

public:
	Player(Game* game);
	~Player();

	void handleInput();

	void update();

	void moverObjeto(GameObject* objeto);
	
};

