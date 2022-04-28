#include "Player.h"

#include "../sdlutils/InputHandler.h"
#include "../Control/Game.h"
#include "../Scenes/Scene.h"
#include "../Scenes/Tutorial.h"
#include "../Control/ObjectManager.h"

#include "Ingrediente.h"
#include "Herramienta.h"
#include "Muebles/Mueble.h"
#include "Muebles/Mesa.h"
#include "Muebles/FinalCinta.h"
#include "Arroz.h"

#include "../Utils/Traza.h"


Player::Player(Game* game, bool chef) : GameObject(game), objectType_(INGREDIENTE), pickedObject_(nullptr), chef_(chef),
overlapPos(Vector2D<double>(getX() - overlapPos.getX() / 2, getY() - getHeight() / 2 - overlapDim.getY())),
overlapDim(Vector2D<int>(50, 50))
{
	setPosition(200, 600);
	setDimension(150, 150);
	overlapDim.set(45, 45);

	aceleracion = 1.2;
	deceleracion = 0.8;
	maxVel = 7;

	setAnimResources();

	clip.x = 0;
	clip.y = 0;
	clip.w = 256;
	clip.h = 256;

	frameCounter = 0;
	lastFrameTime = sdlutils().currRealTime();
	frameRate = 1000 / 60;

	currAnim = 0;

	setTexture("player");

	setDepth(1);
}

Player::~Player()
{
}

