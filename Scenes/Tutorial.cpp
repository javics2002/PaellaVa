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


Tutorial::Tutorial(Game* mGame, string tilemap) : Scene(mGame)
{
	this->mGame = mGame;



	mMapInfo.mRuta = "Assets\\Tilemap\\" + tilemap + ".tmx";
	loadMap(mMapInfo.mRuta);

	mBackground->setTexture(mMapInfo.mRuta);
	mBackground->setPosition(mMapInfo.mAnchoFondo / 2, sdlutils().height() / 2);
	mBackground->setDimension(mMapInfo.mAnchoFondo, mMapInfo.mAltoFondo);

	Player* p = new Player(mGame, mPosicionCamarero.getX(), mPosicionCamarero.getY(), false);
	mObjectManager->addPlayer(p);

	// camara init
	mCamera = new Camera(*new Vector2D<float>(0, 16), sdlutils().width(), sdlutils().height());


	mUiManager->creaMenuPausa();

	mUiManager->addInterfaz(new Reloj(mGame, -1));
	mUiManager->addInterfaz(mCuadroDeDialogo);
	mUiManager->addInterfaz(mXboxBoton);
	mCuadroDeDialogo->setActive(false);
	mXboxBoton->setActive(false);
	mUiManager->addInterfaz(mTexto);
	mTexto->setActive(false);


	mObjectManager->initMuebles();

	changeState(TUTORIALSTATE_PAUSA_INICIO);

	mUiManager->addInterfaz(mRedactaComandaBotonTutorial);
	mUiManager->setRedactaboton(mRedactaComandaBotonTutorial);
	mUiManager->setBarra(mListaComandasTutorial);

	mRedactaComandaBotonTutorial->setActive(false);
	mListaComandasTutorial->setActive(false);
}

Tutorial::~Tutorial(){

}


void Tutorial::handleInput(bool& exit)
{
	Scene::handleInput(exit);

	if (ih().getKey(InputHandler::B)) 
		togglePause();
	if (ih().getKey(InputHandler::X) && mPaused && mTextMngr->TerminadoEscribir()) 
		pauseTutorial();
	if (ih().getKey(InputHandler::X) && mPaused && mTextMngr->terminadoParrado()) 
		mContinua = true;
	else if (ih().getKey(InputHandler::X) && mPaused && !mTextMngr->terminadoParrado() && !mTextMngr->vaRapido())
		mTextMngr->cambiaVelocidad(true);
	if (ih().getKey(InputHandler::Y) && mCurrentState==TUTORIALSTATE_ABRE_COMANDA) {
		if (mUiManager->getComanda() == nullptr)
			mUiManager->creaComanda(mGame);
		else 
			mUiManager->getComanda()->toggleactive();
		
	}
}

void Tutorial::update()
{
	if (!mPaused) {
		mObjectManager->update();

		if (mObjectManager->getPlayerOne()->getX() > mTamanoRestaurante.getY() + TILE_SIZE) { // tamRestaurante es un rango, no una posición, por eso tengo que hacer getY()
			mCamera->Lerp(Vector2D<float>(mTamanoRestaurante.getY(), 16), LERP_INTERPOLATION);
		}
		else if (mObjectManager->getPlayerOne()->getX() < mTamanoRestaurante.getY()) {
			mCamera->Lerp(Vector2D<float>(mTamanoRestaurante.getX(), 16), LERP_INTERPOLATION);
		}
	}
	mTextMngr->update();
	mUiManager->update(mPaused);
}

