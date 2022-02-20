#include "Puerta.h"
#include "../../Control/Game.h"

Puerta::Puerta(Game* game, Pool<Cliente>* pool, Pool<GrupoClientes>* pool2) : 
	GameObject(game), poolClientes(pool), poolGrupos(pool2), time(SDL_GetTicks())
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

		cout << integrantes << endl;

		if (cola->esValido(integrantes)) {
			

			vector<Cliente*> v;
			Cliente* k= poolClientes->add();
			k->cambiaTextura(texturasClientes[0 + rand() % texturasClientes.size()]);
			v.push_back(k);

			int width = k->getWidth()+SEPARACIONCLIENTE;
			int w = -width/2;

			v[0]->setPosition(Vector2D<double>(w, getY()));

			for (int i = 1; i < integrantes; i++) {
				w -= width;
				
				k = poolClientes->add(Vector2D<double>(w, getY()));
				k->cambiaTextura(texturasClientes[0 + rand() % texturasClientes.size()]);
				v.push_back(k);


			}

			GrupoClientes* g = poolGrupos->add();
			cola->add(g, integrantes);
			g->setGrupo(cola->getPos(), v);			
		}
		
		time = SDL_GetTicks();
	}

}
