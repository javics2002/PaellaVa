#include "Puerta.h"
#include "../../Control/Game.h"

Puerta::Puerta(Game* game, Pool<Cliente>* pool):GameObject(game),poolClientes(pool), time(SDL_GetTicks())
{
	cola = new Cola();
	textureName = inicioCintaTexture;
	setDimension(DIMENSION_W, DIMENSION_H);
	setPosition(DIMENSION_W / 2, game->getWindowHeight() - DIMENSION_H / 2);
}

void Puerta::update()
{
	if (SDL_GetTicks() - time >= SPAWN_DELAY) {
		
		
		int integrantes = 1 + rand() % maxGrupo;

		if (cola->esValido(integrantes)) {
			cout << integrantes << endl;

			vector<Cliente*> v;
			v.push_back(poolClientes->add());

			int width = v[0]->getWidth()+SEPARACIONCLIENTE;
			int w = -width/2;

			v[0]->setPosition(Vector2D<double>(w, getY()));

			for (int i = 1; i < integrantes; i++) {
				w -= width;

				v.push_back(poolClientes->add(Vector2D<double>(w, getY())));
			}

			GrupoClientes* g = new GrupoClientes(game, &v);
			cola->add(g, integrantes);
			g->setPosCola(cola->getPos());
		}

		
		time = SDL_GetTicks();
	}

}
