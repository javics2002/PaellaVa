#include "Mesa.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"

Mesa::Mesa(Game* game, Vector2D<double> pos, Vector2D<int> dim, string texture) 
	: Mueble(game, pos, dim.getX() * TILE_SIZE, dim.getY() * TILE_SIZE / 1.15, texture) 
{
	mWidth = dim.getX();
	mHeight = dim.getY();

	setPosition((int)getX(), (int)getY() + h / 8);


	mGrupo = nullptr;

	nSillas = 0;
}

void Mesa::init(ObjectManager* objectManager)
{
	sillas.clear();

	int aum = 20;

	SDL_Rect c = { getX() - getWidth() / 2 - aum, getY() - getHeight() / 2 - aum,
		getWidth() + aum * 2, getHeight() + aum * 2 };

	for (auto i : objectManager->getMueblesCollider(c)) {
		Silla* s = dynamic_cast<Mueble*>(i)->initMesa(this);
		if (s != nullptr)
			sillas.push_back(s);
	}

	nSillas = sillas.size();
}

bool Mesa::receiveGrupoClientes(GrupoClientes* gc)
{
	if (mGrupo == nullptr && paellas.empty() && gc->canDrop()) {
		int n = gc->numIntegrantes();

		if (n <= sillas.size()) {
			mGrupo = gc;

			gc->setPosition(getPosition());

			gc->hacerPedido(mWidth * mHeight,this);


			vector<Cliente*> clientes = gc->getIntegrantes();
			for (int i = 0; i < n; i++) {
				auto p = sillas[i]->getPosition();
				clientes[i]->setPosition(p);
			}
			return true;
		}		
	}	
	return false;
}

bool Mesa::receivePaella(Paella* paella)
{
	if (mGrupo != nullptr && paella->conArroz() && mGrupo->paellasPedidas()) {
		paella->setState(Hecha);
		paellas.push_back(paella);
		paella->setPosition(getPosition());
		paella->enLaMesa(true);

		sdlutils().soundEffects().at("cubiertos").play();
		return true;
	}
	return false;
}

bool Mesa::returnObject(Player* p)
{
	if (mGrupo != nullptr ) {
		if (mGrupo->canPick()) {
			p->setPickedObject(mGrupo, objectType::CLIENTES);

			return true;
		}
	}
	else if (!paellas.empty()) {
		p->setPickedObject(paellas.back(), objectType::PAELLA);
		paellas.back()->enLaMesa(false);
		paellas.back()->setContenido(Sucia);
		paellas.pop_back();

		return true;
	}

	return false;
}

bool Mesa::colisionPlayer(Player* p)
{
	if (mGrupo != nullptr)
		mGrupo->decirPedidio();

	return false;
}

void Mesa::clienteSeVa()
{
	mGrupo = nullptr;
}


vector<Paella*> Mesa::getPaellasEntregadas()
{
	vector<Paella*> v;

	for (auto it : paellas) {
		v.push_back(it);
	}

	return v;
}

void Mesa::comerPaellas() {
	for (auto i : paellas) {
		i->comerPaella();
	}
}






