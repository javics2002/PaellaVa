#include "Player.h"

#include "../Control/Input.h"
#include "../sdlutils/InputHandler.h"
#include "../Control/Game.h"
#include "../Control/ObjectManager.h"

#include "Ingrediente.h"
#include "Muebles/Mueble.h"

#include "../Utils/Traza.h"


Player::Player(Game* game) : GameObject(game), objectType_(INGREDIENTE), pickedObject_(nullptr),
	overlapPos(Vector2D<double>(getX() - overlapPos.getX() / 2, getY() - getHeight() / 2 - overlapDim.getY())), overlapDim(Vector2D<int>(50, 50))
{

	setPosition(100, 100);
	setDimension(64, 64);
	overlapDim.set(50, 50); //margen de choque para fluidez


	aceleracion = 1.2;
	deceleracion = 0.8;
	maxVel = 7;

	
	setTexture("player");

	lastTime_ = SDL_GetTicks();
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
		if (vel.getY() < 0) 
		{
			{
				vel.setY(vel.getY() + deceleracion);
				if (std::round(vel.getY()) == 0) vel.setY(0);
			}
		}
		//Abajo
		else 
		{
			{
				vel.setY(vel.getY() - deceleracion);
				if (std::round(vel.getY()) == 0) vel.setY(0);
			}
		}
	}
	//Lados
	if (vel.getX() != 0 && ih().getAxis().getX() == 0) {
		//Izquierda
		if (vel.getX() < 0) {			
			{
				vel.setX(vel.getX() + deceleracion);
				if (std::round(vel.getX()) == 0) vel.setX(0);
			}
		}
		//Derecha
		else {
			{
				vel.setX(vel.getX() - deceleracion);
				if (std::round(vel.getX()) == 0) vel.setX(0);
			}
		}
	}
	vel.clamp(-maxVel, maxVel);
	


	if (ih().getKey(InputHandler::INTERACT) && SDL_GetTicks() - lastTime_ > 500) {
		//Este lastTime_ peruano se quitará en un futuro
		lastTime_ = SDL_GetTicks();

		//Si el jugador no lleva nada encima
		if (pickedObject_ == nullptr) {

			//Se prioriza la interacci�n con los muebles por encima de otros objetos
			//Se prioriza el mueble m�s cercano al jugador
			Mueble* m = nullptr;
			for (auto i : game->getObjectManager()->getMueblesCollider(getOverlapCollider())) {
				m = nearestObject(m, dynamic_cast<Mueble*>(i));
			}
			
			//Si se ha encontrado un mueble, se intenta interactuar con 
			//este con returnObject(), para que te devuelva el objeto
			if (m != nullptr && m->returnObject(this)) 
			{
				assert(pickedObject_ != nullptr);
				pickedObject_->pickObject();
			}

			//En caso contrario se recorre el resto de objetos del juego para ver si el jugador puede cogerlos
			//Una vez m�s se prioriza el objeto m�s cercano
			else
			{
				//Ingredientes
				for (auto i : game->getObjectManager()->getPoolIngredientes()->getCollisions(getOverlapCollider())) {
					ObjetoPortable* op = dynamic_cast<ObjetoPortable*>(i);
					if (op->canPick() && nearestObject(op))
						objectType_ = INGREDIENTE;
				}

				//Grupo de Clientes
				for (auto i : game->getObjectManager()->getPoolGrupoClientes()->getCollisions(getOverlapCollider())) {
					ObjetoPortable* op = dynamic_cast<ObjetoPortable*>(i);
					if (op->canPick() && nearestObject(op))
						objectType_ = CLIENTES;
				}

				//Paellas
				for (auto i : game->getObjectManager()->getPaellasCollider(getOverlapCollider())) {
					ObjetoPortable* op = dynamic_cast<ObjetoPortable*>(i);
					if (op->canPick() && nearestObject(op))
						objectType_ = PAELLA;
				}

				//Una vez encontrado el m�s cercano, se interact�a con �l
				if (pickedObject_ != nullptr) {
					pickedObject_->pickObject();
				}
			}			
		}
		//Si el jugador lleva algo encima
		else {

			//Se busca el mueble m�s cercano de nuevo
			Mueble* m = nullptr;
			for (auto i : game->getObjectManager()->getMueblesCollider(getOverlapCollider())) {
				m = nearestObject(m, dynamic_cast<Mueble*>(i));
			}

			//Dependiendo de lo que lleve el jugador encima, la interacci�n con el mueble ser� distinta
			switch (objectType_)
			{
			case INGREDIENTE:
				if (m != nullptr && m->receiveIngrediente(dynamic_cast<Ingrediente*>(pickedObject_))) {
					pickedObject_->dropObject();
					pickedObject_ = nullptr;
				}
				break;
			case CLIENTES:
				if (m != nullptr && m->receiveGrupoClientes(dynamic_cast<GrupoClientes*>(pickedObject_))) {
					pickedObject_->dropObject();
					pickedObject_ = nullptr;
				}
				else {
					for (auto i : game->getObjectManager()->getPoolGrupoClientes()->getCollisions(getOverlapCollider())) {
						if (i == pickedObject_) {
							pickedObject_->setPicked(false);
							pickedObject_ = nullptr;
							return;
						}
					}
				}			
				break;
			case PAELLA:
				if (m != nullptr && m->receivePaella(dynamic_cast<Paella*>(pickedObject_))) {
					pickedObject_->dropObject();
					pickedObject_ = nullptr;
				}
				break;
			default:
				break;
			}
		}
	}
}

