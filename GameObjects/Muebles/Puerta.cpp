#include "Puerta.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"
#include "../../GameObjects/UI/UIManager.h"

Puerta::Puerta(Game* game, Vector2D<double> pos,bool vertical_,int t_Max,int tamMaxGrupo_, bool host_) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "puerta")
{
	cola = new Cola(t_Max);
	vertical = vertical_;
	maxTamGrupo = tamMaxGrupo_;

	host = host_;
}

void Puerta::update()
{
	if (!host)
		return;


	if (sdlutils().virtualTimer().currTime() - initTime >= SPAWN_DELAY) {
		int integrantes = 1 + rand() % maxTamGrupo;

		if (cola->esValido(integrantes)) {
			vector<Cliente*> v;

			Cliente* c = game->getObjectManager()->getPool<Cliente>(_p_CLIENTE)->add();
			c->setPosition(getX(), getY());
			int width = c->getWidth();
			int heigh = c->getHeight();
			int w = c->getPosition().getX();
			int h = c->getPosition().getY();

			c->cambiaTextura(texturasClientes[0 + rand() % texturasClientes.size()]);
			if(!vertical)c->setPosition(Vector2D<double>(w + 10, getY()));
			else if(vertical) c->setPosition(Vector2D<double>(getX(), h + 10));

			v.push_back(c);
		
			for (int i = 1; i < integrantes; i++) {
				w -= width;
				h -= heigh;

				if(!vertical)c = game->getObjectManager()->getPool<Cliente>(_p_CLIENTE)->add(Vector2D<double>(w + 10, getY()));
				else if(vertical)c = game->getObjectManager()->getPool<Cliente>(_p_CLIENTE)->add(Vector2D<double>(getX(),h + 10));
				c->cambiaTextura(texturasClientes[0 + rand() % texturasClientes.size()]);

				v.push_back(c);
			}

			GrupoClientes* g = game->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->add();
			g->setVel(vel);
			cola->add(g, integrantes);
			g->initGrupo(cola, v);

			sdlutils().soundEffects().at("puerta").play();
		}

		initTime = sdlutils().virtualTimer().currTime();
	}
}

bool Puerta::receiveGrupoClientes(GrupoClientes* gc)
{
	
	gc->setOrientacion(vertical);
	gc->setActive(false);

	return true;
}

SDL_Rect Puerta::getCollider()
{
	return getTexBox();
}

SDL_Rect Puerta::getOverlap()
{
	return getTexBox();
}
