#include "Mesa.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"
#include "../../Scenes/Tutorial.h"

Mesa::Mesa(Game* game, Vector2D<double> pos, Vector2D<int> dim, Vector2D<int> tiles, string texture)
	: Mueble(game, pos, dim.getX() * TILE_SIZE, dim.getY() * TILE_SIZE, texture) 
{
	mWidth = tiles.getX();
	mHeight = tiles.getY();

	setTiles();


	mGrupo = nullptr;

	nSillas = 0;

	//Siempre tiene que funcionar
	funcionando = true;
}

void Mesa::setTiles() {
	Vector2D<double> supIzq = Vector2D<double>(getTexBox().x, getTexBox().y);

	int dist = getTexBox().w / (2 * mWidth);

	//cout << dist << endl;

	for (int i = 0; i < mWidth; i++) {
		for (int j = 0; j < mHeight; j++) {
			tiles[i][j].first = Vector2D<double>(supIzq.getX() + dist + dist * 2 * i, supIzq.getY() + dist + dist * 2 *j);
			tiles[i][j].second = true;
		}
	}
}

Vector2D<double> Mesa::getProxPos(Vector2D<double> pos) {
	double minPos = INT_MAX;
	int I = 0, J = 0;
	
	for (int i = 0; i < mWidth; i++) {
		for (int j = 0; j < mHeight; j++) {
			double dist = (tiles[i][j].first - pos).magnitude();
			if (dist < minPos && tiles[i][j].second) {
				minPos = dist;
				I = i;
				J = j;
			}
		}
	}

	tiles[I][J].second = false;

	return tiles[I][J].first;
}

void Mesa::init(ObjectManager* objectManager)
{
	sillas.clear();

	int aum = 20;

	SDL_Rect c = { getX() - getWidth() / 2 - aum, getY() - getHeight() / 2 - aum,
		getWidth() + aum * 2, getHeight() + aum * 2 };

	for (auto i : objectManager->getMueblesOverlaps(c)) {
		Silla* s = i->initMesa(this);
		if (s != nullptr)
			sillas.push_back(s);
	}


	nSillas = sillas.size();
}

bool Mesa::receiveGrupoClientes(GrupoClientes* gc)
{
	if (mGrupo == nullptr && paellas.empty() && gc->canDrop()) {
		int n = gc->numIntegrantes();

		if (dynamic_cast<Tutorial*>(game->getCurrentScene())) {
			game->getCurrentScene()->changeState(States::pausaPaellas);
		}

		if (n <= sillas.size()) {
			mGrupo = gc;

			gc->setPosition(getCenterMesa());

			gc->hacerPedido(mWidth * mHeight, this);

			vector<Cliente*> clientes = gc->getIntegrantes();
			for (int i = 0; i < n; i++) {
				clientes[i]->setPosition(sillas[i]->setClientPos());
			}
			return true;
		}		
	}	
	return false;
}

bool Mesa::receivePaella(Paella* paella)
{
	if (mGrupo != nullptr && paella->conArroz() ) {
		if (dynamic_cast<Tutorial*>(game->getCurrentScene())) {
			if (game->getCurrentScene()->getState() == States::darDeComer) {
				if (mGrupo->paellasPedidas()) {
					paella->setState(Hecha);
					paellas.push_back(paella);
					paella->setPosition(getProxPos(paella->getPosition()));
					paella->enLaMesa(true);
					game->getCurrentScene()->changeState(States::pausaDarDeComer);
					sdlutils().soundEffects().at("cubiertos").play();
					return true;
				}
			}
		}
		else if(mGrupo->paellasPedidas()){
			paella->setState(Hecha);
			paellas.push_back(paella);
			paella->setPosition(getProxPos(paella->getPosition()));
			paella->enLaMesa(true);

			sdlutils().soundEffects().at("cubiertos").play();
			return true;
		}
	}
	return false;
}

bool Mesa::returnObject(Player* p)
{
	if (mGrupo != nullptr ) {
		if (mGrupo->canPick()) {
			p->setPickedObject(mGrupo, objectType::CLIENTES);
			if (dynamic_cast<Tutorial*>(game->getCurrentScene()) && game->getCurrentScene()->getState() == sacarCuenta)
				game->getCurrentScene()->changeState(pausaSacarCuenta);

			return true;
		}
	}
	else if (!paellas.empty()) {
		p->setPickedObject(paellas.back(), objectType::PAELLA);
		paellas.back()->enLaMesa(false);
		paellas.back()->setContenido(Sucia);
		paellas.pop_back();

		if (dynamic_cast<Tutorial*>(game->getCurrentScene()) && game->getCurrentScene()->getState() == recogerMesa)
			game->getCurrentScene()->changeState(pausaRecogerMesa);


		if (paellas.empty()) {
			for (int i = 0; i < mWidth; i++) {
				for (int j = 0; j < mHeight; j++) {
					tiles[i][j].second = true;
				}
			}
		}

		return true;
	}

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

SDL_Rect Mesa::getOverlap() {

	SDL_Rect rect = getTexBox();

	int incr = 10;

	return {
		rect.x - incr,
		rect.y - incr,
		rect.w + incr * 2,
		rect.h + incr * 2
	};

}

void Mesa::decirPedido()
{
	if (mGrupo != nullptr)
		mGrupo->decirPedidio();
}

Vector2D<double> Mesa::getCenterMesa()
{
	double tileSize = getWidth() / mWidth;

	return Vector2D<double>(getTexBox().x + (tileSize * mWidth) / 2, getTexBox().y + (tileSize * mHeight) / 2);
}






