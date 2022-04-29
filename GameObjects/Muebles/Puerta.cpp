#include "Puerta.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"
#include "../../GameObjects/UI/UIManager.h"
#include "../../Control/NetworkManager.h"
#include "../../Scenes/Tutorial.h"

Puerta::Puerta(Game* game, Vector2D<double> pos, int t_Max, int tamMaxGrupo_, bool host_) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "puerta")
{
	cola = new Cola(t_Max);
	maxTamGrupo = 3;

	host = host_;

	//Siempre tiene que funcionar
	funcionando = true;
}

void Puerta::update()
{
	if (!host)
		return;

	if (dynamic_cast<Tutorial*>(game->getCurrentScene())) {

		if (sdlutils().virtualTimer().currTime() - initTime >= SPAWN_DELAY && isActive()
			&& cola->esValido(1) && game->getCurrentScene()->getState() == States::cogerClientes) {
			vector<Cliente*> clientes;
			Cliente* c = game->getObjectManager()->getPool<Cliente>(_p_CLIENTE)->add();

			Vector2D<double> dist = vel;
			dist.normalize();
			dist = Vector2D<double>(dist.getX() * c->getWidth(), dist.getY() * c->getHeight());

			Vector2D<double> pos = getPosition();

			int t = rand() % texturasClientes.size();
			c->setPosition(getX(), getY());
			c->cambiaTextura(texturasClientes[t]);
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
	else if (sdlutils().virtualTimer().currTime() - initTime >= SPAWN_DELAY && isActive()) {
		int integrantes = 1 + rand() % maxTamGrupo;

		if (cola->esValido(integrantes)) {

			vector<Cliente*> clientes;
			vector<int> texturas;

			Cliente* c = game->getObjectManager()->getPool<Cliente>(_p_CLIENTE)->add();		
			
			Vector2D<double> dist = vel;
			dist.normalize();
			dist = Vector2D<double>(dist.getX() * c->getWidth(), dist.getY() * c->getHeight());

			Vector2D<double> pos = getPosition();
		
			int t = rand() % texturasClientes.size();
			c->setPosition(getX(), getY());
			c->cambiaTextura(texturasClientes[t]);
			clientes.push_back(c);
			texturas.push_back(t);
		
			for (int i = 1; i < integrantes; i++) {
				pos = pos - dist;

				c = game->getObjectManager()->getPool<Cliente>(_p_CLIENTE)->add();

				t = rand() % texturasClientes.size();
				c->setPosition(pos);
				c->cambiaTextura(texturasClientes[t]);
				clientes.push_back(c);
				texturas.push_back(t);
			}

			GrupoClientes* g = game->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->add();
			g->setVel(vel);
			cola->add(g, integrantes);
			g->initGrupo(cola, clientes);


			sdlutils().soundEffects().at("puerta").play();

			game->getNetworkManager()->sendGrupoCliente(integrantes, getId(), g->getId(), vel, dist, texturas, 100);
		}

		initTime = sdlutils().virtualTimer().currTime();
	}
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
	gc->setOrientacion(vertical);
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
