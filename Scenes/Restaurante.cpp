#include "Restaurante.h"
#include "../GameObjects/GrupoClientes.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Paella.h"
#include "../GameObjects/Ingrediente.h"
#include "../GameObjects/Muebles/Mueble.h"
#include "../sdlutils/SDLUtils.h"
#include "../Control/Game.h"
#include <iostream>

using namespace std;

Restaurante::Restaurante(Game* game)
{
	this->game = game;

	objectManager = new ObjectManager(game);

	host = new Player(game);

	//mapInfo.ruta = "../../../Assets/Tilemap/Restaurante.tmx";
	//fondo = new Fondo(game, loadMap(mapInfo.ruta));
}

Restaurante::~Restaurante()
{
	delete objectManager;
}

void Restaurante::handleInput()
{
	objectManager->handleInput();
	host->handleInput();
}

void Restaurante::update()
{
	objectManager->update();
	host->update();
}

void Restaurante::render()
{
	//fondo->render();
	objectManager->render();
	host->render();
}

ObjectManager* Restaurante::getObjectManager()
{
	return objectManager;
}

Texture* Restaurante::loadMap(string const path) {
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
	int anchoFondo = mapInfo.anchoTile * mapInfo.filas;
	int altoFondo = mapInfo.altoTile * mapInfo.columnas;
	SDL_Texture* fondo = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
		anchoFondo,
		altoFondo
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


					//bool is_wall = false; // Booleano de control
					//// Acceso a las propiedades de una tile dentro de un tileset (.tsx)
					//vector<tmx::Property> tile_props = mapInfo.tile_map.getTilesets()[tsx_file - 1].getTiles()[cur_gid].properties;
					//if (tile_props.size() > 0) {
					//	// Lo separo aqui por si en algun futuro creamos m�s propiedades, realmente habria que hacer una busqueda
					//	// de la propiedad y si esta en el vector usarla acorde
					//	if (tile_props[0].getName() == "wall")
					//		is_wall = tile_props[0].getBoolValue();
					//}

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
				auto aabb = obj.getAABB();
				/*
				if (obj.getName() == "collision") {
					auto collider = mngr_->addEntity();
					collider->setGroup<Wall_grp>(true);
					collider->addComponent<Transform>(Point2D(aabb.left, aabb.top), aabb.width, aabb.height);
					collider->addComponent<BoxCollider>(false, 0);
				}
				else if (obj.getName() == "ladder") {
					auto stair = mngr_->addEntity();
					stair->setGroup<Ladders_grp>(true);
					stair->addComponent<Transform>(Point2D(aabb.left, aabb.top), aabb.width, aabb.height);
					stair->addComponent<BoxCollider>(true, 0);
				}
				else if (obj.getName() == "playerSpawn") {
					new Player(mngr_, Point2D(aabb.left, aabb.top));
					auto camPos = Vector2D(aabb.left, aabb.top) + Vector2D(0, consts::CAMERA_MARGIN_FROM_PLAYER / Camera::mainCamera->getScale());
					Camera::mainCamera->MoveToPoint(camPos);
				}
				else if (obj.getName() == "loot") {
					Entity* interactableElement = mngr_->addEntity();
					interactableElement->addComponent<Transform>(Vector2D(aabb.left, aabb.top), aabb.width, aabb.height, 0);
					interactableElement->addComponent<Image>(&sdlutils().images().at("wardrobe"), 7, 2, 5, 0);
					mngr_->addRenderLayer<Loot>(interactableElement);
					interactableElement->addComponent<Loot>("Press E to open the loot", 5, 5);
					Loot* loot = interactableElement->getComponent<Loot>();

					vector<pair<ItemInfo*, Vector2D>> chestLoot = getGame()->SCENES_LOOT.find(getGame()->currentScene)->second[sceneLoots];
					for (int i = 0; i < chestLoot.size(); i++) {
						int count = 0;
						if (chestLoot[i].first->name() == CLASSIC_AMMO) count = 12;
						if (chestLoot[i].first->name() == LASER_AMMO) count = 5;
						if (chestLoot[i].first->name() == RICOCHET_AMMO) count = 6;
						ItemInfo infoAux = chestLoot[i].first;
						Vector2D pos = chestLoot[i].second;
						//	ItemInfo(ITEMS name, string strName, string description, int width, int height, int row, int col, std::function<void(Entity*)> function, int craftAmount = 0);
						loot->getInventory()->storeItem(
							new Item{ new ItemInfo(infoAux),
							mngr_,loot->getInventory() ,(int)pos.getX() ,(int)pos.getY() ,count });
					}
					sceneLoots++;
				}
				else if (obj.getName() == "enemy") {
					// int en objeto para identificar el tipo de enemigo
					int enemyType = obj.getProperties()[0].getIntValue();
					if (enemyType == 0)  // basico
						new DefaultEnemy(mngr_, Point2D(aabb.left, aabb.top));
					else if (enemyType == 1) // volador
						new FlyingEnemy(mngr_, Point2D(aabb.left, aabb.top));
				}
				else if (obj.getName() == "returnShelter") {
					Entity* returnToShelter = mngr_->addEntity();
					returnToShelter->addComponent<Transform>(Vector2D(aabb.left, aabb.top), aabb.width, aabb.height, 0);
					returnToShelter->addComponent<Image>(&sdlutils().images().at("back_to_shelter"), 1, 1, 0, 0);
					returnToShelter->addComponent<BackToShelter>(this);
					mngr_->addRenderLayer<Walls>(returnToShelter);
				}
				else if (obj.getName() == "returnShelterT") {
					Entity* returnToShelter = mngr_->addEntity();
					returnToShelter->addComponent<Transform>(Vector2D(aabb.left, aabb.top), aabb.width, aabb.height, 0);
					returnToShelter->addComponent<Image>(&sdlutils().images().at("back_to_shelter"), 1, 1, 0, 0);
					returnToShelter->addComponent<TutorialBackToShelter>(this);
					mngr_->addRenderLayer<Walls>(returnToShelter);
				}
				else if (obj.getName() == "InitialScene") {
					Entity* ini = mngr_->addEntity();
					ini->addComponent<Transform>(Vector2D(aabb.left, aabb.top));
					ini->setGroup<Initial_grp>(true);
				}
				else if (obj.getName() == "sleepStation") {
					auto interactable = mngr_->addEntity();
					static_cast<ShelterScene*>(this)->initSleepStation({ aabb.left, aabb.top }, { aabb.width, aabb.height }, interactable);
					interactable->addComponent<Transform>(Vector2D(aabb.left, aabb.top), aabb.width, aabb.height, 0);
					interactable->addComponent<InteractableElement>("Press E to SLEEP");
				}
				else if (obj.getName() == "workStation") {
					static_cast<ShelterScene*>(this)->initMechWs({ aabb.left, aabb.top }, { aabb.width, aabb.height });
					auto interactable = mngr_->addEntity();
					interactable->addComponent<Transform>(Vector2D(aabb.left, aabb.top), aabb.width, aabb.height, 0);
					interactable->addComponent<InteractableElement>("Press E to craft WAR ITEMS");
				}
				else if (obj.getName() == "medicalStation") {
					static_cast<ShelterScene*>(this)->initMedWs({ aabb.left, aabb.top }, { aabb.width, aabb.height });
					auto interactable = mngr_->addEntity();
					interactable->addComponent<Transform>(Vector2D(aabb.left, aabb.top), aabb.width, aabb.height, 0);
					interactable->addComponent<InteractableElement>("Press E to craft MEDICAL ITEMS");
				}
				else if (obj.getName() == "spaceShip") {
					static_cast<ShelterScene*>(this)->initSpaceshipStation({ aabb.left, aabb.top }, { aabb.width, aabb.height }, obj.getRotation());
					auto interactable = mngr_->addEntity();
					interactable->addComponent<Transform>(Vector2D(aabb.left, aabb.top), aabb.width, aabb.height);
					interactable->addComponent<InteractableElement>("Press E to see the SHIP STATE");
				}
			}*/
			}
		}

		SDL_SetRenderTarget(renderer, nullptr);

		//if (!sdlutils().images().count(path)) 
			//sdlutils().images().emplace(path, Texture(renderer, fondo, anchoFondo, altoFondo));
		/*auto backgroundEntity = mngr_->addEntity();
		mngr_->addRenderLayer<Background>(backgroundEntity);
		backgroundEntity->addComponent<Transform>(Vector2D(), anchoFondo, altoFondo);
		backgroundEntity->addComponent<Image>(&sdlutils().images().at(path));*/
		return &sdlutils().images().at(path);
	}
}