#include "Mesa.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"



Mesa::Mesa(Game* game, Vector2D<double> pos, Vector2D<int> dim, string texture) 
	: Mueble(game, pos, dim.getX() * TILE_SIZE, dim.getY() * TILE_SIZE, texture) 
{
	mWidht = dim.getX();
	mHight = dim.getY();

	mGrupo = nullptr;

	nSillas = 0;
}

void Mesa::init()
{
	sillas.clear();

	int aum = 20;

	SDL_Rect c = { getX() - getWidth() / 2 - aum, getY() - getHeight() / 2 - aum,
		getWidth() + aum * 2, getHeight() + aum * 2 };

	for (auto i : game->getObjectManager()->getMueblesCollider(c)) {
		Silla* s = i->initMesa(this);
		if (s != nullptr)
			sillas.push_back(s);
	}

	nSillas = sillas.size();

	cout << nSillas << endl;
}

bool Mesa::receiveGrupoClientes(GrupoClientes* gc)
{
	game->getObjectManager()->initMuebles();

	if (mGrupo == nullptr) {
		int n = gc->numIntegrantes();

		if (n <= sillas.size()) {
			cout << "Clientes colocados" << endl;

			mGrupo = gc;

			gc->setPosition(getPosition());

			vector<Cliente*> clientes = gc->getIntegrantes();
			for (int i = 0; i < n; i++) {
				clientes[i]->setPosition(sillas[i]->getPosition());
			}
			return true;
		}

		
	}	
	return false;
}

bool Mesa::returnObject(Player* p)
{
	if (mGrupo != nullptr) {
		cout << "Clientes recogidos" << endl;

		p->setPickedObject(mGrupo, objectType::CLIENTES);

		mGrupo = nullptr;
		return true;
	}

	return false;
}






