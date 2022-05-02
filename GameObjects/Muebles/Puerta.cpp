#include "Puerta.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"
#include "../../GameObjects/UI/UIManager.h"
#include "../../Control/NetworkManager.h"
#include "../../Scenes/Tutorial.h"

Puerta::Puerta(Game* game, Vector2D<double> pos, int t_Max, int tamMaxGrupo_) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "puerta")
{
	cola = new Cola(t_Max);
	maxTamGrupo = tamMaxGrupo_;

	cout << maxTamGrupo << endl;

	//Siempre tiene que funcionar
	funcionando = true;

	orientacion = E;

	initTime = sdlutils().virtualTimer().currTime();

	cout << initTime << endl;
	
	spawn_delay = 0;
}

void Puerta::update()
{
	if (!game->getNetworkManager()->isHost())
		return;

	if (dynamic_cast<Tutorial*>(game->getCurrentScene())) {

		if (sdlutils().virtualTimer().currTime() - initTime >= spawn_delay && isActive()
			&& cola->esValido(1) && game->getCurrentScene()->getState() == States::cogerClientes) {
			vector<Cliente*> clientes;
			Cliente* c = game->getObjectManager()->getPool<Cliente>(_p_CLIENTE)->add();

			Vector2D<double> dist = vel;
			dist.normalize();
			dist = Vector2D<double>(dist.getX() * c->getWidth(), dist.getY() * c->getHeight());

			Vector2D<double> pos = getPosition();

			int t = sdlutils().rand().nextInt(0, 3);
			c->setPosition(getX(), getY());
			c->setAnimResources(t);
			clientes.push_back(c);

			pos = pos - dist;

			GrupoClientes* g = game->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->add();
			g->setVel(vel);
			cola->add(g, 1);
			g->initGrupo(cola, clientes);


			sdlutils().soundEffects().at("puerta").play();

			initTime = sdlutils().virtualTimer().currTime();
		}	
	}

	
	
	else if (sdlutils().virtualTimer().currTime() - initTime >= spawn_delay) {
		

		if (clientSpawn())
			spawn_delay = MIN_SPAWN_TIME + sdlutils().rand().nextInt() % (MAX_SPAWN_TIME - MIN_SPAWN_TIME);
		else
			spawn_delay = FAIL_SPAWN_TIME;
		
		initTime = sdlutils().virtualTimer().currTime();

		

		cout << spawn_delay << endl;
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

bool Puerta::clientSpawn()
{
	int integrantes = numClientSpawn();

	if (cola->esValido(integrantes)) {

		vector<Cliente*> clientes;
		vector<int> texturas;

		Cliente* c = game->getObjectManager()->getPool<Cliente>(_p_CLIENTE)->add();

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

			c = game->getObjectManager()->getPool<Cliente>(_p_CLIENTE)->add();

			t = sdlutils().rand().nextInt(0, 3);
			c->setPosition(pos);
			c->setAnimResources(t);
			clientes.push_back(c);
			texturas.push_back(t);
			setOrientation(c);
		}

		GrupoClientes* g = game->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->add();
		g->setVel(vel);
		cola->add(g, integrantes);
		g->initGrupo(cola, clientes);


		sdlutils().soundEffects().at("puerta").play();

		game->getNetworkManager()->sendGrupoCliente(integrantes, getId(), g->getId(), vel, dist, texturas, 100);

		return true;
	}

	return false;
}

bool Puerta::receiveGrupoClientes(GrupoClientes* gc)
{
	if (dynamic_cast<Tutorial*>(game->getCurrentScene()) && game->getCurrentScene()->getState() == echarClientes) {
		game->getCurrentScene()->changeState(pausaEcharClientes);
	}
	else if (dynamic_cast<Tutorial*>(game->getCurrentScene())) {
		game->getCurrentScene()->changeState(pausaNoEcharClientes);
		return false;
	}
	sdlutils().soundEffects().at("echarComensales").play(0, game->UI);
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

