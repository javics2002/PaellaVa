#pragma once
#include "GameObject.h"
#include "../Data/Posicion.h"
#include "../Control/Input.h"

class Player : public GameObject
{
private:

	Posicion posicion_;
	GameObject* objetoCargado_;
	Input* input_;

	float velocidad_;
	//Configuración de skin

public:
	Player(Posicion posicion);
	~Player();

	void update();

	void SetVelocidad(float velocidad);
	float GetVelocidad() { return velocidad_; }
	
};

