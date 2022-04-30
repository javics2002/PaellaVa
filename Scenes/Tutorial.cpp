#include "Tutorial.h"
#include "../GameObjects/GrupoClientes.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Paella.h"
#include "../GameObjects/Ingrediente.h"
#include "../GameObjects/Muebles/Pila.h"
#include "../GameObjects/Muebles/MueblesInclude.h"
#include "../sdlutils/SDLUtils.h"
#include "../Control/Game.h"
#include "../GameObjects/UI/Reloj.h"
#include "../Scenes/GameOver.h"
#include "../Scenes/Menu.h"

#include <iostream>

using namespace std;


Tutorial::Tutorial(Game* game, string tilemap) : Scene(game)
{
	this->game = game;



	mapInfo.ruta = "Assets\\Tilemap\\" + tilemap + ".tmx";
	loadMap(mapInfo.ruta);

	fondo->setTexture(mapInfo.ruta);
	fondo->setPosition(mapInfo.anchoFondo / 2, sdlutils().height() / 2);
	fondo->setDimension(mapInfo.anchoFondo, mapInfo.altoFondo);

	Player* p = new Player(game, positionCamarero.getX(), positionCamarero.getY(), false);
	objectManager->addPlayer(p);

	// camara init
	camara = new Camera(*new Vector2D<float>(0, 16), sdlutils().width(), sdlutils().height());


	uiManager->creaMenuPausa();

	uiManager->addInterfaz(new Reloj(game, -1));
	uiManager->addInterfaz(cuadroTexto);
	cuadroTexto->setActive(false);
	uiManager->addInterfaz(text);
	text->setActive(false);


	objectManager->initMuebles();

	changeState(pausaInicio);

	uiManager->addInterfaz(rC);
	uiManager->setBarra(lC);
	rC->setActive(false);
	lC->setActive(false);
}

Tutorial::~Tutorial(){

}


void Tutorial::handleInput(bool& exit)
{
	Scene::handleInput(exit);

	if (ih().getKey(InputHandler::B)) 
		togglePause();
	if (ih().getKey(InputHandler::X) && paused && textMngr->TerminadoEscribir()) 
		pauseTutorial();
	if (ih().getKey(InputHandler::X) && paused && textMngr->terminadoParrado()) 
		continua = true;
	else if (ih().getKey(InputHandler::X) && paused && !textMngr->terminadoParrado() && !textMngr->vaRapido())
		textMngr->cambiaVelocidad(true);
}

void Tutorial::update()
{
	if (!paused) {
		objectManager->update();

		if (objectManager->getPlayerOne()->getX() > tamRestaurante.getY() + TILE_SIZE) { // tamRestaurante es un rango, no una posición, por eso tengo que hacer getY()
			camara->Lerp(Vector2D<float>(tamRestaurante.getY(), 16), LERP_INTERPOLATION);
		}
		else if (objectManager->getPlayerOne()->getX() < tamRestaurante.getY()) {
			camara->Lerp(Vector2D<float>(tamRestaurante.getX(), 16), LERP_INTERPOLATION);
		}
	}
	textMngr->update();
	uiManager->update(paused);
}

void Tutorial::changeState(States state_)
{
	anteriorEstado = currentState;
	currentState = state_;

	if (currentState == pausaComandaEquivocada  || currentState % 2 != 0 
		|| currentState==pausaBorrarComanda || currentState==pausaNoEcharClientes || currentState==pausaInicio)
	{
		if (currentState == 13) {
			for (auto i : objectManager->getCintas())
				i->setActive(true);
			getObjectManager()->getIniCinta()->setActive(true);
			getObjectManager()->getIniCinta()->dejaFuncionar();
			getObjectManager()->getFinCinta()->setActive(true);

		}

		pauseTutorial();
	}
	else switch (currentState){
		case 0: {
			objectManager->getPuerta()->setActive(true);
			objectManager->getCartel()->setActive(true);
			break;
		}
		case 2:{
			for (auto i : objectManager->getMesas())
				i->setActive(true);
			for (auto i : objectManager->getSillas())
				i->setActive(true);
			break;
		}
		case 4:{		
			rC->setActive(true);
			lC->setActive(true);
			break;
		}
		case 8:{
			objectManager->getPlayerOne()->changePlayer(true);
			cuadroTexto->setTexture("cuadroTextoCamarero");
			for (auto i : objectManager->getEncimeras())
				i->setActive(true);
			for (auto i : objectManager->getPilas())
				i->setActive(true);
			break;
		}
		case 12:{
			getObjectManager()->getBolsa()->setActive(true);
			break;
		}
		case 14:{
			game->getObjectManager()->getCajaHerramientas()->setActive(true);
			break;
		}
		case 18:{
			for (auto i : objectManager->getTablas())
				i->setActive(true);
			break;
		}
		case 22: {
			for (auto i : objectManager->getFogones())
				i->setActive(true);
			break;
		}
		case 24: {
			for (auto i : objectManager->getVentanilla())
				i->setActive(true);
			break;
		}
		case 28: {
			cuadroTexto->setTexture("cuadroTextoCocinera");
			break;
		}
		case 36: {
			objectManager->getLavavajillas()->setActive(true);
			cuadroTexto->setTexture("cuadroTextoCamarero");
			break;
		}
		case 40: {
			objectManager->getPlayerOne()->changePlayer(true);
			cuadroTexto->setTexture("cuadroTextoCocinera");
			break;
		}
		case final: {
			
			game->getNetworkManager()->close();
			game->sendMessageScene(new Menu(game));
		}
		default:
			break;
	}
}

