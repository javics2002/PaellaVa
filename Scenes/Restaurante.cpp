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
#include <iostream>

using namespace std;

Restaurante::Restaurante(Game* game) : Scene(game)
{
	this->game = game;

	host = new Player(game);

	mapInfo.ruta = "..\\..\\..\\Assets\\Tilemap\\Restaurante.tmx";
	loadMap(mapInfo.ruta);

	fondo->setTexture(mapInfo.ruta);
	fondo->setPosition(mapInfo.anchoFondo / 2, sdlutils().height() / 2);
	fondo->setDimension(mapInfo.anchoFondo, mapInfo.altoFondo);

	// camara init
	camara = new Camera(*new Vector2D<float>(0, 16), sdlutils().width(), sdlutils().height());

	getUIManager()->addInterfaz(new RedactaComandabutton(game, "redactaboton", 10, 10, 30, 30));
	uiManager->setBarra(new ListaComandas(game));

	objectManager->addPaella(new Paella(game, Paella::Minima));

	//objectManager->initMuebles();
}

Restaurante::~Restaurante()
{
	delete host;
	delete objectManager;
	delete fondo;
	delete camara;
}

void Restaurante::handleInput()
{
	objectManager->handleInput();
	host->handleInput();
}

void Restaurante::update()
{
	objectManager->update();
	uiManager->update();
	host->update();

	if (host->getX() > tamRestaurante.getY() + TILE_SIZE) { // tamRestaurante es un rango, no una posición, por eso tengo que hacer getY()
		camara->Lerp(Vector2D<float>(tamRestaurante.getY(), 16), LERP_INTERPOLATION);
	}
	else if (host->getX() < tamRestaurante.getY()) {
		camara->Lerp(Vector2D<float>(tamRestaurante.getX(), 16), LERP_INTERPOLATION);
	}
}

void Restaurante::render()
{
	fondo->render(camara->renderRect());
	objectManager->render(camara->renderRect());
	
	host->render(camara->renderRect());
	uiManager->render(nullptr); // ponemos nullptr para que se mantenga en la pantalla siempre

	//auto time = sdlutils().currRealTime();

	//auto time_ = to_string(time);

	//Texture* tex;

	//tex->constructFromText(camara->renderRect(), time_, );
}

void Restaurante::debug()
{
	fondo->renderDebug(camara->renderRect());
	objectManager->debug(camara->renderRect());
	host->renderDebug(camara->renderRect());
}

//Check colisiones

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

				if (name == "mesaS") {
					getObjectManager()->addMueble(new Mesa(game, position, { 1, 2 }, name));
				}
				else if (name == "mesaMH") {
					getObjectManager()->addMueble(new Mesa(game, position, { 2, 2 }, name));
				}
				else if (name == "mesaMV") {
					getObjectManager()->addMueble(new Mesa(game, position, { 1, 3 }, name));
				}
				else if (name == "mesaL") {
					getObjectManager()->addMueble(new Mesa(game, position, { 2, 4 }, name));
				}
				else if (name == "sillaIz" || name == "sillaDer" || name == "sillaAr" || name == "sillaAb") {
					getObjectManager()->addMueble(new Silla(game, position, name));
				}
				else if (name == "fogon") {
					getObjectManager()->addMueble(new Fogon(game, position));
				}
				else if (name == "lavavajillas") {
					getObjectManager()->addMueble(new Lavavajillas(game, position));
				}
				else if (name == "cinta") {
					getObjectManager()->addMueble(new Cinta(game, position));
				}
				else if (name == "inicioCinta") {
					getObjectManager()->addMueble(new InicioCinta(game, position));
				}
				else if (name == "finalCinta") {
					getObjectManager()->addMueble(new FinalCinta(game, position));
				}
				else if (name == "puerta") {
					getObjectManager()->addMueble(new Puerta(game, position));
				}
				else if (name == "cartel") {
					getObjectManager()->addMueble(new Cartel(game, position));
				}
				else if (name == "tabla") {
					getObjectManager()->addMueble(new TablaProcesado(game, position));
				}
				else if (name == "encimera") {
					getObjectManager()->addMueble(new Encimera(game, position));
				}
				else if (name == "ventanilla") {
					getObjectManager()->addMueble(new Ventanilla(game, position));
				}
			}
		}

		SDL_SetRenderTarget(renderer, nullptr);

		if (!sdlutils().images().count(path)) 
			sdlutils().images().emplace(path, Texture(renderer, fondo, mapInfo.anchoFondo, mapInfo.altoFondo));
	}
}