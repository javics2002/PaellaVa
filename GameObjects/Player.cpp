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

#include "../Data/Pedido.h"

#include "../Utils/Traza.h"
#include "CajaTakeaway.h"


Player::Player(Game* mGame,double x, double y, bool chef) : GameObject(mGame), objectType_(INGREDIENTE), pickedObject_(nullptr), chef_(chef),
overlapPos(Vector2D<double>(getX() - overlapPos.getX() / 2, getY() - getHeight() / 2 - overlapDim.getY())),
overlapDim(Vector2D<int>(50, 50))
{
	setPosition(x, y);
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
	
		if ((axis.getX() > .1f || axis.getX() < -.1f) && !enComanda)
			vel.setX(vel.getX() + axis.getX() * aceleracion);
		else
			vel.setX(vel.getX() * deceleracion);

		if ((axis.getY() > .1f || axis.getY() < -.1f) && !enComanda)
			vel.setY(vel.getY() + axis.getY() * aceleracion);

		else
			vel.setY(vel.getY() * deceleracion);

		vel.clamp(-maxVel, maxVel);
	
	

	if (ih().getKey(InputHandler::A) && playerOne) {
		//Se prioriza la interaccion con los muebles por encima de otros objetos
		//Se prioriza el mueble mas cercano al jugador
		Mueble* m = nullptr;
		for (auto i : mGame->getObjectManager()->getMueblesOverlaps(getOverlap())) {
			m = nearestObject(m, i);
		}

		//Si el jugador no lleva nada encima
		if (pickedObject_ == nullptr && pickedPaellas_.empty()) {

			//Si se ha encontrado un mueble, se intenta interactuar con 
			//este con returnObject(), para que te devuelva el objeto
			if (m != nullptr && m->returnObject(this)) {
				pickedObject_->pickObject();

				// Mandar mensaje de que he pillado un objeto
				if (objectType_ == PAELLA) {
					Paella* pa = dynamic_cast<Paella*>(pickedObject_);
					pickedPaellas_.push_back(pa);
					mGame->getNetworkManager()->syncPickObject(objectType_, pa->getId(), m->getId(), pa->getTipo());
				}
				else if (objectType_ == INGREDIENTE) {
					Ingrediente* ing = dynamic_cast<Ingrediente*>(pickedObject_);
					mGame->getNetworkManager()->syncPickObject(objectType_, ing->getId(), m->getId(), ing->esLetal());
				}
				else {
					mGame->getNetworkManager()->syncPickObject(objectType_, pickedObject_->getId(), m->getId(), 0);
				}

			}

			//En caso contrario se recorre el resto de objetos del juego para ver si el jugador puede cogerlos
			//Una vez m�s se prioriza el objeto m�s cercano
			else
			{
				//Ingredientes
				for (auto i : mGame->getObjectManager()->getPool<Ingrediente>(_p_INGREDIENTE)->getOverlaps(getOverlap())) {
					if (i->isActive() && !i->isPicked() && i->canPick() && nearestObject(i)) {
						objectType_ = INGREDIENTE;
					}

					if (dynamic_cast<Tutorial*>(mGame->getCurrentScene()) && mGame->getCurrentScene()->getState() == States::TUTORIALSTATE_COGER_INGREDIENTE) {
						mGame->getCurrentScene()->changeState(States::TUTORIALSTATE_PAUSA_COGER_INGREDIENTE);
					}
				}
				//Ingredientes letales
				for (auto i : mGame->getObjectManager()->getPool<Ingrediente>(_p_INGREDIENTELETAL)->getOverlaps(getOverlap())) {
					if (i->isActive() && !i->isPicked() && i->canPick() && nearestObject(i)) {
						objectType_ = INGREDIENTE;
					}
				}
				//Grupo de Clientes
				for (auto i : mGame->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->getOverlaps(getOverlap())) {
					if (i->isActive() && !i->isPicked() && i->canPick() && nearestObject(i)) {
						objectType_ = CLIENTES;
						if (dynamic_cast<Tutorial*>(mGame->getCurrentScene()) && mGame->getCurrentScene()->getState() == States::TUTORIALSTATE_COGER_CLIENTES) {
							mGame->getCurrentScene()->changeState(States::TUTORIALSTATE_PAUSA_COGER_CLIENTES);
						}
					}
				}

				//Una vez encontrado el m�s cercano, se interact�a con �l
				if (pickedObject_ != nullptr && pickedPaellas_.empty()) {
					pickedObject_->pickObject();

					// Mandar mensaje de que he pillado un objeto
					if (objectType_ == INGREDIENTE) {
						Ingrediente* ing = dynamic_cast<Ingrediente*>(pickedObject_);
						mGame->getNetworkManager()->syncPickObject(objectType_, ing->getId(), -1, ing->esLetal());
					}
					else if (objectType_ == CLIENTES) {
						mGame->getNetworkManager()->syncPickObject(objectType_, pickedObject_->getId(), -1, 0);
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
					mGame->getNetworkManager()->syncDropObject(objectType_, pickedObject_->getId(), m->getId());

					pickedObject_->dropObject();
					pickedObject_ = nullptr;

				}
				break;
			case CLIENTES:
				if (m != nullptr && m->receiveGrupoClientes(dynamic_cast<GrupoClientes*>(pickedObject_))) {
					mGame->getNetworkManager()->syncDropObject(objectType_, pickedObject_->getId(), m->getId());

					pickedObject_->dropObject();
					

					// mandar mensaje?
					GrupoClientes* gClientes = dynamic_cast<GrupoClientes*>(pickedObject_);

					if (gClientes->getPedido() != nullptr) {
						vector<int> tamPaellas;
						vector<int> ingPedidos = vector<int>(12, LAST);

						int cont = 0;

						for (int i = 0; i < gClientes->getPedido()->getPedido().size(); i++) {
							tamPaellas.push_back(gClientes->getPedido()->getPedido()[i].tamanoPaella);

							for (int j = 0; j < gClientes->getPedido()->getPedido()[i].ingredientesPedido.size(); j++) {
								ingPedidos[cont] = gClientes->getPedido()->getPedido()[i].ingredientesPedido[j];
								cont++;
							}
						}

						mGame->getNetworkManager()->syncPedido(pickedObject_->getId(), gClientes->getPedido()->getPedido().size(), tamPaellas, ingPedidos);

						pickedObject_ = nullptr;
					}
				}
				else {
					for (auto i : mGame->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->getOverlaps(getOverlap())) {
						if (i == pickedObject_) {
							mGame->getNetworkManager()->syncDropObject(objectType_, pickedObject_->getId(), -1);						
							pickedObject_->setPicked(false);
							i->setGoshtGroup();
							pickedObject_ = nullptr;
							return;
						}
					}
				}
				break;
			case PAELLA:

				// Coger otra Paella si la hay
				// Mira objeto en mueble
				// if (m != nullptr && m->returnObject(this) && m->hasPaella() != nullptr && pickedPaellas_.size() < MAX_PAELLAS_CARRY_) {
 				if (m != nullptr && m->returnObject(this) && pickedPaellas_.size() < MAX_PAELLAS_CARRY_) {
 					pickedObject_->pickObject();
					Paella* pa = dynamic_cast<Paella*>(pickedObject_);

					pickedPaellas_.push_back(pa);
					mGame->getNetworkManager()->syncPickObject(objectType_, pa->getId(), m->getId(), pa->getTipo());
				}
				else if (m != nullptr) {
					// Si no es basura ni cajaTakeaway
					// m->returnObject(this);
					if (m != dynamic_cast<FinalCinta*>(m) && m->hasCajaTakeaway() == nullptr) {
						if (m->receivePaella(pickedPaellas_.back())) {

							if (dynamic_cast<Tutorial*>(mGame->getCurrentScene())) {
								if (dynamic_cast<Mesa*>(m))
								{
									if (mGame->getCurrentScene()->getState() == States::TUTORIALSTATE_PAUSA_DAR_DE_COMER) {
										pickedPaellas_.back()->dropObject();
										pickedPaellas_.pop_back();
										pickedPaellas_.shrink_to_fit();

										if (pickedPaellas_.empty())
											pickedObject_ = nullptr;
										else {
											setPickedObject(pickedPaellas_.back(), PAELLA);
										}
									}
								}
								else {
									pickedPaellas_.back()->dropObject();
									pickedPaellas_.pop_back();
									pickedPaellas_.shrink_to_fit();

									if (pickedPaellas_.empty())
										pickedObject_ = nullptr;
									else {
										setPickedObject(pickedPaellas_.back(), PAELLA);
									}
								}
							}
							else {

								mGame->getNetworkManager()->syncDropObject(objectType_, pickedPaellas_.back()->getId(), m->getId());
								pickedPaellas_.back()->dropObject();
								pickedPaellas_.pop_back();
								pickedPaellas_.shrink_to_fit();

								if (pickedPaellas_.empty())
									pickedObject_ = nullptr;
								else {
									setPickedObject(pickedPaellas_.back(), PAELLA);
								}
							}
						}
					}
					// Tirar a la basura
					else if (m == dynamic_cast<FinalCinta*>(m)) {

						int i = MAX_PAELLAS_CARRY_ - 1;
						while (i >= pickedPaellas_.size() || (i > 0 && (pickedPaellas_[i]->getContenido() == Limpia || pickedPaellas_[i]->getContenido() == Sucia))) {
							i--;
						}

						Paella* pa = pickedPaellas_[i];
						// Tenemos ultima paella llena o primera paella de la pila
						if (pa != pickedPaellas_.front() && m->receivePaella(pa))	mGame->getNetworkManager()->syncDropObject(objectType_, pa->getId(), m->getId());
						else if (pa->getContenido() != Sucia && pa->getContenido() != Limpia && m->receivePaella(pa))	mGame->getNetworkManager()->syncDropObject(objectType_, pa->getId(), m->getId());
					}

					// Meter en cajaTakeaway
					else if (m->hasCajaTakeaway() != nullptr) {

						int i = MAX_PAELLAS_CARRY_ - 1;
						while (i >= pickedPaellas_.size() || (i > 0 && pickedPaellas_[i]->getContenido() != Entera && pickedPaellas_[i]->getState() != Hecha)) {
							i--;
						}

						Paella* pa = pickedPaellas_[i];
						// Tenemos ultima paella llena o primera paella de la pila
						if (pa != pickedPaellas_.front() && m->receivePaella(pa))	mGame->getNetworkManager()->syncDropObject(objectType_, pa->getId(), m->getId());
						else if (pa->getContenido() != Sucia && pa->getContenido() != Limpia && m->receivePaella(pa))	mGame->getNetworkManager()->syncDropObject(objectType_, pa->getId(), m->getId());

					}

				}
				break;
			case ARROZ:
				if (m != nullptr && m->receiveArroz(dynamic_cast<Arroz*>(pickedObject_))) {
					mGame->getNetworkManager()->syncDropObject(objectType_, pickedObject_->getId(), m->getId());

					pickedObject_->dropObject();
					pickedObject_ = nullptr;
				}
				break;
			case HERRAMIENTA:
				if (m != nullptr && m->receiveHerramienta(dynamic_cast<Herramienta*>(pickedObject_))) {
					mGame->getNetworkManager()->syncDropObject(objectType_, pickedObject_->getId(), m->getId());

					pickedObject_->dropObject();
					pickedObject_ = nullptr;
				}
				break;
			case CAJATAKEAWAY:
				if (m != nullptr && m->receiveCajaTakeaway(dynamic_cast<CajaTakeaway*>(pickedObject_))) {
					mGame->getNetworkManager()->syncDropObject(objectType_, pickedObject_->getId(), m->getId());

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
	{
		
		animUpdate(axis);
	}
}

void Player::update()
{
	//Próxima posición
	SDL_Rect newCol = getCollider();
	newCol.x += vel.getX();
	newCol.y += vel.getY();

	SDL_Rect rect = getCollider();

	Vector2D<double> newPos = pos + vel;

	for (auto i : mGame->getObjectManager()->getMueblesCollisions(newCol)) {
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

	for (auto i : mGame->getObjectManager()->getMueblesOverlaps(getCollider())) {
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
			pickedObject_->setPosition(getX(), getY() - getHeight() / 5);
		else
			pickedObject_ = nullptr;
	}

	if (!pickedPaellas_.empty()) {
		for (int i = 0; i < pickedPaellas_.size(); i++) {
			pickedPaellas_[i]->setPosition(getX(), getY() - getHeight() / 5 - i * 15);
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
	lastFrameTime = sdlutils().currRealTime();

	clip.x = frameCounter * clip.w;
	frameCounter++;

	if (frameCounter * clip.w > anims[currAnim]->width() - 10)
		frameCounter = 0;

	
		switch (orientation_)
		{
		case N:
			if (pickedObject_ != nullptr || !pickedPaellas_.empty())
				currAnim = 8;
			else 
				currAnim = 2;
			break;

		case S:
			if (pickedObject_ != nullptr || !pickedPaellas_.empty())
				currAnim = 6;
			else
				currAnim = 0;
			break;

		case E:
			if (pickedObject_ != nullptr || !pickedPaellas_.empty())
				currAnim = 7;
			else
				currAnim = 1;
			flip = SDL_FLIP_NONE;
			break;

		case O:
			if (pickedObject_ != nullptr || !pickedPaellas_.empty())
				currAnim = 7;
			else
				currAnim = 1;
			flip = SDL_FLIP_HORIZONTAL;
			break;

		default:
			break;
		}
		
		

			if (axis.getY() > .1f && !enComanda) {
				// Andar Abajo
				if (pickedObject_ != nullptr || !pickedPaellas_.empty())
					currAnim = 9;
				else
					currAnim = 3;
			}
			else if (axis.getY() < -.1f && !enComanda) {
				// Andar Arriba
				if (pickedObject_ != nullptr || !pickedPaellas_.empty())
					currAnim = 11;
				else
					currAnim = 5;
			}

			// Horizontal va segundo para tener prioridad
			if (axis.getX() > .1f && !enComanda) {
				// Andar der
				if (pickedObject_ != nullptr || !pickedPaellas_.empty())
					currAnim = 10;
				else
					currAnim = 4;
			}
			else if (axis.getX() < -.1f && !enComanda) {
				// Andar izq
				if (pickedObject_ != nullptr || !pickedPaellas_.empty())
					currAnim = 10;
				else
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

		anims.push_back(&sdlutils().images().at("cocineraIdleDownCarry"));
		anims.push_back(&sdlutils().images().at("cocineraIdleSideCarry"));
		anims.push_back(&sdlutils().images().at("cocineraIdleUpCarry"));

		anims.push_back(&sdlutils().images().at("cocineraWalkDownCarry"));
		anims.push_back(&sdlutils().images().at("cocineraWalkSideCarry"));
		anims.push_back(&sdlutils().images().at("cocineraWalkUpCarry"));
	}

	else {
		anims.push_back(&sdlutils().images().at("camareroIdleDown"));
		anims.push_back(&sdlutils().images().at("camareroIdleSide"));
		anims.push_back(&sdlutils().images().at("camareroIdleUp"));

		anims.push_back(&sdlutils().images().at("camareroWalkDown"));
		anims.push_back(&sdlutils().images().at("camareroWalkSide"));
		anims.push_back(&sdlutils().images().at("camareroWalkUp"));

		anims.push_back(&sdlutils().images().at("camareroIdleDownCarry"));
		anims.push_back(&sdlutils().images().at("camareroIdleSideCarry"));
		anims.push_back(&sdlutils().images().at("camareroIdleUpCarry"));

		anims.push_back(&sdlutils().images().at("camareroWalkDownCarry"));
		anims.push_back(&sdlutils().images().at("camareroWalkSideCarry"));
		anims.push_back(&sdlutils().images().at("camareroWalkUpCarry"));
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

		anims.push_back(&sdlutils().images().at("cocineraIdleDownCarry"));
		anims.push_back(&sdlutils().images().at("cocineraIdleSideCarry"));
		anims.push_back(&sdlutils().images().at("cocineraIdleUpCarry"));

		anims.push_back(&sdlutils().images().at("cocineraWalkDownCarry"));
		anims.push_back(&sdlutils().images().at("cocineraWalkSideCarry"));
		anims.push_back(&sdlutils().images().at("cocineraWalkUpCarry"));
	}

	else {
		anims.push_back(&sdlutils().images().at("camareroIdleDown"));
		anims.push_back(&sdlutils().images().at("camareroIdleSide"));
		anims.push_back(&sdlutils().images().at("camareroIdleUp"));

		anims.push_back(&sdlutils().images().at("camareroWalkDown"));
		anims.push_back(&sdlutils().images().at("camareroWalkSide"));
		anims.push_back(&sdlutils().images().at("camareroWalkUp"));

		anims.push_back(&sdlutils().images().at("camareroIdleDownCarry"));
		anims.push_back(&sdlutils().images().at("camareroIdleSideCarry"));
		anims.push_back(&sdlutils().images().at("camareroIdleUpCarry"));

		anims.push_back(&sdlutils().images().at("camareroWalkDownCarry"));
		anims.push_back(&sdlutils().images().at("camareroWalkSideCarry"));
		anims.push_back(&sdlutils().images().at("camareroWalkUpCarry"));
	}
}

void Player::PickCustomObject(int objectType, int objectId, int muebleId, int extraInfo)
{
	if (objectType == INGREDIENTE) {
		if (extraInfo == 0) {
			// comprobar si existe el ingrediente
			for (auto i : mGame->getObjectManager()->getPool<Ingrediente>(_p_INGREDIENTE)->getActiveObjects()) {
				if (i->getId() == objectId) {
					pickedObject_ = i;
					break;
				}
			}
		}
		else {
			// ingrediente letal
			if (pickedObject_ == nullptr) {
				for (auto i : mGame->getObjectManager()->getPool<IngredienteLetal>(_p_INGREDIENTELETAL)->getActiveObjects()) {
					if (i->getId() == objectId) {
						pickedObject_ = i;
						break;
					}
				}
			}
		}

		for (auto m : mGame->getObjectManager()->getMuebles()) {
			if (m->getId() == muebleId) {
				m->returnObject(this);
				break;
			}
		}
	}
	else if (objectType == CLIENTES)
	{
		for (auto i : mGame->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->getActiveObjects()) {
			if (i->getId() == objectId) {
				pickedObject_ = i;
				break;
			}
		}
	}
	else if (objectType == HERRAMIENTA) {
		// comprobar si existe la herramienta
		for (auto i : mGame->getObjectManager()->getPool<Ingrediente>(_p_HERRAMIENTA)->getActiveObjects()) {
			if (i->getId() == objectId) {
				pickedObject_ = i;
				break;
			}
		}

		for (auto m : mGame->getObjectManager()->getMuebles()) {
			if (m->getId() == muebleId) {
				m->returnObject(this);
				break;
			}
		}
	}

	else { // Es un mueble
		for (auto m : mGame->getObjectManager()->getMuebles()) {
			if (m->getId() == muebleId) {
				m->returnObject(this);
				break;
			}
		}
	}

	// pick object
	if(pickedObject_ != nullptr)
		pickedObject_->pickObject();
}

void Player::DropCustomObject(int objectType, int objectId, int muebleId)
{
	Mueble* mueble = nullptr;
	for (auto i : mGame->getObjectManager()->getMuebles()) {
		if (i->getId() == muebleId) {
			mueble = i;
			break;
		}
	}

	if (mueble != nullptr && pickedObject_ != nullptr) {
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
		else if (objectType == HERRAMIENTA) {
			mueble->receiveHerramienta(dynamic_cast<Herramienta*>(pickedObject_));
		}

		if (mueble != dynamic_cast<FinalCinta*>(mueble)) {
			pickedObject_->dropObject();
			pickedObject_ = nullptr;
		}
	}
	else {
		for (auto g : mGame->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->getActiveObjects()) {
			if (g->getId() == objectId) {			
				g->setPicked(false);
				g->setGoshtGroup();
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
	//drawRender(cameraRect, 128);
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





