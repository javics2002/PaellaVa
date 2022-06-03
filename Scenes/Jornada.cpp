#include "Jornada.h"
#include "../GameObjects/GrupoClientes.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Paella.h"
#include "../GameObjects/Ingrediente.h"
#include "../GameObjects/Muebles/Pila.h"
#include "../GameObjects/Muebles/CajaHerramientas.h"
#include "../GameObjects/Muebles/MueblesInclude.h"
#include  "../GameObjects/UI/RedactaComandabutton.h"
#include "../sdlutils/SDLUtils.h"
#include "../Control/Game.h"
#include "../Data/ListaComandas.h"
#include "../GameObjects/UI/Reloj.h"

#ifdef _DEBUG
#include "../Scenes/GameOver.h"
#endif // _DEBUG

#include <iostream>

using namespace std;

Jornada::Jornada(Game* mGame, string tilemap, int numeroJornada, bool host_) : Scene(mGame)
{
	sdlutils().musics().at("musicaMenuInicio").haltMusic();

	sdlutils().musics().at("musicaJugando").play();

	this->mGame = mGame;
	mNumJornada = numeroJornada;

	mHost = host_;

#ifdef _DEBUG
	auto mStartButton = new UiButton(mGame, "start", 640, 100, 100, 100);
	mStartButton->setInitialDimension(100, 100);
	mStartButton->setAction([this, mStartButton](Game* mGame, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, mGame->UI);

		mUiManager->addTween(0.9f, 1.0f, 600.0f,false).via(easing::exponentialOut).onStep([mGame, mStartButton, this](tweeny::tween<float>& t, float) mutable {
			mStartButton->setDimension(t.peek() * mStartButton->getInitialWidth(), t.peek() * mStartButton->getInitialHeight());

			if (t.progress() > .2f) {
				//Start game
				mGame->getNetworkManager()->setGameStarted(false);

				mGame->sendMessageScene(new GameOver(mGame, getPunctuationJornada(), mNumJornada));
				mGame->getNetworkManager()->sendFinishGame(mPuntuacionTotal, mNumJornada);
				return true;
			}
			return false;
			});
		});

	mUiManager->addInterfaz(mStartButton);
#endif // _DEBUG

	// crear player dependiendo si es cocinera o no
	
	mMapInfo.mRuta = "Assets\\Tilemap\\" + tilemap + ".tmx";
	loadMap(mMapInfo.mRuta);

	mBackground->setTexture(mMapInfo.mRuta);
	mBackground->setPosition(mMapInfo.mAnchoFondo / 2, sdlutils().height() / 2);
	mBackground->setDimension(mMapInfo.mAnchoFondo, mMapInfo.mAltoFondo);

	if (mHost) {
		Player* mPlayer = new Player(mGame, mPositionCocinera.getX(), mPositionCocinera.getY(),true);
		mObjectManager->addPlayer(mPlayer);

		// Jugador 2
		Player* mPlayer2 = new Player(mGame, mPositionCamarero.getX(), mPositionCamarero.getY(),false);
		mObjectManager->addPlayer(mPlayer2);
	}
	else {
		Player* mPlayer = new Player(mGame, mPositionCamarero.getX(), mPositionCamarero.getY(), false);
		mObjectManager->addPlayer(mPlayer);

		// Jugador 2
		Player* mPlayer2 = new Player(mGame, mPositionCocinera.getX(), mPositionCocinera.getY(), true);
		mObjectManager->addPlayer(mPlayer2);
	}

	// camara init
	mCamera = new Camera(*new Vector2D<float>(0, 16), sdlutils().width(), sdlutils().height());
	mRedactaBut = new RedactaComandabutton(mGame, mUiManager, "redactaboton", 100, 20, 130, 60);
	mUiManager->addInterfaz(mRedactaBut);
	mUiManager->setRedactaboton(mRedactaBut);
	mUiManager->addInterfaz(new Imagen(mGame, 50, 20, 40, 40, "R"));
	mUiManager->setBarra(new ListaComandas(mGame, mUiManager));

	mUiManager->creaMenuPausa();

	mUiManager->addInterfaz(new Reloj(mGame, mNumJornada));

	mObjectManager->initMuebles();

	mUiManager->setEnJornada(true);
}

Jornada::~Jornada()
{
}

void Jornada::handleInput(bool& exit)
{
	Scene::handleInput(exit);

	if (mPaused) {
		if (ih().getKey(InputHandler::A)) {
			mUiManager->getPauseButtons()[1]->execute(exit);
		}
		else if (ih().getKey(InputHandler::B)) {
			mUiManager->getPauseButtons()[0]->execute(exit);
		}
			
	}
	else if (ih().getKey(InputHandler::PAUSE)) {
		//Abrir menú de pausa
		togglePause();

		// Mandar mensaje para pausar el juego
		mGame->getNetworkManager()->syncPause();
	}

	if (ih().getKey(InputHandler::Y)) {
		if (mUiManager->getComanda() == nullptr) {
			sdlutils().soundEffects().at("sacarComandas").play(0, mGame->UI);
			mUiManager->creaComanda(mGame);
		}
		else {
			mUiManager->getComanda()->toggleactive();
		}
	
	}
}

