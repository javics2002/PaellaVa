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

Jornada::Jornada(Game* game, string tilemap, int numeroJornada, bool host_) : Scene(game)
{
	sdlutils().musics().at("musicaMenuInicio").haltMusic();

	sdlutils().musics().at("musicaJugando").play();

	this->game = game;
	nJornada = numeroJornada;

	host = host_;

#ifdef _DEBUG
	auto startButton = new UiButton(game, "start", 640, 100, 100, 100);
	startButton->setInitialDimension(100, 100);
	startButton->setAction([this, startButton](Game* game, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, game->UI);

		uiManager->addTween(0.9f, 1.0f, 600.0f,false).via(easing::exponentialOut).onStep([game, startButton, this](tweeny::tween<float>& t, float) mutable {
			startButton->setDimension(t.peek() * startButton->getInitialWidth(), t.peek() * startButton->getInitialHeight());

			if (t.progress() > .2f) {
				//Start game
				game->getNetworkManager()->setGameStarted(false);

				game->sendMessageScene(new GameOver(game, 0, nJornada));
				
				return true;
			}
			return false;
			});
		});

	uiManager->addInterfaz(startButton);
#endif // _DEBUG

	// crear player dependiendo si es cocinera o no
	
	mapInfo.ruta = "Assets\\Tilemap\\" + tilemap + ".tmx";
	loadMap(mapInfo.ruta);

	fondo->setTexture(mapInfo.ruta);
	fondo->setPosition(mapInfo.anchoFondo / 2, sdlutils().height() / 2);
	fondo->setDimension(mapInfo.anchoFondo, mapInfo.altoFondo);

	if (host) {
		Player* p = new Player(game, positionCocinera.getX(), positionCocinera.getY(),true);
		objectManager->addPlayer(p);

		// Jugador 2
		Player* p2 = new Player(game, positionCamarero.getX(), positionCamarero.getY(),false);
		objectManager->addPlayer(p2);
	}
	else {
		Player* p = new Player(game, positionCamarero.getX(), positionCamarero.getY(), false);
		objectManager->addPlayer(p);

		// Jugador 2
		Player* p2 = new Player(game, positionCocinera.getX(), positionCocinera.getY(), true);
		objectManager->addPlayer(p2);
	}

	// camara init
	camara = new Camera(*new Vector2D<float>(0, 16), sdlutils().width(), sdlutils().height());

	uiManager->addInterfaz(new RedactaComandabutton(game, uiManager, "redactaboton", 10, 10, 30, 30));
	uiManager->addInterfaz(new Imagen(game, 50, 20, 40, 40, "R"));
	uiManager->setBarra(new ListaComandas(game, uiManager));

	uiManager->creaMenuPausa();

	uiManager->addInterfaz(new Reloj(game, nJornada));

	objectManager->initMuebles();

	uiManager->setEnJornada(true);
}

Jornada::~Jornada()
{
}

void Jornada::handleInput(bool& exit)
{
	Scene::handleInput(exit);

	if (paused) {
		if (ih().getKey(InputHandler::A)) {
			uiManager->getPauseButtons()[1]->execute(exit);
		}
		else if (ih().getKey(InputHandler::B)) {
			uiManager->getPauseButtons()[0]->execute(exit);
		}
			
	}
	else if (ih().getKey(InputHandler::PAUSE)) {
		//Abrir menú de pausa
		togglePause();

		// Mandar mensaje para pausar el juego
		game->getNetworkManager()->syncPause();
	}

	if (ih().getKey(InputHandler::Y)) {
		if (uiManager->getComanda() == nullptr) {
			uiManager->creaComanda(game);
			getObjectManager()->getPlayerOne()->changeEnComanda(true);
		}
		else {
			uiManager->getComanda()->toggleactive();
		}
		
	}
}

