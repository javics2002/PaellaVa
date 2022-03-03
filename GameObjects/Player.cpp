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

	aceleracion = 0.8;
	deceleracion = 0.4;
	maxVel = 5;

	setTexture("alcachofa"); //añadir imagen de jugador xD
}

Player::~Player()
{
}

void Player::handleInput()
{
	vel = (vel + ih().getAxis() * aceleracion);

	if (vel.getX() != 0 && ih().getAxis().getX() == 0) { // aplicar rozamiento
		if (vel.getX() < 0) {
			vel.setX(vel.getX() + deceleracion);
			if (std::round(vel.getX()) == 0) vel.setX(0);
		}
		else {
			vel.setX(vel.getX() - deceleracion);
			if (std::round(vel.getX()) == 0) vel.setX(0);
		}

		
	}
	if (vel.getY() != 0 && ih().getAxisY() == 0) { // aplicar rozamiento
		if (vel.getY() < 0) {
			vel.setY(vel.getY() + deceleracion);
			if (std::round(vel.getY()) == 0) vel.setY(0);
		}
		else {
			vel.setY(vel.getY() - deceleracion);
			if (std::round(vel.getY()) == 0) vel.setY(0);
		}
	}

	vel.clamp(-maxVel, maxVel);
}

void Player::update()
{
	// cout << ih().getAxisX() << " " << ih().getAxisY() << endl;
	pos = pos + vel;
}

void Player::SetVelocidad(float velocidad)
{
	velocidad_ = velocidad;
}