void Jornada::update()
{
	if (!mPaused) {
		mObjectManager->update();

		Player* mPlayer = mObjectManager->getPlayerOne();
		if (mPlayer != nullptr) {
			if (mPlayer->getX() > mTamRestaurante.getY() + TILE_SIZE) { // tamRestaurante es un rango, no una posición, por eso tengo que hacer getY()
				mCamera->Lerp(Vector2D<float>(mTamRestaurante.getY(), 16), LERP_INTERPOLATION);
			}
			else if (mPlayer->getX() < mTamRestaurante.getY()) {
				mCamera->Lerp(Vector2D<float>(mTamRestaurante.getX(), 16), LERP_INTERPOLATION);
			}
		}
	}

	mUiManager->update(mPaused);
}


void Jornada::refresh()
{
	if (!mPaused) {
		mObjectManager->refresh();
	}
}

void Jornada::addPuntuaciones(double puntosComanda)
{
	mPuntuacionesComandas.push_back(puntosComanda);
	cout << "puntos paella" << mPuntuacionesComandas.back() << endl;
}

void Jornada::mediaPuntuaciones()
{
	int mSumaMedia = 0;
	for (auto i : mPuntuacionesComandas) {
		mSumaMedia += i;
	}
	if(mPuntuacionesComandas.size() > 0)
		mPuntuacionTotal= mSumaMedia / mPuntuacionesComandas.size();
}