void Jornada::update()
{
	if (!paused) {
		objectManager->update();

		Player* player = objectManager->getPlayerOne();
		if (player != nullptr) {
			if (player->getX() > tamRestaurante.getY() + TILE_SIZE) { // tamRestaurante es un rango, no una posición, por eso tengo que hacer getY()
				camara->Lerp(Vector2D<float>(tamRestaurante.getY(), 16), LERP_INTERPOLATION);
			}
			else if (player->getX() < tamRestaurante.getY()) {
				camara->Lerp(Vector2D<float>(tamRestaurante.getX(), 16), LERP_INTERPOLATION);
			}
		}
	}

	uiManager->update(paused);
}


void Jornada::refresh()
{
	if (!paused) {
		objectManager->refresh();
	}
}

void Jornada::addPuntuaciones(double puntosComanda)
{
	puntuacionesComandas.push_back(puntosComanda);
	cout << "puntos paella" << puntuacionesComandas.back() << endl;
}

void Jornada::mediaPuntuaciones()
{
	int sumaMedia = 0;
	for (auto i : puntuacionesComandas) {
		sumaMedia += i;
	}
	puntuaciónTotal = sumaMedia / puntuacionesComandas.size();
}

void Jornada::loadMap(string const& path)
{
	//Cargamos el mapa .tmx del archivo indicado
	mapInfo.tilemap = new tmx::Map();
	mapInfo.tilemap->load(path);


	//Obtenemos las dimensiones del mapa en tiles
	auto map_dimensions = mapInfo.tilemap->getTileCount();
	mapInfo.columnas = map_dimensions.x;
	mapInfo.filas = map_dimensions.y;

	//Obtenemos el tamaño de los tiles
	auto tilesize = mapInfo.tilemap->getTileSize();
	mapInfo.anchoTile = tilesize.x;
	mapInfo.altoTile = tilesize.y;

	//Creamos la textura de fondo
	auto renderer = sdlutils().renderer();
	mapInfo.anchoFondo = mapInfo.anchoTile * mapInfo.columnas;
	mapInfo.altoFondo = mapInfo.altoTile * mapInfo.filas;
	SDL_Texture* fondo = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
		mapInfo.anchoFondo,
		mapInfo.altoFondo
	);

	SDL_SetTextureBlendMode(fondo, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(renderer, fondo);

	//Establecemos los bordes de la camara con respecto al tamaño del tilemap
	//Camera::mainCamera->setBounds(0, 0, mapInfo.cols * mapInfo.tile_width, mapInfo.rows * mapInfo.tile_height);

	//Cargamos y almacenamos los tilesets utilizados por el tilemap
	// (el mapa utiliza el �ndice [gid] del primer tile cargado del tileset como clave)
	// (para poder cargar los tilesets del archivo .tmx, les ponemos de nombre 
	// el nombre del archivo sin extension en el .json) 
	auto& mapTilesets = mapInfo.tilemap->getTilesets();
	for (auto& tileset : mapTilesets) {
		string name = tileset.getName();
		Texture* texture = &sdlutils().tilesets().find(name)->second;
		mapInfo.tilesets.insert(pair<uint, Texture*>(tileset.getFirstGID(), texture));
	}

	// recorremos cada una de las capas (de momento solo las de tiles) del mapa
	auto& map_layers = mapInfo.tilemap->getLayers();
	for (auto& layer : map_layers) {
		// aqui comprobamos que sea la capa de tiles
		if (layer->getType() == tmx::Layer::Type::Tile) {
			// cargamos la capa
			tmx::TileLayer* tile_layer = dynamic_cast<tmx::TileLayer*>(layer.get());

			// obtenemos sus tiles
			auto& layer_tiles = tile_layer->getTiles();

			// recorremos todos los tiles para obtener su informacion
			for (auto y = 0; y < mapInfo.filas; ++y) {
				for (auto x = 0; x < mapInfo.columnas; ++x) {
					// obtenemos el indice relativo del tile en el mapa de tiles
					auto tile_index = x + (y * mapInfo.columnas);

					// con dicho indice obtenemos el indice del tile dentro de su tileset
					auto cur_gid = layer_tiles[tile_index].ID;

					// si es 0 esta vacio asi que continuamos a la siguiente iteracion
					if (cur_gid == 0)
						continue;

					// guardamos el tileset que utiliza este tile (nos quedamos con el tileset cuyo gid sea
					// el mas cercano, y a la vez menor, al gid del tile)
					auto tset_gid = -1, tsx_file = 0;;
					for (auto& ts : mapInfo.tilesets) {
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
					SDL_QueryTexture(mapInfo.tilesets[tset_gid]->getTexture(),
						NULL, NULL, &ts_width, &ts_height);

					// calculamos el area del tileset que corresponde al dibujo del tile
					auto region_x = (cur_gid % (ts_width / mapInfo.anchoTile)) * mapInfo.anchoTile;
					auto region_y = (cur_gid / (ts_width / mapInfo.anchoTile)) * mapInfo.altoTile;

					// calculamos la posicion del tile
					auto x_pos = x * mapInfo.anchoTile;
					auto y_pos = y * mapInfo.altoTile;

					// metemos el tile
					auto tileTex = mapInfo.tilesets[tset_gid];

					SDL_Rect src;
					src.x = region_x; src.y = region_y;
					src.w = mapInfo.anchoTile;
					src.h = mapInfo.altoTile;

					SDL_Rect dest;
					dest.x = x_pos;
					dest.y = y_pos;
					dest.w = src.w;
					dest.h = src.h;

					mapInfo.tilesets[tset_gid]->render(src, dest);
				}
			}
		}
		if (layer->getType() == tmx::Layer::Type::Object) {
			tmx::ObjectGroup* object_layer = dynamic_cast<tmx::ObjectGroup*>(layer.get());

			auto& objs = object_layer->getObjects();


			for (auto obj : objs) {
				auto& aabb = obj.getAABB();
				auto position = Vector2D<double>(aabb.left, aabb.top);
				auto dimension = Vector2D<int>(mapInfo.anchoTile, mapInfo.altoTile);
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
					positionCamarero = position;
				}
				if (name == "cocinera") {
					positionCocinera = position;
				}
				if (name == "mesaS") { // 1 tile
					Mesa* m = new Mesa(game, position, { 1, 2 }, { 1 , 1 }, name);
					m->setId(idCount);
					idCount++;
					getObjectManager()->addMueble(m);
				}
				else if (name == "mesaMH") { // 2 tiles horizontal
					Mesa* m = new Mesa(game, position, { 2, 2 }, { 2 , 1 }, name);
					m->setId(idCount);
					idCount++;
					getObjectManager()->addMueble(m);
				}
				else if (name == "mesaMV") { // 2 tiles vertical
					Mesa* m = new Mesa(game, position, { 1, 3 }, { 1 , 2 }, name);
					m->setId(idCount);
					idCount++;
					getObjectManager()->addMueble(m);
				}
				else if (name == "mesaL") { // 4 tiles
					Mesa* m = new Mesa(game, position, { 2, 3 }, { 2 , 2 }, name);
					m->setId(idCount);
					idCount++;
					getObjectManager()->addMueble(m);
				}
				else if (name == "sillaDer") {
					Silla* s = new SillaE(game, position, name);
					s->setId(idCount);
					idCount++;
					getObjectManager()->addMueble(s);
				}
				else if (name == "sillaIz") {
					Silla* s = new SillaO(game, position, name);
					s->setId(idCount);
					idCount++;
					getObjectManager()->addMueble(s);
				}
				else if (name == "sillaAr") {
					Silla* s = new SillaS(game, position, name);
					s->setId(idCount);
					idCount++;
					getObjectManager()->addMueble(s);
				}
				else if (name == "sillaAb") {
					Silla* s = new SillaN(game, position, name);
					s->setId(idCount);
					idCount++;
					getObjectManager()->addMueble(s);
				}
				else if (name == "fogon") {
					Fogon* f = new Fogon(game, position);
					f->setId(idCount);
					idCount++;
					getObjectManager()->addMueble(f);
				}
				else if (name == "lavavajillas") {
					Lavavajillas* l = new Lavavajillas(game, position);
					l->setId(idCount);
					idCount++;
					getObjectManager()->addMueble(l);
				}
				else if (name == "cinta") {
					Cinta* c = new Cinta(game, position);
					SDL_Rect aux = c->getTexBox();
					if (p[0].getBoolValue()) {
						c->setCollider({ aux.x, aux.y + aux.h / 2 , aux.w, aux.h / 2 });
					}
					else c->setCollider(aux);
					getObjectManager()->addMueble(c);
				}
				else if (name == "inicioCinta") {
					InicioCinta* c = new InicioCinta(game, position);
					c->setVel(Vector2D<double>((double)p[1].getFloatValue(), (double)p[2].getFloatValue()));

					c->setId(idCount);
					idCount++;

					getObjectManager()->addMueble(c);
				}
				else if (name == "finalCinta") {
					FinalCinta* c = new FinalCinta(game, position);
					c->setId(idCount);
					idCount++;
					getObjectManager()->addMueble(c);
				}
				else if (name == "puerta") { 
					Puerta* puerta = new Puerta(game, position, p[3].getIntValue(), p[0].getIntValue());
					puerta->setVel(Vector2D<double>((double)p[1].getFloatValue(), (double)p[2].getFloatValue()));

					puerta->setId(idCount);
					idCount++;

					getObjectManager()->addMueble(puerta);
				}
				else if (name == "cartel") {
					Cartel* c = new Cartel(game, position);
					getObjectManager()->addMueble(c);
				}
				else if (name == "tabla") {
					TablaProcesado* t = new TablaProcesado(game, position);
					t->setId(idCount);
					idCount++;
					getObjectManager()->addMueble(t);
				}
				else if (name == "encimera") {
					Encimera* e = new Encimera(game, position);
					e->setId(idCount);
					idCount++;
					getObjectManager()->addMueble(e);
				}
				else if (name == "ventanilla") {
					Ventanilla* v = new Ventanilla(game, position, camara->renderRect());
					getObjectManager()->addMueble(v);
					getObjectManager()->addVentanilla(v);
				}
				else if (name == "arroz") {
					BolsaArroz* b = new BolsaArroz(game, position);
					b->setId(idCount);
					idCount++;
					getObjectManager()->addMueble(b);
				}
				else if (name == "pared") {
					Pared* p = new Pared(game, position);
					p->setDepth(-5);
					getObjectManager()->addMueble(p);
				}
				else if (name == "pilaS")
				{
					Pila* p = new Pila(game, position, TipoPaella::Pequena, 4);
					p->setId(idCount);
					idCount++;
					getObjectManager()->addMueble(p);
				}
				else if (name == "pilaM")
				{
					Pila* p = new Pila(game, position, TipoPaella::Mediana, 5);
					p->setId(idCount);
					idCount++;
					getObjectManager()->addMueble(p);
				}
				else if (name == "pilaL")
				{
					Pila* p = new Pila(game, position, TipoPaella::Grande, 3);
					p->setId(idCount);
					idCount++;
					getObjectManager()->addMueble(p);
				}
				else if (name == "cajaHerramientas")
				{
					CajaHerramientas* ch = new CajaHerramientas(game, position);
					ch->setId(idCount);
					idCount++;
					getObjectManager()->addMueble(ch);
				}

				
			}
		}
		// ordenar render
		getObjectManager()->sortAllGO();

		SDL_SetRenderTarget(renderer, nullptr);

		if (!sdlutils().images().count(path))
			sdlutils().images().emplace(path, Texture(renderer, fondo, mapInfo.anchoFondo, mapInfo.altoFondo));

	}
}

void Jornada::togglePause()
{
	uiManager->togglePause();

	paused = !paused;

	if (paused) {

		sdlutils().virtualTimer().pause();

		sdlutils().soundEffects().at("cancel").play(0, game->UI);
	}
	else {
		sdlutils().virtualTimer().resume();


		sdlutils().soundEffects().at("select").play(0, game->UI);
	}
}
