#include "Mesa.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"


Mesa::Mesa(Game* game, Vector2D<double> pos, Vector2D<int> dim, string texture) 
	: Mueble(game, pos, dim.getX() * TILE_SIZE, dim.getY() * TILE_SIZE, texture) 
{
	mWidht_ = dim.getX();
	mHight_ = dim.getY();

	nSillas = 0;
}

void Mesa::init()
{
	int aum = 20;

	SDL_Rect c = { getX() - getWidth() / 2 - aum, getY() - getHeight() / 2 - aum,
		getWidth() + aum * 2, getHeight() + aum * 2 };

	for (auto i : game->getObjectManager()->getMueblesCollider(c)) {
		Silla* s = i->initMesa();
		if (s != nullptr)
			sillas.push_back(s);
	}

	nSillas = sillas.size();

	cout << nSillas << endl;
}

bool Mesa::recieveGrupoClientes(GrupoClientes* gc)
{
	int n = gc->numIntegrantes();

	if (n <= sillas.size()) {
		vector<Cliente*> clientes = gc->getIntegrantes();
		for (int i = 0; i < n; i++) {
			clientes[i]->setPosition(sillas[i]->getPosition());
		}
		return true;
	}
	return false;
}

