#include "Player.h"
#include "../Control/Input.h"
#include "../Control/Game.h"
#include "../sdlutils/InputHandler.h"
#include "../Control/ObjectManager.h"
#include "Ingrediente.h"

#define TRACE_F(F, L, T) (cout << F <<":" << L << " "<<#T << ": " << T << endl )
#define TRACE( T) (cout << #T << ": " << T << endl )
Player::Player(Game* game) : GameObject(game)
{

	//posicion_ = posicion;
	setPosition(100, 100);
	setDimension(50, 50);

	objetoCargado_ = nullptr;

	aceleracion = 1.2;
	deceleracion = 0.8;
	maxVel = 7;

	myIng = nullptr;

	//TRACE_F(__FILE__ , __LINE__,   maxVel);
	
	setTexture("alcachofa"); //añadir imagen de jugador xD
}

Player::~Player()
{
}

void Player::handleInput()
{
	vel = (vel + ih().getAxis() * aceleracion);


	// esto es un test
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

	if (ih().isKeyboardEvent() && ih().getKeyPressed() == SDL_SCANCODE_E)
	{
		if (myIng == nullptr)
		{
			cout << "E pressed" << endl;
			vector<Collider*> ingredientes = game->getObjectManager()->getIngredientes(this->getCollider());
			cout << "Ingredientes colisionando " << ingredientes.size() << endl;
			TRACE(ingredientes.size());
			for (auto it : ingredientes)
			{
				myIng = dynamic_cast<Ingrediente*>(it);
				myIng->ingredienteRecogido();
			}
		}
	}
}

void Player::update()
{
	//cout << ih().getAxisX() << " " << ih().getAxisY() << endl;
	pos = pos + vel;
	
	if (myIng != nullptr)
	{
		myIng->setPosition(pos);
	}
}

void Player::SetVelocidad(float velocidad)
{
	velocidad_ = velocidad;
}