void Player::handleInput(Vector2D<double> axis)
{
	vel = (vel + axis * aceleracion);

	//Altura
	if (vel.getY() != 0 && axis.getY() == 0) {
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
	if (vel.getX() != 0 && axis.getX() == 0) {
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
}

bool Player::nearestObject(ObjetoPortable* go)
{
	if (pickedObject_ == nullptr) {
		pickedObject_ = go;
		return true;
	}	
	else
	{
		Vector2D<double> pos = getPosition();
		if ((pos - go->getPosition()).magnitude() < (pos - pickedObject_->getPosition()).magnitude()) {
			pickedObject_ = go;
			return true;
		}
		else return false;
	}
}

Mueble* Player::nearestObject(Mueble* m1, Mueble* m2)
{
	if (m1 == nullptr)
		return m2;
	else
	{
		Vector2D<double> pos = getPosition();
		if ((pos - m1->getPosition()).magnitude() < (pos - m2->getPosition()).magnitude()) {
			return m1;
		}
		else return m2;
	}
}

void Player::animUpdate()
{

}

void Player::setAnimResources()
{

}



void Player::update()
{
	Vector2D<double> newPos = pos + vel;

	SDL_Rect newRect = { newPos.getX(), newPos.getY(), getCollider().w, getCollider().h };

	if (game->getObjectManager()->getMueblesCollider(getCollider()).empty())
	{
		pos = pos + vel;
	}
	//else if (game->getObjectManager()->getMueblesCollider(getOverlapCollider()).empty())
	//{
	//	pos = pos + vel;
	//}
	//Este caso está para arreglar un bug de q el jugador se podía quedar atascado en las esquinas con la opción anterior
	else
	{
		bool movPos = true;

		Collider* col = nullptr;

		for (auto i : game->getObjectManager()->getMueblesCollider(getCollider()))
		{
			if (game->getObjectManager()->getMueblesCollider(getOverlapCollider()).size() > 0)
				movPos = movPos && i != game->getObjectManager()->getMueblesCollider(getOverlapCollider())[0];

			if (game->getObjectManager()->getMueblesCollider(getOverlapCollider()).size() > 1)
				movPos = movPos && i != game->getObjectManager()->getMueblesCollider(getOverlapCollider())[1];

			if (game->getObjectManager()->getMueblesCollider(getOverlapCollider()).size() > 2)
				movPos = movPos && i != game->getObjectManager()->getMueblesCollider(getOverlapCollider())[2];
		}

		if (movPos)
		{
			if (orientation_ == N || orientation_ == S)
			{
				vel.setX(0);
			}	
			else if (orientation_ == E || orientation_ == O)
			{
				vel.setY(0);
			}

			pos = pos + vel;
		}
	}

	if (pickedObject_ != nullptr) {
		if (pickedObject_->isPicked())
			pickedObject_->setPosition(getX(), getY() - getHeight() / 2);
		else
			pickedObject_ = nullptr;
	}

	if (ih().getAxisY() == 1) 
		orientation_ = S;		
	else if (ih().getAxisY() == -1)
		orientation_ = N;		

	if (ih().getAxisX() == 1)
		orientation_ = E;		
	else if (ih().getAxisX() == -1) 
		orientation_ = O;


	switch (orientation_)
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

	for (auto i : game->getObjectManager()->getMueblesCollider(getCollider())) {
		i->colisionPlayer(this);
	}
}

SDL_Rect Player::getOverlapCollider()
{
	return { int(overlapPos.getX()),
		 int(overlapPos.getY()),
		  (overlapDim.getX()),
		  (overlapDim.getY()) };
}

void Player::setVel(double x, double y)
{
	vel.set(x, y);
}

void Player::setVel(Vector2D<double> vel2)
{
	vel.set(vel2);
}

Vector2D<double> Player::getOrientation()
{
	Vector2D<double> orientation = { 0, 0 };

	switch (orientation_)
	{
	case E:
		orientation.setX(1);
		break;
	case O:
		orientation.setX(-1);
		break;
	case S:
		orientation.setY(1);
		break;
	case N:
		orientation.setY(-1);
		break;
	default:
		break;
	}

	return orientation;
}

Vector2D<double> Player::getVel()
{
	return vel;
}

void Player::renderDebug(SDL_Rect* cameraRect)
{
	drawDebug(cameraRect);
	drawDebug(cameraRect, getOverlapCollider());
}

void Player::setPickedObject(ObjetoPortable* op, objectType ot)
{
	pickedObject_ = op;
	objectType_ = ot;
}