void Jornada::loadMap(string const& path)
{
	//Cargamos el mapa .tmx del archivo indicado
	mMapInfo.mTilemap = new tmx::Map();
	mMapInfo.mTilemap->load(path);


	//Obtenemos las dimensiones del mapa en tiles
	auto map_dimensions = mMapInfo.mTilemap->getTileCount();
	mMapInfo.mColumnas = map_dimensions.x;
	mMapInfo.mFilas = map_dimensions.y;

	//Obtenemos el tamaño de los tiles
	auto tilesize = mMapInfo.mTilemap->getTileSize();
	mMapInfo.mAnchoTile = tilesize.x;
	mMapInfo.mAltoTile = tilesize.y;

	//Creamos la textura de fondo
	auto renderer = sdlutils().renderer();
	mMapInfo.mAnchoFondo = mMapInfo.mAnchoTile * mMapInfo.mColumnas;
	mMapInfo.mAltoFondo = mMapInfo.mAltoTile * mMapInfo.mFilas;
	SDL_Texture* fondo = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
		mMapInfo.mAnchoFondo,
		mMapInfo.mAltoFondo
	);

	SDL_SetTextureBlendMode(fondo, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(renderer, fondo);

	//Establecemos los bordes de la camara con respecto al tamaño del tilemap
	//Camera::mainCamera->setBounds(0, 0, mapInfo.cols * mapInfo.tile_width, mapInfo.rows * mapInfo.tile_height);

	//Cargamos y almacenamos los tilesets utilizados por el tilemap
	// (el mapa utiliza el �ndice [gid] del primer tile cargado del tileset como clave)
	// (para poder cargar los tilesets del archivo .tmx, les ponemos de nombre 
	// el nombre del archivo sin extension en el .json) 
	auto& mapTilesets = mMapInfo.mTilemap->getTilesets();
	for (auto& tileset : mapTilesets) {
		string name = tileset.getName();
		Texture* texture = &sdlutils().tilesets().find(name)->second;
		mMapInfo.mTilesets.insert(pair<uint, Texture*>(tileset.getFirstGID(), texture));
	}

	// recorremos cada una de las capas (de momento solo las de tiles) del mapa
	auto& map_layers = mMapInfo.mTilemap->getLayers();
	for (auto& layer : map_layers) {
		// aqui comprobamos que sea la capa de tiles
		if (layer->getType() == tmx::Layer::Type::Tile) {
			// cargamos la capa
			tmx::TileLayer* tile_layer = dynamic_cast<tmx::TileLayer*>(layer.get());

			// obtenemos sus tiles
			auto& layer_tiles = tile_layer->getTiles();

			// recorremos todos los tiles para obtener su informacion
			for (auto y = 0; y < mMapInfo.mFilas; ++y) {
				for (auto x = 0; x < mMapInfo.mColumnas; ++x) {
					// obtenemos el indice relativo del tile en el mapa de tiles
					auto tile_index = x + (y * mMapInfo.mColumnas);

					// con dicho indice obtenemos el indice del tile dentro de su tileset
					auto cur_gid = layer_tiles[tile_index].ID;

					// si es 0 esta vacio asi que continuamos a la siguiente iteracion
					if (cur_gid == 0)
						continue;

					// guardamos el tileset que utiliza este tile (nos quedamos con el tileset cuyo gid sea
					// el mas cercano, y a la vez menor, al gid del tile)
					auto tset_gid = -1, tsx_file = 0;;
					for (auto& ts : mMapInfo.mTilesets) {
						if (ts.first <= cur_gid) {
							tset_gid = ts.first;
							tsx_file++;
						}
						else
							break;
					}

					// si no hay tileset v�lido, continuamos a la siguiente iteracion
					if (tset_gid == -1)
						continue;

					// normalizamos el �ndice
					cur_gid -= tset_gid;

					// calculamos dimensiones del tileset
					auto ts_width = 0;
					auto ts_height = 0;
					SDL_QueryTexture(mMapInfo.mTilesets[tset_gid]->getTexture(),
						NULL, NULL, &ts_width, &ts_height);

					// calculamos el area del tileset que corresponde al dibujo del tile
					auto region_x = (cur_gid % (ts_width / mMapInfo.mAnchoTile)) * mMapInfo.mAnchoTile;
					auto region_y = (cur_gid / (ts_width / mMapInfo.mAnchoTile)) * mMapInfo.mAltoTile;

					// calculamos la posicion del tile
					auto x_pos = x * mMapInfo.mAnchoTile;
					auto y_pos = y * mMapInfo.mAltoTile;

					// metemos el tile
					auto tileTex = mMapInfo.mTilesets[tset_gid];

					SDL_Rect src;
					src.x = region_x; src.y = region_y;
					src.w = mMapInfo.mAnchoTile;
					src.h = mMapInfo.mAltoTile;

					SDL_Rect dest;
					dest.x = x_pos;
					dest.y = y_pos;
					dest.w = src.w;
					dest.h = src.h;

					mMapInfo.mTilesets[tset_gid]->render(src, dest);
				}
			}
		}
		if (layer->getType() == tmx::Layer::Type::Object) {
			tmx::ObjectGroup* object_layer = dynamic_cast<tmx::ObjectGroup*>(layer.get());

			auto& objs = object_layer->getObjects();


			for (auto obj : objs) {
				auto& aabb = obj.getAABB();
				auto position = Vector2D<double>(aabb.left, aabb.top);
				auto dimension = Vector2D<int>(mMapInfo.mAnchoTile, mMapInfo.mAltoTile);
				string name = obj.getName();
				vector<tmx::Property> p = obj.getProperties();

				/// <Z coords>
				/// Fondo: -1
				/// Pared: -5 (no tiene sprite o si tiene no queremos verlo)
				/// Silla Arriba = 0
				/// Silla Izq/Silla Der = 1
				/// Mesas = 1
				/// Silla Abajo = 2
				/// Muebles = 1
				/// Puerta = 1
				/// 
				/// </Z coords>
				if (name == "camarero") {
					mPositionCamarero = position;
				}
				if (name == "cocinera") {
					mPositionCocinera = position;
				}
				if (name == "mesaS") { // 1 tile
					Mesa* m = new Mesa(mGame, position, { 1, 2 }, { 1 , 1 }, name);
					m->setTiles(21, 1);
					m->setId(mIdCount);
					mIdCount++;
					getObjectManager()->addMueble(m);
				}
				else if (name == "mesaMH") { // 2 tiles horizontal
					Mesa* m = new Mesa(mGame, position, { 2, 2 }, { 2 , 1 }, name);
					m->setTiles(22, 1);
					m->setId(mIdCount);
					mIdCount++;
					getObjectManager()->addMueble(m);
				}
				else if (name == "mesaMV") { // 2 tiles vertical
					Mesa* m = new Mesa(mGame, position, { 1, 3 }, { 1 , 2 }, name);
					m->setTiles(38, 0.85);
					m->setId(mIdCount);
					mIdCount++;
					getObjectManager()->addMueble(m);
				}
				else if (name == "mesaL") { // 4 tiles
					Mesa* m = new Mesa(mGame, position, { 2, 3 }, { 2 , 2 }, name);
					m->setTiles(36, 0.85);
					m->setId(mIdCount);
					mIdCount++;
					getObjectManager()->addMueble(m);
				}
				else if (name == "sillaDer") {
					Silla* s = new SillaE(mGame, position, name);
					s->setId(mIdCount);
					mIdCount++;
					getObjectManager()->addMueble(s);
				}
				else if (name == "sillaIz") {
					Silla* s = new SillaO(mGame, position, name);
					s->setId(mIdCount);
					mIdCount++;
					getObjectManager()->addMueble(s);
				}
				else if (name == "sillaAr") {
					Silla* s = new SillaS(mGame, position, name);
					s->setId(mIdCount);
					mIdCount++;
					getObjectManager()->addMueble(s);
				}
				else if (name == "sillaAb") {
					Silla* s = new SillaN(mGame, position, name);
					s->setId(mIdCount);
					mIdCount++;
					getObjectManager()->addMueble(s);
				}
				else if (name == "fogon") {
					Fogon* f = new Fogon(mGame, position);
					f->setId(mIdCount);
					mIdCount++;
					getObjectManager()->addMueble(f);
				}
				else if (name == "lavavajillas") {
					Lavavajillas* l = new Lavavajillas(mGame, position);
					l->setId(mIdCount);
					mIdCount++;
					getObjectManager()->addMueble(l);
				}
				else if (name == "cinta") {
					Cinta* c = new Cinta(mGame, position);
					SDL_Rect aux = c->getTexBox();
					if (p[0].getBoolValue()) {
						c->setCollider({ aux.x, aux.y + aux.h / 2 , aux.w, aux.h / 2 });
					}
					else c->setCollider(aux);
					getObjectManager()->addMueble(c);
				}
				else if (name == "inicioCinta") {
					InicioCinta* c = new InicioCinta(mGame, position);
					c->setVel(Vector2D<double>((double)p[1].getFloatValue(), (double)p[2].getFloatValue()));

					c->setId(mIdCount);
					mIdCount++;

					getObjectManager()->addMueble(c);
				}
				else if (name == "finalCinta") {
					FinalCinta* c = new FinalCinta(mGame, position);
					c->setId(mIdCount);
					mIdCount++;
					getObjectManager()->addMueble(c);
				}
				else if (name == "puerta") { 
					Puerta* puerta = new Puerta(mGame, position, p[3].getIntValue(), p[0].getIntValue());
					puerta->setVel(Vector2D<double>((double)p[1].getFloatValue(), (double)p[2].getFloatValue()));

					puerta->setId(mIdCount);
					mIdCount++;

					getObjectManager()->addMueble(puerta);
				}
				else if (name == "cartel") {
					Cartel* c = new Cartel(mGame, position);
					getObjectManager()->addMueble(c);
				}
				else if (name == "tabla") {
					TablaProcesado* t = new TablaProcesado(mGame, position);
					t->setId(mIdCount);
					mIdCount++;
					getObjectManager()->addMueble(t);
				}
				else if (name == "encimera") {
					Encimera* e = new Encimera(mGame, position);
					e->setId(mIdCount);
					mIdCount++;
					getObjectManager()->addMueble(e);
				}
				else if (name == "arroz") {
					BolsaArroz* b = new BolsaArroz(mGame, position);
					b->setId(mIdCount);
					mIdCount++;
					getObjectManager()->addMueble(b);
				}
				else if (name == "pared") {
					Pared* p = new Pared(mGame, position);
					p->setDepth(-5);
					getObjectManager()->addMueble(p);
				}
				else if (name == "pilaS")
				{
					Pila* p = new Pila(mGame, position, TipoPaella::Pequena, 4);
					p->setId(mIdCount);
					mIdCount++;
					getObjectManager()->addMueble(p);
				}
				else if (name == "pilaM")
				{
					Pila* p = new Pila(mGame, position, TipoPaella::Mediana, 5);
					p->setId(mIdCount);
					mIdCount++;
					getObjectManager()->addMueble(p);
				}
				else if (name == "pilaL")
				{
					Pila* p = new Pila(mGame, position, TipoPaella::Grande, 3);
					p->setId(mIdCount);
					mIdCount++;
					getObjectManager()->addMueble(p);
				}
				else if (name == "cajaHerramientas")
				{
					CajaHerramientas* ch = new CajaHerramientas(mGame, position);
					ch->setId(mIdCount);
					mIdCount++;
					getObjectManager()->addMueble(ch);
				}
				else if (name == "torreCajaTakeaway")
				{
					TorreCajaTakeaway* torre = new TorreCajaTakeaway(mGame, position);
					torre->setId(mIdCount);
					mIdCount++;
					getObjectManager()->addMueble(torre);
				}
			}
		}
		// ordenar render
		getObjectManager()->sortAllGO();

		SDL_SetRenderTarget(renderer, nullptr);

		if (!sdlutils().images().count(path))
			sdlutils().images().emplace(path, Texture(renderer, fondo, mMapInfo.mAnchoFondo, mMapInfo.mAltoFondo));
	}
}

void Jornada::togglePause()
{
	mUiManager->togglePause();

	mPaused = !mPaused;

	if (mPaused) {
		sdlutils().virtualTimer().pause();

		sdlutils().soundEffects().at("cancel").play(0, mGame->UI);
	}
	else {
		sdlutils().virtualTimer().resume();

		sdlutils().soundEffects().at("select").play(0, mGame->UI);
	}
}