void Tutorial::changeState(States state_)
{
	anteriorEstado = mCurrentState;
	mCurrentState = state_;

	if (mCurrentState == TUTORIALSTATE_PAUSA_COMANDA_EQUIVOCADA  || mCurrentState % 2 != 0  //Si el estado actual es impar o cualquiera de esas pausas, pausas el juego
		|| mCurrentState==TUTORIALSTATE_PAUSA_BORRAR_COMANDA || mCurrentState==TUTORIALSTATE_PAUSA_NO_ECHAR_CLIENTES || mCurrentState==TUTORIALSTATE_PAUSA_INICIO)
	{
		if (mCurrentState == 13) {
			//Activas las cintas y haces que deje de funcionar en ese estado
			for (auto i : mObjectManager->getCintas())
				i->setActive(true);
			getObjectManager()->getIniCinta()->setActive(true);
			getObjectManager()->getIniCinta()->dejaFuncionar();
			getObjectManager()->getFinCinta()->setActive(true);

		}
		//Pausa
		pauseTutorial();
	}
	else switch (mCurrentState){ // Si no estamos en una pausa
		case 0: { // Activamos la puerta y el cartel para que entre nuestro cliente
			mObjectManager->getPuerta()->setActive(true);
			mObjectManager->getCartel()->setActive(true);
			break;
		}
		case 2:{ // Activamos las mesas y la silla para que el cliente se pueda sentar
			for (auto i : mObjectManager->getMesas())
				i->setActive(true);
			for (auto i : mObjectManager->getSillas())
				i->setActive(true);
			break;
		}
		case 4:{ // Permitimos al jugador que pueda tomar comandas
			mRedactaComandaBotonTutorial->setActive(true);
			mListaComandasTutorial->setActive(true);
			break;
		}
		case 8:{ // Ahora pasamos al rol de cocinera ya que vamos a hacer cosas en la cocina
			mObjectManager->getPlayerOne()->changePlayer(true);
			mCuadroDeDialogo->setTexture("cuadroTextoCamarero"); // Cambiamos el cuadro del texto para que ahora te hable el camarero
			// Activamos las encimeras y las pilas de paellas para que el jugador pueda usarlas
			for (auto i : mObjectManager->getEncimeras())
				i->setActive(true);
			for (auto i : mObjectManager->getPilas())
				i->setActive(true);
			break;
		}
		case 12:{ // Activamos la bolsa de arroz para ponerla en la paella
			getObjectManager()->getBolsa()->setActive(true);
			break;
		}
		case 14:{ // Activamos la caja de herramientas para que el jugador pueda arreglar la cinta rota
			mGame->getObjectManager()->getCajaHerramientas()->setActive(true);
			break;
		}
		case 18:{ // Una vez coge un ingredientes, se habilitan las tablas de procesado
			for (auto i : mObjectManager->getTablas())
				i->setActive(true);
			break;
		}
		case 22: { // Una vez hemos colocado todos los ingredientes, se activan los fogones para cocinar la paella
			for (auto i : mObjectManager->getFogones())
				i->setActive(true);
			break;
		}
		case 24: { // Activamos otras encimeras que haran de 'ventanillas' y volvemos a cambiar al jugador a camarero
			for (auto i : mObjectManager->getVentanilla())
				i->setActive(true);
			mObjectManager->getPlayerOne()->changePlayer(false);
			mCuadroDeDialogo->setTexture("cuadroTextoCocinera");
			break;
		}
		case 30: { // Activamos el lavavjillas para que el jugador deje la paella sucia y canmbiamos el rol a cocinera
			mObjectManager->getLavavajillas()->setActive(true);
			mCuadroDeDialogo->setTexture("cuadroTextoCamarero");
			mObjectManager->getPlayerOne()->changePlayer(true);
			break;
		}
		case TUTORIALSTATE_FINAL: {
			// Volvemos al menú
			mGame->getNetworkManager()->close();
			mGame->sendMessageScene(new Menu(mGame));
		}
		default:
			break;
	}
}

States Tutorial::getState() // Returnea el estado actual en el que se encuentra el jugador
{
	return mCurrentState;
}


void Tutorial::refresh() 
{
	mObjectManager->refresh();
}

void Tutorial::render() // Render
{
 	mBackground->render(mCamera->renderRect());
	mObjectManager->render(mCamera->renderRect());
	mUiManager->render(nullptr); // ponemos nullptr para que se mantenga en la pantalla siempre

	// Para ciertos casos necesitamos renderizar un texto, y para el resto de pausas, se van mostrando en orden
	if (mCurrentState == TUTORIALSTATE_PAUSA_COMANDA_EQUIVOCADA) {
		activaCuadro("textoComandaEquivocada");
	}
	else if (mCurrentState == TUTORIALSTATE_PAUSA_BORRAR_COMANDA) {
		activaCuadro("textoBorrarComanda");
	}
	else if (mCurrentState == TUTORIALSTATE_PAUSA_NO_ECHAR_CLIENTES) {
		activaCuadro("textoNoEcharClientes");
	}
	else if (mCurrentState == TUTORIALSTATE_PAUSA_INICIO)
		activaCuadro("texto0");
	else if (mCurrentState % 2 != 0) {
		activaCuadro(mTextosDialogos[mCurrentState/2]);
	}
	else if (mCurrentState % 2 == 0) { // Si no estamos en una pausa desactivamos el texto
		desactivaCuadro();
	}
	mTextMngr->render(); // Llamamos al render del TextManager
}