void Player::handleInput(Vector2D<double> axis, bool playerOne)
{
	//El jugador se mueve o se para en ambos ejes
	if (!ih().getFocus())
	{
		if ((axis.getX() > .1f || axis.getX() < -.1f) && !enComanda)
			vel.setX(vel.getX() + axis.getX() * aceleracion);
		else
			vel.setX(vel.getX() * deceleracion);

		if ((axis.getY() > .1f || axis.getY() < -.1f) && !enComanda)
			vel.setY(vel.getY() + axis.getY() * aceleracion);

		else
			vel.setY(vel.getY() * deceleracion);

		vel.clamp(-maxVel, maxVel);
	}

	if (ih().getKey(InputHandler::A) && playerOne) {
		//Se prioriza la interaccion con los muebles por encima de otros objetos
		//Se prioriza el mueble mas cercano al jugador
		Mueble* m = nullptr;
		for (auto i : game->getObjectManager()->getMueblesOverlaps(getOverlap())) {
			m = nearestObject(m, i);
		}

		//Si el jugador no lleva nada encima
		if (pickedObject_ == nullptr) {

			//Si se ha encontrado un mueble, se intenta interactuar con 
			//este con returnObject(), para que te devuelva el objeto
			if (m != nullptr && m->returnObject(this)) {
				pickedObject_->pickObject();

				// Mandar mensaje de que he pillado un objeto
				if (objectType_ == PAELLA) {
					Paella* pa = dynamic_cast<Paella*>(pickedObject_);
					game->getNetworkManager()->syncPickObject(objectType_, pa->getId(), m->getId(), pa->getTipo());
				}
				else if (objectType_ == INGREDIENTE) {
					Ingrediente* ing = dynamic_cast<Ingrediente*>(pickedObject_);
					game->getNetworkManager()->syncPickObject(objectType_, ing->getId(), m->getId(), ing->esLetal());
				}
				else {
					game->getNetworkManager()->syncPickObject(objectType_, pickedObject_->getId(), m->getId(), 0);
				}

			}

			//En caso contrario se recorre el resto de objetos del juego para ver si el jugador puede cogerlos
			//Una vez m�s se prioriza el objeto m�s cercano
			else
			{
				//Ingredientes
				for (auto i : game->getObjectManager()->getPool<Ingrediente>(_p_INGREDIENTE)->getOverlaps(getOverlap())) {
					if (i->isActive() && i->canPick() && nearestObject(i)) {
						objectType_ = INGREDIENTE;
					}

					if (dynamic_cast<Tutorial*>(game->getCurrentScene()) && game->getCurrentScene()->getState() == States::cogerIngrediente) {
						game->getCurrentScene()->changeState(States::pausaCogerIngrediente);
					}
				}
				//Ingredientes letales
				for (auto i : game->getObjectManager()->getPool<Ingrediente>(_p_INGREDIENTELETAL)->getOverlaps(getOverlap())) {
					if (i->isActive() && i->canPick() && nearestObject(i)) {
						objectType_ = INGREDIENTE;
					}
				}
				//Grupo de Clientes
				for (auto i : game->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->getOverlaps(getOverlap())) {
					if (i->isActive() && i->canPick() && !i->isPicked() && nearestObject(i)) {
						objectType_ = CLIENTES;
						if (dynamic_cast<Tutorial*>(game->getCurrentScene()) && game->getCurrentScene()->getState() == States::cogerClientes) {
							game->getCurrentScene()->changeState(States::pausaClientes);
						}
					}
				}

				//Una vez encontrado el m�s cercano, se interact�a con �l
				if (pickedObject_ != nullptr) {
					pickedObject_->pickObject();

					// Mandar mensaje de que he pillado un objeto
					if (objectType_ == INGREDIENTE) {
						Ingrediente* ing = dynamic_cast<Ingrediente*>(pickedObject_);
						game->getNetworkManager()->syncPickObject(objectType_, ing->getId(), -1, ing->esLetal());
					}
					else if (objectType_ == CLIENTES) {
						game->getNetworkManager()->syncPickObject(objectType_, pickedObject_->getId(), -1, 0);
					}
				}
			}
		}
		//Si el jugador lleva algo encima
		else {

			//Dependiendo de lo que lleve el jugador encima, la interacci�n con el mueble ser� distinta
			switch (objectType_)
			{
			case INGREDIENTE:
				if (m != nullptr && m->receiveIngrediente(dynamic_cast<Ingrediente*>(pickedObject_))) {
					// Mandar mensaje drop 
					game->getNetworkManager()->syncDropObject(objectType_, pickedObject_->getId(), m->getId());

					pickedObject_->dropObject();
					pickedObject_ = nullptr;

				}
				break;
			case CLIENTES:
				if (m != nullptr && m->receiveGrupoClientes(dynamic_cast<GrupoClientes*>(pickedObject_))) {
					game->getNetworkManager()->syncDropObject(objectType_, pickedObject_->getId(), m->getId());

					pickedObject_->dropObject();
					pickedObject_ = nullptr;
				}
				else {
					for (auto i : game->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->getOverlaps(getOverlap())) {
						if (i == pickedObject_) {
							game->getNetworkManager()->syncDropObject(objectType_, pickedObject_->getId(), -1);

							pickedObject_->setPicked(false);
							pickedObject_ = nullptr;
							return;
						}
					}
				}
				break;
			case PAELLA:
				if (m != nullptr && m->receivePaella(dynamic_cast<Paella*>(pickedObject_))) {
					if (dynamic_cast<Tutorial*>(game->getCurrentScene())) {
						if (dynamic_cast<Mesa*>(m))
						{
							if (game->getCurrentScene()->getState() == States::pausaDarDeComer) {
								pickedObject_->dropObject();
								pickedObject_ = nullptr;
							}
						}
						else {
							pickedObject_->dropObject();
							pickedObject_ = nullptr;
						}
					}
					else {
						game->getNetworkManager()->syncDropObject(objectType_, pickedObject_->getId(), m->getId());

						if (m != dynamic_cast<FinalCinta*>(m)) {
							pickedObject_->dropObject();
							pickedObject_ = nullptr;
						}
					}
				}
				break;
			case ARROZ:
				if (m != nullptr && m->receiveArroz(dynamic_cast<Arroz*>(pickedObject_))) {
					game->getNetworkManager()->syncDropObject(objectType_, pickedObject_->getId(), m->getId());

					pickedObject_->dropObject();
					pickedObject_ = nullptr;
				}
				break;
			case HERRAMIENTA:
				if (m != nullptr && m->receiveHerramienta(dynamic_cast<Herramienta*>(pickedObject_))) {
					game->getNetworkManager()->syncDropObject(objectType_, pickedObject_->getId(), m->getId());

					pickedObject_->dropObject();
					pickedObject_ = nullptr;
				}
				break;
			default:
				break;
			}
		}
	}
	if (sdlutils().currRealTime() - lastFrameTime > frameRate)
		animUpdate(axis);
}

