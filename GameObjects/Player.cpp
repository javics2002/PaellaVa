#include "Player.h"
#include "../Control/Input.h"
#include "../Control/Game.h"
#include "../sdlutils/InputHandler.h"
#include "../Control/ObjectManager.h"
#include "Ingrediente.h"
#include "Cliente.h"
#include "../Utils/Traza.h"


Player::Player(Game* game) : GameObject(game), 
	overlapPos(Vector2D<double>(getX() - overlapPos.getX() / 2, getY() - getHeight() / 2 - overlapDim.getY())), overlapDim(Vector2D<int>(50, 50))
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


	//Altura
	if (vel.getY() != 0 && ih().getAxisY() == 0) { 
		//Arriba
		if (vel.getY() < 0) {
			
			vel.setY(vel.getY() + deceleracion);
			if (std::round(vel.getY()) == 0) vel.setY(0);
		}
		//Abajo
		else {
			
			vel.setY(vel.getY() - deceleracion);
			if (std::round(vel.getY()) == 0) vel.setY(0);
		}
	}
	//Lados
	if (vel.getX() != 0 && ih().getAxis().getX() == 0) {
		//Izquierda
		if (vel.getX() < 0) {			
			
			vel.setX(vel.getX() + deceleracion);
			if (std::round(vel.getX()) == 0) vel.setX(0);
		}
		//Derecha
		else {
			
			vel.setX(vel.getX() - deceleracion);
			if (std::round(vel.getX()) == 0) vel.setX(0);
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
		if (myIng->isActive())
			moverObjeto(myIng);
		else
			myIng = nullptr;
	}

	if (myClient != nullptr)
	{
		if (myClient->isActive())
			moverObjeto(myClient);
		else
			myClient = nullptr;
	}

	if (ih().getAxisY() == 1) 
		orientacion = S;		
	else if (ih().getAxisY() == -1)
		orientacion = N;		

	if (ih().getAxisX() == 1)
		orientacion = E;		
	else if (ih().getAxisX() == -1) 
		orientacion = O;


	switch (orientacion)
	{
	case E:
		overlapPos = Vector2D<double>(getX() + getWidth() / 2,
			getY() - overlapDim.getY() / 2);
		break;
	case O:
		overlapPos = Vector2D<double>(getX() - getWidth() / 2 - overlapDim.getX(),
			getY() - overlapDim.getY() / 2);
		break;
	case S:
		overlapPos = Vector2D<double>(getX() - overlapDim.getX() / 2,
			getY() + getHeight() / 2);		
		break;
	case N:
		overlapPos = Vector2D<double>(getX() - overlapDim.getX() / 2,
			getY() - getHeight() / 2 - overlapDim.getY());
		break;		
	default:
		break;
	}
}

void Player::moverObjeto(GameObject* objeto)
{
	Vector2D<double> objPos = pos;

	objPos.setY(objPos.getY() - this->getHeight() / 2);

	objeto->setPosition(objPos);
}

SDL_Rect Player::getOverlapCollider()
{
	return { int(overlapPos.getX()),
		 int(overlapPos.getY()),
		  (overlapDim.getX()),
		  (overlapDim.getY()) };
}

void Player::debugOverlap(SDL_Rect& rect)
{
	SDL_Rect collider = getOverlapCollider();
	collider = { collider.x - rect.x, collider.y - rect.y, collider.w, collider.h };

	SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 255, 0, 0);
	SDL_RenderDrawRect(sdlutils().renderer(), &collider);
	SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 255, 255, 0);

	
}
//
//void Player::drawDebug(SDL_Rect& rect)
//{
//	debugOverlap(rect);
//
//	SDL_Rect collider = getCollider();
//	SDL_Rect center = getCenter();
//	collider = { collider.x - rect.x, collider.y - rect.y, collider.w, collider.h };
//	center = { center.x - rect.x, center.y - rect.y, center.w, center.h };
//
//	SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 0, 0, 0);
//	SDL_RenderDrawRect(sdlutils().renderer(), &collider);
//	SDL_SetRenderDrawColor(sdlutils().renderer(), 0, 0, 255, 0);
//	SDL_RenderFillRect(sdlutils().renderer(), &center);
//	SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 255, 255, 0);
//}