void Tutorial::loadMap(string const& path)
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


				//En algunos casos necesitamos meter los muebles en un vector a parte para poder activarlos secuencialmente con lo deseado
				// Desactivamos todos los muebles ya que los vamos a ir activando poco a poco
				if (name == "camarero") {
					mPosicionCamarero = position;
				}
				if (name == "cocinera") {
					mPosicionCocinera = position;
				}
				if (name == "mesaS") { // 1 tile
					Mesa* m = new Mesa(mGame, position, { 1, 2 }, { 1 , 1 }, name);
					m->setTiles(21, 1);
					getObjectManager()->addMueble(m);
				}
				else if (name == "mesaMH") { // 2 tiles horizontal
					Mesa* m = new Mesa(mGame, position, { 2, 2 }, { 2 , 1 }, name);
					m->setTiles(22, 1);
					getObjectManager()->addMueble(m);
				}
				else if (name == "mesaMV") { // 2 tiles vertical
					Mesa* m = new Mesa(mGame, position, { 1, 3 }, { 1 , 2 }, name);
					m->setTiles(38, 0.85);
					getObjectManager()->addMueble(m);
				}
				else if (name == "mesaL") { // 4 tiles
					Mesa* m = new Mesa(mGame, position, { 2, 3 }, { 2 , 2 }, name);
					m->setTiles(36, 0.85);
					getObjectManager()->addMueble(m);
				}
				else if (name == "sillaIz") {
					Silla* s = new SillaE(mGame, position, name);
					getObjectManager()->addMueble(s);
					getObjectManager()->addSilla(s);
					s->setActive(false);
				}
				else if (name == "sillaIz") {
					Silla* s = new SillaO(mGame, position, name);;
					getObjectManager()->addMueble(s);
					getObjectManager()->addSilla(s);
					s->setActive(false);
				}
				else if (name == "sillaAr") {
					Silla* s = new SillaS(mGame, position, name);;
					getObjectManager()->addMueble(s);
					getObjectManager()->addSilla(s);
					s->setActive(false);
				}
				else if (name == "sillaAb") {
					Silla* s = new SillaN(mGame, position, name);
					getObjectManager()->addMueble(s);
					getObjectManager()->addSilla(s);
					s->setActive(false);
				}
				else if (name == "fogon") {
					Fogon* f = new Fogon(mGame, position);
					getObjectManager()->addMueble(f);
					getObjectManager()->addFogon(f);
					f->setActive(false);

				}
				else if (name == "vEncimera") {
					Encimera* e = new Encimera(mGame, position);
					getObjectManager()->addMueble(e);
					getObjectManager()->addVentanilla(e);
					e->setActive(false);
				}
				else if (name == "lavavajillas") {
					Lavavajillas* l = new Lavavajillas(mGame, position);
					getObjectManager()->addMueble(l);
					getObjectManager()->addLavavajillas(l);
					l->setActive(false);

				}
				else if (name == "cinta") {
					Cinta* c = new Cinta(mGame, position);
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
					InicioCinta* c = new InicioCinta(mGame, position);
					c->setVel(Vector2D<double>((double)p[1].getFloatValue(), (double)p[2].getFloatValue()));
					getObjectManager()->addMueble(c);
					getObjectManager()->addIniCinta(c);
					c->setActive(false);
				}
				else if (name == "finalCinta") {
					FinalCinta* c = new FinalCinta(mGame, position);
					getObjectManager()->addMueble(c);
					getObjectManager()->addFinCinta(c);
					c->setActive(false);
				}
				else if (name == "puerta") {
					Puerta* puerta = new Puerta(mGame, position, p[3].getIntValue(), p[0].getIntValue());
					puerta->setVel(Vector2D<double>((double)p[1].getFloatValue(), (double)p[2].getFloatValue()));
					getObjectManager()->addMueble(puerta);
					getObjectManager()->addPuerta(puerta);
					puerta->setActive(false);
				}
				else if (name == "cartel") {
					Cartel* c = new Cartel(mGame, position);
					getObjectManager()->addMueble(c);
					getObjectManager()->addCartel(c);
					c->setActive(false);
				}
				else if (name == "tabla") {
					TablaProcesado* t = new TablaProcesado(mGame, position);
					getObjectManager()->addMueble(t);
					getObjectManager()->addTabla(t);
					t->setActive(false);
				}
				else if (name == "encimera") {
					Encimera* e = new Encimera(mGame, position);
					getObjectManager()->addMueble(e);
					getObjectManager()->addEncimera(e);
					e->setActive(false);
				}				
				else if (name == "arroz") {
					BolsaArroz* b = new BolsaArroz(mGame, position);
					getObjectManager()->addMueble(b);
					getObjectManager()->addBolsa(b);
					b->setActive(false);
				}
				else if (name == "pared") {
					Pared* p = new Pared(mGame, position);
					p->setDepth(-5);
					getObjectManager()->addMueble(p);
				}
				else if (name == "pilaS")
				{
					Pila* p = new Pila(mGame, position, TipoPaella::Pequena, 1);
					getObjectManager()->addMueble(p);
					getObjectManager()->addPilas(p);
					p->setActive(false);
				}
				else if (name == "pilaM")
				{
					Pila* p = new Pila(mGame, position, TipoPaella::Mediana, 1);
					getObjectManager()->addMueble(p);
					getObjectManager()->addPilas(p);
					p->setActive(false);
				}
				else if (name == "pilaL")
				{
					Pila* p = new Pila(mGame, position, TipoPaella::Grande, 1);
					getObjectManager()->addMueble(p);
					getObjectManager()->addPilas(p);
				p->setActive(false);
				}
				else if (name == "cajaHerramientas")
				{
					CajaHerramientas* p = new CajaHerramientas(mGame, position);
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
			sdlutils().images().emplace(path, Texture(renderer, fondo, mMapInfo.mAnchoFondo, mMapInfo.mAltoFondo));
	}
}

