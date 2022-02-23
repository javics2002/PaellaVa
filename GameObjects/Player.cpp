#include "Player.h"
#include "../Control/Input.h"
#include "../Control/Game.h"

Player::Player(Game* game) : GameObject(game)
{
	//posicion_ = posicion;
	setPosition(100, 100);

	objetoCargado_ = nullptr;
	game_ = game;

	input_ = new Input();
	velocidad_ = 2.0f;
	setTexture("alcachofa"); //añadir imagen de jugador xD
}

Player::~Player()
{
}

void Player::update()
{
	/*float posX = posicion_.GetX();
	float posY = posicion_.GetY();*/

	float posX = getX();
	float posY = getY();


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

	setPosition(posX, posY);
}

void Player::SetVelocidad(float velocidad)
{
	velocidad_ = velocidad;
}

