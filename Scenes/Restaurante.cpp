#include "Restaurante.h"
#include "../GameObjects/GrupoClientes.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Paella.h"
#include "../GameObjects/Ingrediente.h"
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

Restaurante::Restaurante(Game* game) : Scene(game)
{
	this->game = game;

	mapInfo.ruta = "..\\..\\..\\Assets\\Tilemap\\Restaurante.tmx";
	loadMap(mapInfo.ruta);

	fondo->setTexture(mapInfo.ruta);
	fondo->setPosition(mapInfo.anchoFondo / 2, sdlutils().height() / 2);
	fondo->setDimension(mapInfo.anchoFondo, mapInfo.altoFondo);

	// camara init
	camara = new Camera(*new Vector2D<float>(0, 16), sdlutils().width(), sdlutils().height());

	uiManager->addInterfaz(new RedactaComandabutton(game, uiManager, "redactaboton", 10, 10, 30, 30));
	uiManager->setBarra(new ListaComandas(game,uiManager));

	uiManager->creaMenuPausa();

	objectManager->addPaella(new Paella(game, TipoPaella::Minima));
	objectManager->addPaella(new Paella(game, TipoPaella::Minima));
	objectManager->addPaella(new Paella(game, TipoPaella::Minima));
	uiManager->addInterfaz(new Reloj(game));

	objectManager->initMuebles();
}

Restaurante::~Restaurante()
{
	delete objectManager;
	delete fondo;
	delete camara;
}

void Restaurante::handleInput(bool& exit)
{
	Scene::handleInput(exit);

	if (ih().getKey(InputHandler::CANCEL)) {
#ifdef _DEBUG
		// game->changeScene(new GameOver(game, 100));
		togglePause();
#else
		//Abrir menú de pausa
		togglePause();
#endif // _DEBUG
	}
}

void Restaurante::update()
{
	if (!paused) {
		objectManager->update();
		
		if (objectManager->getHost()->getX() > tamRestaurante.getY() + TILE_SIZE) { // tamRestaurante es un rango, no una posición, por eso tengo que hacer getY()
			camara->Lerp(Vector2D<float>(tamRestaurante.getY(), 16), LERP_INTERPOLATION);
		}
		else if (objectManager->getHost()->getX() < tamRestaurante.getY()) {
			camara->Lerp(Vector2D<float>(tamRestaurante.getX(), 16), LERP_INTERPOLATION);
		}
	}

	uiManager->update();
}

void Restaurante::render()
{

	fondo->render(camara->renderRect());
	objectManager->render(camara->renderRect());
	
	uiManager->render(nullptr); // ponemos nullptr para que se mantenga en la pantalla siempre
}

void Restaurante::debug()
{
	fondo->renderDebug(camara->renderRect());
	objectManager->debug(camara->renderRect());
}

void Restaurante::addPuntuaciones(double puntosComanda)
{
	puntuacionesComandas.push_back(puntosComanda);
}

void Restaurante::mediaPuntuaciones()
{
	int sumaMedia = 0;
	for (auto i : puntuacionesComandas) {
		sumaMedia+=i;
	}
	puntuaciónTotal = sumaMedia / puntuacionesComandas.size();
}

void Restaurante::togglePause()
{
	uiManager->togglePause();

	paused = !paused;

	if (paused) {
		Mueble* m;
		double offsetM = 0;
		for (int i = 0u; i < objectManager->getMuebles().size(); i++) {
			m = dynamic_cast<Mueble*>(objectManager->getMuebles()[i]);
			offsetM = SDL_GetTicks() - m->getTime();

			m->setOffset(offsetM);
		}

		sdlutils().soundEffects().at("cancel").play(0, game->UI);
	}
	else {
		Mueble* m;
		for (int i = 0u; i < objectManager->getMuebles().size(); i++) {
			m = dynamic_cast<Mueble*>(objectManager->getMuebles()[i]);

			m->setTime(SDL_GetTicks());
		}

		sdlutils().soundEffects().at("select").play(0, game->UI);
	}
}

