#include "Player.h"
#include "../Control/Input.h"
#include "../Control/Game.h"
#include "../sdlutils/InputHandler.h"

Player::Player(Game* game) : GameObject(game)
{
	//posicion_ = posicion;
	setPosition(100, 100);
	setDimension(50, 50);

	objetoCargado_ = nullptr;
	game_ = game;

	input_ = new Input();
	velocidad_ = 2.0f;
	setTexture("alcachofa"); //añadir imagen de jugador xD
}

Player::~Player()
{
}

void Player::handleInput()
{
	bool changed = false;

	float posX = getX();
	float posY = getY();

	if (ih().isJoystickEvent())
	{
		cout << "Eje X: " << ih().getAxisX() << " Eje Y: " << ih().getAxisY();
	}
	else if (ih().keyDownEvent())
	{
		if (ih().isKeyDown(SDL_SCANCODE_W))
		{
			posY -= velocidad_;
			changed = true;
		}

		if (ih().isKeyDown(SDL_SCANCODE_S))
		{
			posY += velocidad_;
			changed = true;
		}

		if (ih().isKeyDown(SDL_SCANCODE_A))
		{
			posX -= velocidad_;
			changed = true;
		}

		if (ih().isKeyDown(SDL_SCANCODE_D))
		{
			posX += velocidad_;
			changed = true;
		}
	}

	setPosition(posX, posY);
}

void Player::update()
{
	//bool changed = false;

	//float posX = getX();
	//float posY = getY();


	//if (input_->GetKey(SDL_SCANCODE_W)) 
	//{
	//	posY -= velocidad_;
	//	changed = true;
	//}

	//if (input_->GetKey(SDL_SCANCODE_S)) 
	//{
	//	posY += velocidad_;
	//	changed = true;
	//}

	//if (input_->GetKey(SDL_SCANCODE_A)) 
	//{
	//	posX -= velocidad_;
	//	changed = true;
	//}

	//if (input_->GetKey(SDL_SCANCODE_D)) 
	//{
	//	posX += velocidad_;
	//	changed = true;
	//}

	//setPosition(posX, posY);

	//if (!changed)
	//{
	//	input_->UpdateAxis();

	//	Sint16 ejeX = input_->getAxisX();
	//	Sint16 ejeY = input_->getAxisY();

	//	if (ejeX != 0 || ejeY != 0)
	//		cout << "ejeX: " << ejeX << " ejeY: " << ejeY << endl;


	//}



}

void Player::SetVelocidad(float velocidad)
{
	velocidad_ = velocidad;
}

