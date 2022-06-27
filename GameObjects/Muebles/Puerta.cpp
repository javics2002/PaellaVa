#include "Puerta.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"
#include "../../GameObjects/UI/UIManager.h"
#include "../../Control/NetworkManager.h"
#include "../../Scenes/Tutorial.h"

Puerta::Puerta(Game* mGame, Vector2D<double> pos, int t_Max, int tamMaxGrupo_) : Mueble(mGame, pos, TILE_SIZE, 2 * TILE_SIZE, "puerta")
{
	cola = new Cola(t_Max);
	colaTakeaway = new ColaTakeaway(t_Max);
	maxTamGrupo = tamMaxGrupo_;

	//Siempre tiene que funcionar
	funcionando = true;

	orientacion = E;

	initTime = 0;
	
	spawn_delay = 0;
	repartSpawn_delay = 0;
}

void Puerta::update()
{
	if (!mGame->getNetworkManager()->isHost())
		return;

	if (dynamic_cast<Tutorial*>(mGame->getCurrentScene())) {

		if (sdlutils().virtualTimer().currTime() - initTime >= spawn_delay && isActive()
			&& cola->esValido(1) && mGame->getCurrentScene()->getState() == States::TUTORIALSTATE_COGER_CLIENTES) {
			vector<Cliente*> clientes;
			
			clientSpawn();

			initTime = sdlutils().virtualTimer().currTime();
		}	
	}
	
	else {
		if (sdlutils().virtualTimer().currTime() - initTime >= spawn_delay) {


			if (clientSpawn())
				spawn_delay = MIN_SPAWN_TIME + sdlutils().rand().nextInt() % (MAX_SPAWN_TIME - MIN_SPAWN_TIME);
			else
				spawn_delay = FAIL_SPAWN_TIME;

			initTime = sdlutils().virtualTimer().currTime();



			cout << spawn_delay << endl;
		}

		if (sdlutils().virtualTimer().currTime() - initTime >= repartSpawn_delay) {


			if (repartidorSpawn())
				repartSpawn_delay = MIN_REPARTSPAWN_TIME + sdlutils().rand().nextInt() % (MAX_REPARTSPAWN_TIME - MIN_REPARTSPAWN_TIME);
			else
				repartSpawn_delay = FAIL_REPARTSPAWN_TIME;

			initTime = sdlutils().virtualTimer().currTime();



			cout << repartSpawn_delay << endl;
		}


		for (auto i : mGame->getObjectManager()->getPool<Repartidor>(_p_REPARTIDOR)->getCollisions(getCollider())) {
			if (i->getState() == repSALIENDO) {
				i->setActive(false);
			}
		}

	}
}

int Puerta::numClientSpawn() {
	int n = sdlutils().rand().nextInt() % 20;

	if (n < 2)
		return 1;
	else if (n < 7)
		return 2;
	else if (n < 12)
		return 3;
	else if (n < 16)
		return 4;
	else if (n < 18)
		return 5;
	else if (n < 20)
		return 6;
}

bool Puerta::repartidorSpawn()
{
	if (colaTakeaway->esValido()) {

		Repartidor* rep = mGame->getObjectManager()->getPool<Repartidor>(_p_REPARTIDOR)->add();

		Vector2D<double> pos = getPosition();

		Vector2D<double> dist = vel;
		dist.normalize();
		dist = Vector2D<double>(dist.getX() * rep->getCollider().w, dist.getY() * rep->getCollider().h);

		rep->setPosition(getX() - 50, getY() - 50);
		// rep->setAnimResources(t);
		setOrientation(rep);
		rep->setVel(vel);
		colaTakeaway->add(rep);
		rep->init(colaTakeaway);


		sdlutils().soundEffects().at("puerta").play();

		mGame->getNetworkManager()->sendRepartidor(getId(), rep->getId(), vel, dist, 100);

		return true;
	}

	return false;
}

bool Puerta::clientSpawn()
{
	int integrantes = numClientSpawn();

	if (cola->esValido(integrantes)) {

		vector<Cliente*> clientes;
		vector<int> texturas;

		Cliente* c = mGame->getObjectManager()->getPool<Cliente>(_p_CLIENTE)->add();

		Vector2D<double> dist = vel;
		dist.normalize();
		dist = Vector2D<double>(dist.getX() * c->getCollider().w, dist.getY() * c->getCollider().h);

		Vector2D<double> pos = getPosition();

		int t = sdlutils().rand().nextInt(0, 3);
		c->setPosition(getX(), getY());
		c->setAnimResources(t);
		clientes.push_back(c);
		texturas.push_back(t);
		setOrientation(c);

		for (int i = 1; i < integrantes; i++) {
			pos = pos - dist;

			c = mGame->getObjectManager()->getPool<Cliente>(_p_CLIENTE)->add();

			t = sdlutils().rand().nextInt(0, 3);
			c->setPosition(pos);
			c->setAnimResources(t);
			clientes.push_back(c);
			texturas.push_back(t);
			setOrientation(c);
		}

		GrupoClientes* g = mGame->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->add();
		g->setVel(vel);
		cola->add(g, integrantes);
		g->initGrupo(cola, clientes);


		sdlutils().soundEffects().at("puerta").play();

		mGame->getNetworkManager()->sendGrupoCliente(integrantes, getId(), g->getId(), vel, dist, texturas, 100);

		return true;
	}

	return false;
}

bool Puerta::receiveGrupoClientes(GrupoClientes* gc)
{
	if (dynamic_cast<Tutorial*>(mGame->getCurrentScene()) && mGame->getCurrentScene()->getState() == TUTORIALSTATE_ECHAR_CLIENTES) {
		mGame->getCurrentScene()->changeState(TUTORIALSTATE_PAUSA_ECHAR_CLIENTES);
	}
	else if (dynamic_cast<Tutorial*>(mGame->getCurrentScene())) {
		mGame->getCurrentScene()->changeState(TUTORIALSTATE_PAUSA_NO_ECHAR_CLIENTES);
		return false;
	}
	if (gc->getState() != CUENTA) {
		dynamic_cast<Jornada*>(mGame->getCurrentScene())->addPuntuaciones(0);
	}
	sdlutils().soundEffects().at("echarComensales").play(0, mGame->UI);
	gc->setActive(false);

	return true;
}

SDL_Rect Puerta::getCollider()
{
	if(isActive())return getTexBox();
	return { 0,0,0,0 };
}

SDL_Rect Puerta::getOverlap()
{
	if (isActive())return getTexBox();
	return { 0,0,0,0 };
}

Cola* Puerta::getCola() {
	return cola;
}

ColaTakeaway* Puerta::getColaTakeaway()
{
	return colaTakeaway;
}

void Puerta::setVel(Vector2D<double> vel_)
{
	vel = vel_;

	if (vel_.getX() < 0)
		orientacion = O;
	else if (vel_.getY() > 0)
		orientacion = S;
}

void Puerta::setOrientation(Cliente* c)
{
	switch (orientacion)
	{
	case E:
		c->setAnim(0);
		break;
	case O:
		c->setAnim(0);
		c->setFlip(SDL_FLIP_HORIZONTAL);
		break;
	case S:
		c->setAnim(5);
	default:
		break;
	}
}

void Puerta::setOrientation(Repartidor* rep)
{
	// Para anims
}

