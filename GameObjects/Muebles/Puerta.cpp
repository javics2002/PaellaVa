#include "Puerta.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"
#include "../../GameObjects/UI/UIManager.h"

Puerta::Puerta(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "puerta")
{
	cola = new Cola();
}

void Puerta::update()
{
	if (sdlutils().virtualTimer().currTime() - initTime >= SPAWN_DELAY) {
		int integrantes = 1 + rand() % MAX_TAM;

		if (cola->esValido(integrantes)) {
			vector<Cliente*> v;

			Cliente* c = game->getObjectManager()->getPool<Cliente>(_p_CLIENTE)->add();

			int width = c->getWidth();
			int w = c->getPosition().getX();

			c->cambiaTextura(texturasClientes[0 + rand() % texturasClientes.size()]);
			c->setPosition(Vector2D<double>(w, getY()));

			v.push_back(c);
		
			for (int i = 1; i < integrantes; i++) {
				w -= width;

				c = game->getObjectManager()->getPool<Cliente>(_p_CLIENTE)->add(Vector2D<double>(w, getY()));
				c->cambiaTextura(texturasClientes[0 + rand() % texturasClientes.size()]);

				v.push_back(c);
			}

			GrupoClientes* g = game->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->add();
			cola->add(g, integrantes);
			g->initGrupo(cola, v);
		}
		initTime = sdlutils().virtualTimer().currTime();

		sdlutils().soundEffects().at("puerta").play();
	}
}

bool Puerta::receiveGrupoClientes(GrupoClientes* gc)
{
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
