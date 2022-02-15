#include "Puerta.h"

Puerta::Puerta(Game* game, Pool<Cliente>* pool):GameObject(game),poolClientes(pool), time(SDL_GetTicks())
{
	cola = new Cola();
}

void Puerta::update()
{
	if (SDL_GetTicks() - time >= SPAWN_DELAY) {

		int integrantes = 1 + rand() % (maxGrupo + 1);

		if (cola->esValido(integrantes)) {

			vector<Cliente*> v;
			v.push_back(poolClientes->add());

			int width = v[0]->getWidth();
			int w = -width/2;

			v[0]->setPosition({w,getY()});

			for (int i = 1; i < integrantes; i++) {
				w -= width;

				v.push_back(poolClientes->add({w, getY()}));
			}

			cola->add(new GrupoClientes(game, &v));
		}

		
		time = SDL_GetTicks();
	}

}