States Tutorial::getState()
{
	return currentState;
}


void Tutorial::refresh()
{
	objectManager->refresh();
}

void Tutorial::render()
{
 	fondo->render(camara->renderRect());
	objectManager->render(camara->renderRect());
	uiManager->render(nullptr); // ponemos nullptr para que se mantenga en la pantalla siempre
	if (currentState == pausaComandaEquivocada) {
		activaCuadro("textoComandaEquivocada");
	}
	else if (currentState == pausaBorrarComanda) {
		activaCuadro("textoBorrarComanda");
	}
	else if (currentState == pausaNoEcharClientes) {
		activaCuadro("textoNoEcharClientes");
	}
	else if (currentState == pausaInicio)
		activaCuadro("texto0");
	else if (currentState % 2 != 0) {
		activaCuadro(textos[currentState/2]);
	}
	else if (currentState % 2 == 0) {
		desactivaCuadro();
	}
	textMngr->render();
}


void Tutorial::loadMap(string const& path)
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
					getObjectManager()->addMueble(m);
					getObjectManager()->addMesa(m);
					m->setActive(false);
				}
				else if (name == "mesaMH") { // 2 tiles horizontal
					Mesa* m = new Mesa(game, position, { 2, 2 }, { 2 , 1 }, name);
					getObjectManager()->addMueble(m);
					getObjectManager()->addMesa(m);
					m->setActive(false);
				}
				else if (name == "mesaMV") { // 2 tiles vertical
					Mesa* m = new Mesa(game, position, { 1, 3 }, { 1 , 2 }, name);
					getObjectManager()->addMueble(m);
					getObjectManager()->addMesa(m);
					m->setActive(false);
				}
				else if (name == "mesaL") { // 4 tiles
					Mesa* m = new Mesa(game, position, { 2, 4 }, { 2 , 2 }, name);
					getObjectManager()->addMueble(m);
					getObjectManager()->addMesa(m);
					m->setActive(false);
				}
				else if (name == "sillaIz" || name == "sillaDer") {
					Silla* s = new Silla(game, position, name, +1);
					getObjectManager()->addMueble(s);
					getObjectManager()->addSilla(s);
					s->setActive(false);
				}
				else if (name == "sillaAr") {
					Silla* s = new Silla(game, position, name, -1);;
					getObjectManager()->addMueble(s);
					getObjectManager()->addSilla(s);
					s->setActive(false);
				}
				else if (name == "sillaAb") {
					Silla* s = new Silla(game, position, name, +1);
					getObjectManager()->addMueble(s);
					getObjectManager()->addSilla(s);
					s->setActive(false);
				}
				else if (name == "fogon") {
					Fogon* f = new Fogon(game, position);
					getObjectManager()->addMueble(f);
					getObjectManager()->addFogon(f);
					f->setActive(false);

				}
				else if (name == "lavavajillas") {
					Lavavajillas* l = new Lavavajillas(game, position);
					getObjectManager()->addMueble(l);
					getObjectManager()->addLavavajillas(l);
					l->setActive(false);

				}
				else if (name == "cinta") {
					Cinta* c = new Cinta(game, position);
					SDL_Rect aux = c->getTexBox();
					if (p[0].getBoolValue()) {
						c->setCollider({ aux.x, aux.y + aux.h / 2 , aux.w, aux.h / 2 });
					}
					else c->setCollider(aux);
					getObjectManager()->addMueble(c);
					getObjectManager()->addCinta(c);
					c->setActive(false);
				}
				else if (name == "inicioCinta") {
					InicioCinta* c = new InicioCinta(game, position);
					c->setVel(Vector2D<double>((double)p[1].getFloatValue(), (double)p[2].getFloatValue()));
					getObjectManager()->addMueble(c);
					getObjectManager()->addIniCinta(c);
					c->setActive(false);
				}
				else if (name == "finalCinta") {
					FinalCinta* c = new FinalCinta(game, position);
					getObjectManager()->addMueble(c);
					getObjectManager()->addFinCinta(c);
					c->setActive(false);
				}
				else if (name == "puerta") {
					Puerta* puerta = new Puerta(game, position, p[3].getIntValue(), p[0].getIntValue());
					puerta->setVel(Vector2D<double>((double)p[1].getFloatValue(), (double)p[2].getFloatValue()));
					getObjectManager()->addMueble(puerta);
					getObjectManager()->addPuerta(puerta);
					puerta->setActive(false);
				}
				else if (name == "cartel") {
					Cartel* c = new Cartel(game, position);
					getObjectManager()->addMueble(c);
					getObjectManager()->addCartel(c);
					c->setActive(false);
				}
				else if (name == "tabla") {
					TablaProcesado* t = new TablaProcesado(game, position);
					getObjectManager()->addMueble(t);
					getObjectManager()->addTabla(t);
					t->setActive(false);
				}
				else if (name == "encimera") {
					Encimera* e = new Encimera(game, position);
					getObjectManager()->addMueble(e);
					getObjectManager()->addEncimera(e);
					e->setActive(false);
				}
				else if (name == "ventanilla") {
					Ventanilla* v = new Ventanilla(game, position, camara->renderRect());
					getObjectManager()->addMueble(v);
					getObjectManager()->addVentanilla(v);
					v->setActive(false);
				}
				else if (name == "arroz") {
					BolsaArroz* b = new BolsaArroz(game, position);
					getObjectManager()->addMueble(b);
					getObjectManager()->addBolsa(b);
					b->setActive(false);
				}
				else if (name == "pared") {
					Pared* p = new Pared(game, position);
					p->setDepth(-5);
					getObjectManager()->addMueble(p);
				}
				else if (name == "pilaS")
				{
					Pila* p = new Pila(game, position, TipoPaella::Pequena, 1);
					getObjectManager()->addMueble(p);
					getObjectManager()->addPilas(p);
					p->setActive(false);
				}
				else if (name == "pilaM")
				{
					Pila* p = new Pila(game, position, TipoPaella::Mediana, 1);
					getObjectManager()->addMueble(p);
					getObjectManager()->addPilas(p);
					p->setActive(false);
				}
				else if (name == "pilaL")
				{
					Pila* p = new Pila(game, position, TipoPaella::Grande, 1);
					getObjectManager()->addMueble(p);
					getObjectManager()->addPilas(p);
					p->setActive(false);
				}
				else if (name == "cajaHerramientas")
				{
					CajaHerramientas* p = new CajaHerramientas(game, position);
					getObjectManager()->addMueble(p);
					getObjectManager()->addCaja(p);
					p->setActive(false);
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

void Tutorial::togglePause()
{
	uiManager->togglePause();

	if (currentState % 2 == 0)paused = !paused;
	else textMngr->cambiaPausa();

	if (paused) {

		sdlutils().virtualTimer().pause();

		sdlutils().soundEffects().at("cancel").play(0, game->UI);
	}
	else {
		sdlutils().virtualTimer().resume();

		sdlutils().soundEffects().at("select").play(0, game->UI);
	}
}

void Tutorial::pauseTutorial()
{

	paused = !paused;

	if (paused) {

		sdlutils().virtualTimer().pause();
		sdlutils().soundEffects().at("cancel").play(0, game->UI);
	}
	else {
		sdlutils().virtualTimer().resume();
		if (currentState == pausaComandaEquivocada)
			currentState = anteriorEstado;
		else if (currentState == pausaBorrarComanda)
			currentState = anteriorEstado;
		else if (currentState == pausaNoEcharClientes)
			currentState = anteriorEstado;
		else if (currentState == pausaInicio)
			currentState = cogerClientes;
		else currentState = (States)(currentState + 1);
		changeState(currentState);
		sdlutils().soundEffects().at("select").play(0, game->UI);
	}
}

void Tutorial::nextStates()
{
	currentState =(States) (currentState + 1);
}

void Tutorial::desactivaCuadro()
{
	textMngr->desactivaTexto();
	textMngr->cambiaVelocidad(false);
	cuadroTexto->setActive(false);
	text->setActive(false);
}


void Tutorial::activaCuadro(string texto_)
{
	cuadroTexto->setActive(true);
	if (textMngr->desactivado())textMngr->activaTexto(sdlutils().dialogs().at(texto_));
	else if (textMngr->terminadoParrado() && !textMngr->esUltimoParrafo() && continua) {
		textMngr->cambiaVelocidad(false);
		textMngr->reiniciaParrafo();
		continua = false;
	}
}
