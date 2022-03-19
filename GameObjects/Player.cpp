#include "Player.h"
#include "../Control/Input.h"
#include "../Control/Game.h"
#include "../sdlutils/InputHandler.h"
#include "../Control/ObjectManager.h"
#include "Ingrediente.h"
#include "Cliente.h"
#include "Muebles/Mueble.h"
#include "../Utils/Traza.h"


Player::Player(Game* game) : GameObject(game), objectType_(INGREDIENTE), pickedObject_(nullptr),
	overlapPos(Vector2D<double>(getX() - overlapPos.getX() / 2, getY() - getHeight() / 2 - overlapDim.getY())), overlapDim(Vector2D<int>(50, 50))
{

	setPosition(100, 100);
	setDimension(64, 64);


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
	


	if (ih().isKeyboardEvent() && ih().getKey(InputHandler::INTERACT) && SDL_GetTicks() - lastTime_ > 500) {
		lastTime_ = SDL_GetTicks();

		if (pickedObject_ == nullptr) {
			for (auto i : game->getObjectManager()->getPoolIngredientes()->getCollisions(getOverlapCollider())) {
				ObjetoPortable* op = dynamic_cast<ObjetoPortable*>(i);
				if (op->canPick() && nearestObject(op))
					objectType_ = INGREDIENTE;
			}
			for (auto i : game->getObjectManager()->getPoolGrupoClientes()->getCollisions(getOverlapCollider())) {
				ObjetoPortable* op = dynamic_cast<ObjetoPortable*>(i);
				if (op->canPick() && nearestObject(op))
					objectType_ = CLIENTES;
			}

			if (pickedObject_ != nullptr) {
				pickedObject_->pickObject();
			}
		}
		else {
			switch (objectType_)
			{
			case INGREDIENTE:
				for (auto i : game->getObjectManager()->getMueblesCollider(getOverlapCollider())) {
					
				}
				break;
			case CLIENTES:
				for (auto i : game->getObjectManager()->getMueblesCollider(getOverlapCollider())) {
					if (i->recieveGrupoClientes(dynamic_cast<GrupoClientes*>(pickedObject_))) {
						pickedObject_->dropObject();
						pickedObject_ = nullptr;
						return;
					}
				}
				break;
			case PAELLA:
				for (auto i : game->getObjectManager()->getMueblesCollider(getOverlapCollider())) {

				}
				break;
			default:
				break;
			}
		}
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

void Player::update()
{
	pos = pos + vel;

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
}

SDL_Rect Player::getOverlapCollider()
{
	return { int(overlapPos.getX()),
		 int(overlapPos.getY()),
		  (overlapDim.getX()),
		  (overlapDim.getY()) };
}

void Player::renderDebug(SDL_Rect* cameraRect)
{
	drawDebug(cameraRect);
	drawDebug(cameraRect, getOverlapCollider());
}