void Restaurante::loadMap(string const& path) {
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
				
				if (name == "mesaS") { // 1 tile
					Mesa* m = new Mesa(game, position, { 1, 2 }, name);
					m->setDepth(1);
					getObjectManager()->addMueble(m);
				}
				else if (name == "mesaMH") { // 2 tiles horizontal
					Mesa* m = new Mesa(game, position, { 2, 2 }, name);
					m->setDepth(1);
					getObjectManager()->addMueble(m);
				}
				else if (name == "mesaMV") { // 2 tiles vertical
					Mesa* m = new Mesa(game, position, { 1, 3 }, name);
					m->setDepth(1);
					getObjectManager()->addMueble(m);
				}
				else if (name == "mesaL") { // 4 tiles
					Mesa* m = new Mesa(game, position, { 2, 4 }, name);
					m->setDepth(1);
					getObjectManager()->addMueble(m);
				}
				else if (name == "sillaIz" || name == "sillaDer") {
					Silla* s = new Silla(game, position, name);
					s->setDepth(1);
					getObjectManager()->addMueble(s);
				}
				else if (name == "sillaAr") {
					Silla* s = new Silla(game, position, name);
					s->setDepth(0);
					getObjectManager()->addMueble(s);
				}
				else if (name == "sillaAb") {
					Silla* s = new Silla(game, position, name);
					s->setDepth(2);
					getObjectManager()->addMueble(s);
				}
				else if (name == "fogon") {
					Fogon* f = new Fogon(game, position);
					f->setDepth(1);
					getObjectManager()->addMueble(f);
				}
				else if (name == "lavavajillas") {
					Lavavajillas* l = new Lavavajillas(game, position);
					l->setDepth(1);
					getObjectManager()->addMueble(l);
				}
				else if (name == "cinta") {
					Cinta* c = new Cinta(game, position);
					c->setDepth(1);
					getObjectManager()->addMueble(c);
				}
				else if (name == "inicioCinta") {
					InicioCinta* c = new InicioCinta(game, position);
					c->setDepth(1);
					getObjectManager()->addMueble(c);
				}
				else if (name == "finalCinta") {
					FinalCinta* c = new FinalCinta(game, position);
					c->setDepth(1);
					getObjectManager()->addMueble(c);
				}
				else if (name == "puerta") {
					Puerta* p = new Puerta(game, position);
					p->setDepth(1);
					getObjectManager()->addMueble(p);
				}
				else if (name == "cartel") {
					Cartel* c = new Cartel(game, position);
					c->setDepth(1);
					getObjectManager()->addMueble(c);
				}
				else if (name == "tabla") {
					TablaProcesado* t = new TablaProcesado(game, position);
					t->setDepth(1);
					getObjectManager()->addMueble(t);
				}
				else if (name == "encimera") {
					Encimera* e = new Encimera(game, position);
					e->setDepth(1);
					getObjectManager()->addMueble(e);
				}
				else if (name == "ventanilla") {
					Ventanilla* v = new Ventanilla(game, position, camara->renderRect());
					v->setDepth(1);
					getObjectManager()->addMueble(v);
				}
				else if (name == "arroz") {
					BolsaArroz* b = new BolsaArroz(game, position);
					b->setDepth(1);
					getObjectManager()->addMueble(b);
				}
				else if (name == "pared") {
					Pared* p = new Pared(game, position);
					p->setDepth(-5);
					getObjectManager()->addMueble(p);
				}
			}
		}
		// ordenar renderstatic
		getObjectManager()->sortRenderStatic();

		SDL_SetRenderTarget(renderer, nullptr);

		if (!sdlutils().images().count(path)) 
			sdlutils().images().emplace(path, Texture(renderer, fondo, mapInfo.anchoFondo, mapInfo.altoFondo));
	}
}