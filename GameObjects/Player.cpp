#include "Player.h"
#include "../Control/Input.h"

Player::Player(Posicion posicion) : GameObject(game)
{
	posicion_ = posicion;
	objetoCargado_ = nullptr;

	input_ = new Input();
	velocidad_ = 2.0f;
	textureName = ingrAlcachofa; //añadir imagen de jugador xD
}

Player::~Player()
{
}

void Player::update()
{
	float posX = posicion_.GetX();
	float posY = posicion_.GetY();


	if (input_->GetKey(SDL_SCANCODE_W)) 
	{
		posY -= velocidad_;
	}

	if (input_->GetKey(SDL_SCANCODE_S)) 
	{
		posY += velocidad_;
	}

	if (input_->GetKey(SDL_SCANCODE_A)) 
	{
		posX -= velocidad_;
	}

	if (input_->GetKey(SDL_SCANCODE_D)) 
	{
		posX += velocidad_;
	}

	posicion_.Set(posX, posY);
}

void Player::SetVelocidad(float velocidad)
{
	velocidad_ = velocidad;
}

