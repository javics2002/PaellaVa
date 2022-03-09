#include "Player.h"
#include "../Control/Input.h"
#include "../Control/Game.h"
#include "../sdlutils/InputHandler.h"
#include "../Control/ObjectManager.h"
#include "Ingrediente.h"
#include "Cliente.h"
#include "../Utils/Traza.h"


Player::Player(Game* game) : GameObject(game)
{

	//posicion_ = posicion;
	setPosition(100, 100);
	setDimension(64, 64);

	objetoCargado_ = nullptr;

	aceleracion = 1.2;
	deceleracion = 0.8;
	maxVel = 7;

	myIng = nullptr;

	//TRACE_F(__FILE__ , __LINE__,   maxVel);
	
	setTexture("player");
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
		if (myIng == nullptr && myClient == nullptr)
		{
			cout << "E pressed" << endl;

			vector<Collider*> ingredientes = game->getObjectManager()->getIngredientes(this->getCollider());
			cout << "Ingredientes colisionando " << ingredientes.size() << endl;
			TRACE(ingredientes.size());
			for (auto it : ingredientes)
			{
				if (myIng == nullptr)
				{
					myIng = dynamic_cast<Ingrediente*>(it);
					myIng->ingredienteRecogido();
				}
			}

			if (!myIng)
			{
				vector<Collider*> clientes = game->getObjectManager()->getClientes(this->getCollider());
				cout << "Clientes colisionando " << clientes.size() << endl;
				TRACE(clientes.size());
				for (auto it : clientes)
				{
					if (myClient == nullptr)
					{
						myClient = dynamic_cast<Cliente*>(it);
						myClient->clienteRecogido();
					}
				}
			}

			
		}
		else if (myIng != nullptr)
		{
			myIng = nullptr;
		}
		else if (myClient != nullptr)
		{
			myClient = nullptr;
		}
	}

}

void Player::update()
{
	//cout << ih().getAxisX() << " " << ih().getAxisY() << endl;
	pos = pos + vel;
	
	if (myIng != nullptr)
	{
		moverObjeto(myIng);
	}

	if (myClient != nullptr)
	{
		moverObjeto(myClient);
	}
}

void Player::moverObjeto(GameObject* objeto)
{
	Vector2D<double> objPos = pos;

	objPos.setY(objPos.getY() - this->getHeight() / 2);

	objeto->setPosition(objPos);
}

