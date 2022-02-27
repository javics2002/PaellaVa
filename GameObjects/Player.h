#pragma once
#include "GameObject.h"
#include "../Data/Posicion.h"
#include "../Control/Input.h"

class Player : public GameObject
{
private:

	//Posicion* posicion_;
	GameObject* objetoCargado_;
	Input* input_;
	Game* game_;

	float velocidad_;
	//Configuración de skin

public:
	Player(Game* game);
	~Player();

	void handleInput();

	void update();

	void SetVelocidad(float velocidad);
	float GetVelocidad() { return velocidad_; }
	
};