void Player::update()
{
	//Próxima posición
	SDL_Rect newCol = getCollider();
	newCol.x += vel.getX();
	newCol.y += vel.getY();

	SDL_Rect rect = getCollider();

	Vector2D<double> newPos = pos + vel;

	for (auto i : game->getObjectManager()->getMueblesCollisions(newCol)) {
		//Cuando colisiono con un mueble
		SDL_Rect c = i->getCollider();

		//Comprobamos Izquierda o Derecha
		double interseccionIz = abs((rect.x + rect.w) - (c.x));
		double interseccionDer = abs((rect.x) - (c.x + c.w));
		bool bIz = interseccionIz < interseccionDer;
		double interseccionX = bIz ? interseccionIz : interseccionDer;

		//Comprobamos Arriba o Abajo
		double interseccionAr = abs((rect.y + rect.h) - (c.y));
		double interseccionAb = abs((rect.y) - (c.y + c.h));
		bool bAr = interseccionAr < interseccionAb;
		double interseccionY = bAr ? interseccionAr : interseccionAb;

		//Combrobamos Horizontal o Vertical, y aplicamos el cambio
		if (interseccionX < interseccionY)
			newPos.setX(bIz ? c.x - rect.w / 2 : c.x + c.w + rect.w / 2);
		else
			newPos.setY(bAr ? c.y - getHeight() / 2 : c.y + c.h - rect.h / 2);
	}

	//Nos movemos al nuevo sitio
	setPosition(newPos);

	for (auto i : game->getObjectManager()->getMueblesOverlaps(getCollider())) {
		i->decirPedido();
	}

	if (vel.getY() > .8f)
		orientation_ = S;
	else if (vel.getY() < -.8f)
		orientation_ = N;

	if (vel.getX() > .8f)
		orientation_ = E;
	else if (vel.getX() < -.8f)
		orientation_ = O;

	switch (orientation_)
	{
	case E:
		overlapPos = Vector2D<double>(getX() + getWidth() / 4,
			getY());
		break;
	case O:
		overlapPos = Vector2D<double>(getX() - getWidth() / 4 - overlapDim.getX(),
			getY());
		break;
	case S:
		overlapPos = Vector2D<double>(getX() - overlapDim.getX() / 2,
			getY() + getHeight() / 3);
		break;
	case N:
		overlapPos = Vector2D<double>(getX() - overlapDim.getX() / 2,
			getY() - getHeight() / 4 - overlapDim.getY());
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
		Vector2D<double> pos = getRectCenter(getOverlap());

		if ((pos - getRectCenter(go->getOverlap())).magnitude()
			< (pos - getRectCenter(pickedObject_->getOverlap())).magnitude()) {

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
		Vector2D<double> pos = getRectCenter(getOverlap());

		if ((pos - getRectCenter(m1->getOverlap())).magnitude()
			< (pos - getRectCenter(m2->getOverlap())).magnitude()) {

			return m1;
		}
		else return m2;
	}
}

void Player::animUpdate(Vector2D<double> axis)
{
	//bool para saber si hay que cambiar el flip
	bool flipH = false;
	if (flip == SDL_FLIP_HORIZONTAL) flipH = true;

	lastFrameTime = sdlutils().currRealTime();

	clip.x = frameCounter * clip.w;
	frameCounter++;

	if (frameCounter * clip.w > anims[currAnim]->width() - 10)
		frameCounter = 0;

	switch (orientation_)
	{
	case N:
		currAnim = 2;
		break;
	case S:
		currAnim = 0;
		break;
	case E:
		currAnim = 1;
		if (flipH) flip = SDL_FLIP_NONE;
		break;
	case O:
		currAnim = 1;
		if (!flipH)flip = SDL_FLIP_HORIZONTAL;
		break;
	default:
		break;
	}

	if (axis.getY() > .1f && !enComanda) {
		// Andar Abajo
		currAnim = 3;
	}
	else if (axis.getY() < -.1f && !enComanda) {
		// Andar Arriba
		currAnim = 5;
	}

	// Horizontal va segundo para tener prioridad
	if (axis.getX() > .1f && !enComanda) {
		// Andar der
		currAnim = 4;
	}
	else if (axis.getX() < -.1f && !enComanda) {
		// Andar izq
		currAnim = 4;
	}
}

void Player::setAnimResources()
{
	if (chef_) {
		anims.push_back(&sdlutils().images().at("cocineraIdleDown"));
		anims.push_back(&sdlutils().images().at("cocineraIdleSide"));
		anims.push_back(&sdlutils().images().at("cocineraIdleUp"));

		anims.push_back(&sdlutils().images().at("cocineraWalkDown"));
		anims.push_back(&sdlutils().images().at("cocineraWalkSide"));
		anims.push_back(&sdlutils().images().at("cocineraWalkUp"));
	}

	else {
		anims.push_back(&sdlutils().images().at("camareroIdleDown"));
		anims.push_back(&sdlutils().images().at("camareroIdleSide"));
		anims.push_back(&sdlutils().images().at("camareroIdleUp"));

		anims.push_back(&sdlutils().images().at("camareroWalkDown"));
		anims.push_back(&sdlutils().images().at("camareroWalkSide"));
		anims.push_back(&sdlutils().images().at("camareroWalkUp"));
	}
}

SDL_Rect Player::getOverlap()
{
	return { int(overlapPos.getX()),
		 int(overlapPos.getY()),
		  (overlapDim.getX()),
		  (overlapDim.getY()) };
}

void Player::changePlayer(bool c)
{
	chef_ = c;
	anims.clear();
	if (chef_) {
		anims.push_back(&sdlutils().images().at("cocineraIdleDown"));
		anims.push_back(&sdlutils().images().at("cocineraIdleSide"));
		anims.push_back(&sdlutils().images().at("cocineraIdleUp"));

		anims.push_back(&sdlutils().images().at("cocineraWalkDown"));
		anims.push_back(&sdlutils().images().at("cocineraWalkSide"));
		anims.push_back(&sdlutils().images().at("cocineraWalkUp"));
	}

	else {
		anims.push_back(&sdlutils().images().at("camareroIdleDown"));
		anims.push_back(&sdlutils().images().at("camareroIdleSide"));
		anims.push_back(&sdlutils().images().at("camareroIdleUp"));

		anims.push_back(&sdlutils().images().at("camareroWalkDown"));
		anims.push_back(&sdlutils().images().at("camareroWalkSide"));
		anims.push_back(&sdlutils().images().at("camareroWalkUp"));
	}
}

void Player::PickCustomObject(int objectType, int objectId, int muebleId, int extraInfo)
{
	if (objectType == INGREDIENTE) {
		if (extraInfo == 0) {
			// comprobar si existe el ingrediente
			for (auto i : game->getObjectManager()->getPool<Ingrediente>(_p_INGREDIENTE)->getActiveObjects()) {
				if (i->getId() == objectId) {
					pickedObject_ = i;
					break;
				}
			}
		}
		else {
			// ingrediente letal
			if (pickedObject_ == nullptr) {
				for (auto i : game->getObjectManager()->getPool<IngredienteLetal>(_p_INGREDIENTELETAL)->getActiveObjects()) {
					if (i->getId() == objectId) {
						pickedObject_ = i;
						break;
					}
				}
			}
		}

		for (auto m : game->getObjectManager()->getMuebles()) {
			if (m->getId() == muebleId) {
				m->returnObject(this);
				break;
			}
		}
	}
	else if (objectType == CLIENTES)
	{
		for (auto i : game->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->getActiveObjects()) {
			if (i->getId() == objectId) {
				pickedObject_ = i;
				break;
			}
		}
	}

	else { // Es un mueble
		for (auto m : game->getObjectManager()->getMuebles()) {
			if (m->getId() == muebleId) {
				m->returnObject(this);
				break;
			}
		}
	}

	// pick object
	pickedObject_->pickObject();
}

void Player::DropCustomObject(int objectType, int objectId, int muebleId)
{
	Mueble* mueble = nullptr;
	for (auto i : game->getObjectManager()->getMuebles()) {
		if (i->getId() == muebleId) {
			mueble = i;
			break;
		}
	}

	if (mueble != nullptr) {
		if (objectType == INGREDIENTE) {
			// drop in the mueble
			mueble->receiveIngrediente(dynamic_cast<Ingrediente*>(pickedObject_));
		}
		else if (objectType == PAELLA) {
			mueble->receivePaella(dynamic_cast<Paella*>(pickedObject_));
		}
		else if (objectType == ARROZ) {
			mueble->receiveArroz(dynamic_cast<Arroz*>(pickedObject_));
		}
		else if (objectType == CLIENTES) {
			mueble->receiveGrupoClientes(dynamic_cast<GrupoClientes*>(pickedObject_));
		}

		if (mueble != dynamic_cast<FinalCinta*>(mueble)) {
			pickedObject_->dropObject();
			pickedObject_ = nullptr;
		}
	}
	else {
		for (auto g : game->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->getActiveObjects()) {
			if (g->getId() == objectId) {
				g->setPicked(false);
				pickedObject_ = nullptr;
			}
		}
	}
	
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
	drawDebug(cameraRect, getTexBox());
}

void Player::render(SDL_Rect* cameraRect)
{
	SDL_Rect dest = { getX() - getWidth() / 2, getY() - getHeight() / 2, w, h };
	drawRender(cameraRect, dest, anims[currAnim], clip, flip);
}

void Player::setPickedObject(ObjetoPortable* op, objectType ot)
{
	pickedObject_ = op;
	objectType_ = ot;
}

SDL_Rect Player::getCollider()
{
	SDL_Rect rect = getTexBox();

	return { rect.x + rect.w / 4,
		rect.y + rect.h / 3 * 2,
		rect.w / 2,
		rect.h / 3 };
}





