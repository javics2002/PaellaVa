#include "Player.h"

#include "../sdlutils/InputHandler.h"
#include "../Control/Game.h"
#include "../Control/ObjectManager.h"

#include "Ingrediente.h"
#include "Muebles/Mueble.h"
#include "Arroz.h"

#include "../Utils/Traza.h"


Player::Player(Game* game) : GameObject(game), objectType_(INGREDIENTE), pickedObject_(nullptr),
	overlapPos(Vector2D<double>(getX() - overlapPos.getX() / 2, getY() - getHeight() / 2 - overlapDim.getY())), overlapDim(Vector2D<int>(50, 50))
{
	setPosition(200, 600);
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

	setColliderRect({ (int)getX(), (int)getY(), w, h });

	//El jugador se mueve o se para en ambos ejes
	if (abs(ih().getAxisX()) > .1f)
		vel.setX(vel.getX() + ih().getAxisX() * aceleracion);
	else
		vel.setX(vel.getX() * deceleracion);

	if (abs(ih().getAxisY()) > .1f)
		vel.setY(vel.getY() + ih().getAxisY() * aceleracion);
	else
		vel.setY(vel.getY() * deceleracion);

	vel.clamp(-maxVel, maxVel);


	if (ih().getKey(InputHandler::INTERACT) && SDL_GetTicks() - lastTime_ > 500) {
		lastTime_ = SDL_GetTicks();

		//Si el jugador no lleva nada encima
		if (pickedObject_ == nullptr) {

			//Se prioriza la interaccion con los muebles por encima de otros objetos
			//Se prioriza el mueble mas cercano al jugador
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

			//Se busca el mueble mas cercano de nuevo
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
			case ARROZ:
				if (m != nullptr && m->receiveArroz(dynamic_cast<Arroz*>(pickedObject_))) {
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
	//El jugador se mueve o se para en ambos ejes
	if (abs(axis.getX()) > .1f)
		vel.setX(vel.getX() + axis.getX() * aceleracion);
	else
		vel.setX(vel.getX() * deceleracion);

	if (abs(axis.getY()) > .1f)
		vel.setY(vel.getY() + axis.getY() * aceleracion);
	else
		vel.setY(vel.getY() * deceleracion);

	vel.clamp(-maxVel, maxVel);
}

void Player::update()
{
	//Próxima posición
	Vector2D<double> newPos = pos + vel;
	SDL_Rect newRect = { newPos.getX() - getCollider().w / 2, newPos.getY() - getCollider().h / 2, getCollider().w, getCollider().h };
	
	auto colisionMuebles = game->getObjectManager()->getMueblesCollider(newRect);
	for (auto i : colisionMuebles) {
		//Si colisionamos con un mueble, le avisaremos y alejaremos al jugador
		i->colisionPlayer(this);

		//Cuanto estoy metido en el mueble?
		SDL_Rect c = i->getCollider();

		//Comprobamos por la izquierda y la derecha
		/*La intersección que busco es mas pequeña desde la izquierda y la derecha para
		que nos reposicione donde acabamos de entrar al mueble*/
		int interseccionIz = (newPos.getX() + getCollider().w / 2) - c.x;
		int interseccionDer = (newPos.getX() - getCollider().w / 2) - (c.x + c.w);
		int interseccionX = abs(interseccionIz) < abs(interseccionDer) ? interseccionIz : interseccionDer;
		
		//Lo mismo por arriba y por abajo
		int interseccionAr = (newPos.getY() + getCollider().h / 2) - c.y;
		int interseccionAb = (newPos.getY() - getCollider().h / 2) - (c.y + c.h);
		int interseccionY = abs(interseccionAr) < abs(interseccionAb) ? interseccionAr : interseccionAb;
		
		//Aplicamos la menor interseccion, que es la que tiene
		if (abs(interseccionX) < abs(interseccionY))
			newPos = newPos - Vector2D<double>(interseccionX, 0);
		else
			newPos = newPos - Vector2D<double>(0, interseccionY);
			
	}

	//Nos movemos al nuevo sitio
	pos = newPos;

	if (vel.getY() > .7f)
		orientation_ = S;
	else if (vel.getY() < -.7f)
		orientation_ = N;

	if (vel.getX() > .7f)
		orientation_ = E;
	else if (vel.getX() < -.7f)
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

	if (pickedObject_ != nullptr) {
		if (pickedObject_->isPicked())
			pickedObject_->setPosition(getX(), getY() - getHeight() / 2);
		else
			pickedObject_ = nullptr;
	}
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
	//drawDebug(cameraRect);
	drawDebugColl(cameraRect);
	drawDebug(cameraRect, getOverlapCollider());
}

void Player::setPickedObject(ObjetoPortable* op, objectType ot)
{
	pickedObject_ = op;
	objectType_ = ot;
}



