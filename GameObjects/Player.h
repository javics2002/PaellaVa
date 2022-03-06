#pragma once
#include "GameObject.h"
#include "../Data/Posicion.h"
#include "../Control/Input.h"

class Ingrediente;
class Cliente;

class Player : public GameObject
{
private:

	//Posicion* posicion_;
	GameObject* objetoCargado_;
	Input* input_;

	float velocidad_;

	float aceleracion, deceleracion, maxVel;

	Ingrediente* myIng = nullptr;
	Cliente* myCl = nullptr;
	//Configuración de skin

public:
	Player(Game* game);
	~Player();

	void handleInput();

	void update();

	void SetVelocidad(float velocidad);
	float GetVelocidad() { return velocidad_; }
	
};