void Tutorial::togglePause()
{
	mUiManager->togglePause(); // Pausamos el juego cuando hay un dialogo o si hay una pausa

	if (mCurrentState % 2 == 0)mPaused = !mPaused;
	else mTextMngr->cambiaPausa(); // Si se pausa el juego y estamos en un dialogo, paramos el dialogo

	if (mPaused) {

		sdlutils().virtualTimer().pause();

		sdlutils().soundEffects().at("cancel").play(0, mGame->UI);
	}
	else {
		sdlutils().virtualTimer().resume();

		sdlutils().soundEffects().at("select").play(0, mGame->UI);
	}
}

void Tutorial::pauseTutorial()
{
	// Pausa para los dialogos
	mPaused = !mPaused;

	if (mPaused) {

		sdlutils().virtualTimer().pause();
		sdlutils().soundEffects().at("cancel").play(0, mGame->UI);
	}
	else { // En funcion del estado, pasamos a un estado pero volvemos al anterior
		sdlutils().virtualTimer().resume();
		if (mCurrentState == TUTORIALSTATE_PAUSA_COMANDA_EQUIVOCADA)
			mCurrentState = anteriorEstado;
		else if (mCurrentState == TUTORIALSTATE_PAUSA_BORRAR_COMANDA)
			mCurrentState = anteriorEstado;
		else if (mCurrentState == TUTORIALSTATE_PAUSA_NO_ECHAR_CLIENTES)
			mCurrentState = anteriorEstado;
		else if (mCurrentState == TUTORIALSTATE_PAUSA_INICIO)
			mCurrentState = TUTORIALSTATE_COGER_CLIENTES;
		else mCurrentState = (States)(mCurrentState + 1);
		changeState(mCurrentState);
		sdlutils().soundEffects().at("select").play(0, mGame->UI);
	}
}

void Tutorial::nextStates() // Pasamos al siguiente estado
{
	mCurrentState =(States) (mCurrentState + 1);
}

void Tutorial::desactivaCuadro() // Desactivamos el cuadro de texto que haya en pantalla
{
	mTextMngr->desactivaTexto();
	mXboxBoton->setActive(false);
	mTextMngr->cambiaVelocidad(false);
	mCuadroDeDialogo->setActive(false);
	mTexto->setActive(false);
}


void Tutorial::activaCuadro(string texto_) // Activamos el cuadro de texto y empezamos a escribir el dialogo
										   // Si estamos jugando con mando renderizamos un boton para indicar como continuar
{
	mCuadroDeDialogo->setActive(true);
	if(ih().isMandoActive())
		mXboxBoton->setActive(true);
	if (mTextMngr->desactivado())mTextMngr->activaTexto(sdlutils().dialogs().at(texto_));
	else if (mTextMngr->terminadoParrado() && !mTextMngr->esUltimoParrafo() && mContinua) {
		mTextMngr->cambiaVelocidad(false);
		mTextMngr->reiniciaParrafo();
		mContinua = false;
	}
}
