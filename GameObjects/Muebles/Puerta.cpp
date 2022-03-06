#include "Puerta.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"

Puerta::Puerta(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "puerta")
{
	cola = new Cola();
}

void Puerta::update()
{
	if (SDL_GetTicks() - time >= SPAWN_DELAY) {

		int integrantes = 1 + rand() % maxGrupo;

		if (cola->esValido(integrantes)) {
			vector<Cliente*> v;

			Cliente* c = game->getObjectManager()->getPoolClientes()->add();
			c->cambiaTextura(texturasClientes[0 + rand() % texturasClientes.size()]);
			c->setPosition(Vector2D<double>(w, getY()));
			v.push_back(c);

			int width = c->getWidth();
			int w = -width / 2;
		
			for (int i = 1; i < integrantes; i++) {
				w -= width;

				c = game->getObjectManager()->getPoolClientes()->add(Vector2D<double>(w, getY()));
				c->cambiaTextura(texturasClientes[0 + rand() % texturasClientes.size()]);
				v.push_back(c);
			}

			GrupoClientes* g = game->getObjectManager()->getPoolGrupoClientes()->add();
			cola->add(g, integrantes);
			g->setGrupo(cola->getPos(), v);
		}

		time = SDL_GetTicks();
	}
}

