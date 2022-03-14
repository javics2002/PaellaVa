#include "Player.h"
#include "../Control/Input.h"
#include "../Control/Game.h"
#include "../sdlutils/InputHandler.h"
#include "../Control/ObjectManager.h"
#include "Ingrediente.h"
#include "Cliente.h"
#include "../Utils/Traza.h"


Player::Player(Game* game) : GameObject(game), objectType_(INGREDIENTE), pickedObject_(nullptr),
	overlapPos(Vector2D<double>(getX() - overlapPos.getX() / 2, getY() - getHeight() / 2 - overlapDim.getY())), overlapDim(Vector2D<int>(50, 50))
{

	setPosition(100, 100);
	setDimension(64, 64);


	aceleracion = 1.2;
	deceleracion = 0.8;
	maxVel = 7;

	myIng = nullptr;
	
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
	

	//Todos los dynamic cast se puden evitar con métodos comunes en pool o con una nueva clase
	if (ih().isKeyboardEvent() && ih().getKeyPressed() == SDL_SCANCODE_E) {
		if (pickedObject_ == nullptr) {
			for (auto i : game->getObjectManager()->getPoolIngredientes()->getCollisions(getOverlapCollider())) {
				if (nearestObject(dynamic_cast<GameObject*>(i)))
					objectType_ = INGREDIENTE;
			}
			for (auto i : game->getObjectManager()->getPoolGrupoClientes()->getCollisions(getOverlapCollider())) {
				if (nearestObject(dynamic_cast<GameObject*>(i))) 
					objectType_ = CLIENTES;					
			}

			switch (objectType_)
			{
			case INGREDIENTE:
				dynamic_cast<Ingrediente*>(pickedObject_)->ingredienteRecogido();			
				break;
			case CLIENTES:
				if (dynamic_cast<GrupoClientes*>(pickedObject_)->getState() == estado::ENCOLA)
					dynamic_cast<GrupoClientes*>(pickedObject_)->setState(estado::COGIDO);
				else pickedObject_ = nullptr;
				break;
			case PAELLA:
				break;
			default:
				break;
			}
		}
		else pickedObject_ = nullptr;
	}
}

bool Player::nearestObject(GameObject* go)
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
		if (dynamic_cast<PoolObject*>(pickedObject_)->isActive())
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

void Player::debugOverlap(SDL_Rect* rect)
{
	SDL_Rect collider = getOverlapCollider();
	collider = { collider.x - rect->x, collider.y - rect->y, collider.w, collider.h };

	SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 255, 0, 0);
	SDL_RenderDrawRect(sdlutils().renderer(), &collider);
	SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 255, 255, 0);	
}

void Player::drawDebug(SDL_Rect* rect)
{
	debugOverlap(rect);

	SDL_Rect collider = getCollider();
	SDL_Rect center = getCenter();
	collider = { collider.x - rect->x, collider.y - rect->y, collider.w, collider.h };
	center = { center.x - rect->x, center.y - rect->y, center.w, center.h };
	
	SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 0, 0, 0);
	SDL_RenderDrawRect(sdlutils().renderer(), &collider);
	SDL_SetRenderDrawColor(sdlutils().renderer(), 0, 0, 255, 0);
	SDL_RenderFillRect(sdlutils().renderer(), &center);
	SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 255, 255, 0);
}

