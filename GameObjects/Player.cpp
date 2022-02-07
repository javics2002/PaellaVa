#include "Player.h"
#include "../Control/Input.h"

Player::Player(Posicion posicion)
{
	posicion_ = posicion;
	objetoCargado_ = nullptr;

	input_ = new Input();
	velocidad_ = 2.0f;
}

Player::~Player()
{
}

void Player::Update()
{
	float posX = posicion.GetX();
	float posY = posicion.GetY();


	if (input_->GetKey(SDL_SCANCODE_W)) {
		posY -= velocidad_;
	}

	if (input_->GetKey(SDL_SCANCODE_S)) {
		posY += velocidad_;
	}

	if (input_->GetKey(SDL_SCANCODE_A)) {
		posX -= velocidad_;
	}

	if (input_->GetKey(SDL_SCANCODE_D)) {
		posX += velocidad_;
	}

	posicion.Set(posX, posY);
}

void Player::SetVelocidad(float velocidad)
{
	velocidad_ = velocidad;
}

