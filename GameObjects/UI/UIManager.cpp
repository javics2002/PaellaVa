#include "UIManager.h"
#include "../../Data/Comanda.h"
#include "../../GameObjects/UI/RedactaComandabutton.h"
#include "../../sdlutils/InputHandler.h"
#include "../UI/Ingredientebutton.h"
#include "../UI/Numerobutton.h"
#include "../UI/Tamanobutton.h"
#include "../UI/RedactaComandabutton.h"
#include "DescartaCommandaButton.h"
#include "EnviaComandaButton.h"
#include "BorrarButton.h"
#include "AceptaPaellaButton.h"
#include "EliminaComandaButton.h"
#include "OcultabarraButton.h"
#include "../../Data/ListaComandas.h"
#include "../../Control/NetworkManager.h"

#include "../../Scenes/Jornada.h"
#include "../../Scenes/Tutorial.h"
#include "../../Scenes/Menu.h"

#include "../../Utils/Texture.h"

#include <iostream>

UIManager::UIManager(Game* mGame)
{
	this->mGame = mGame;
	anchobotones *= uiscale;

	barra = nullptr;

	burnEffect = new Imagen(mGame, sdlutils().width() / 2, sdlutils().height() / 2, 
		sdlutils().width() * 2, sdlutils().height() * 2, "quemadura");
	burnEffect->setInitialDimension(sdlutils().width(), sdlutils().height());

	enAjustes = false;
}

UIManager::~UIManager()
{
	for (auto i : interfaz)
	{
		delete i;
		i = nullptr;
	}

	for (auto i : pauseMenu) {
		delete i;
		i = nullptr;
	}

	for (auto i : pauseButtons) {
		delete i;
		i = nullptr;
	}

	for (auto i : botones) {
		delete i;
		i = nullptr;
	}
	
	delete barra;
	barra = nullptr;

	delete burnEffect;
	burnEffect = nullptr;

	activeTweens.clear();
}

void UIManager::uiEvent(int mx, int my, bool& exit, bool paused)
{
	for (auto j : teclado)
	{
		if (j->isActive() && !paused)
		{
			if (j->onClick(mx, my, exit))
			{
				mx = -100;
				my = -100;
			}
		}
	}

	for(auto i : botones)
		
		if (i->isActive() && i->onClick(mx, my, exit)) {
			mx = -100;
			my = -100;
		}

	for (auto j : uicomandas)
	{
		if (j->isActive() && !paused)
		{
			if (j->onClick(mx, my, exit) && !paused)
			{
				mx = -100;
				my = -100;
			}
		}
	}

	if (barra != nullptr && !paused)
	{
		for (auto c : barra->getlista())
		{
			if (c->isActive())
			{
				if (c->getEliminabutton()->onClick(mx, my, exit))
				{
					mx = -100;
					my = -100;
				}
				if (c->onClick(mx, my, exit))
				{
					mx = -100;
					my = -100;
				}
			}
		}
	}

 	for (auto i : pauseButtons)
	{
		if (i->isActive())
		{
			if (i->onClick(mx, my, exit))
			{
				mx = -100;
				my = -100;
			}
		}
	}

	for (auto i : optionsButtons)
	{
		if (i->isActive())
		{
			if (i->onClick(mx, my, exit))
			{
				mx = -100;
				my = -100;
			}
		}
	}

	for (auto i : creditsButtons)
	{
		for (auto j : i) {
			if (j->isActive())
			{
				if (j->onClick(mx, my, exit))
				{
					mx = -100;
					my = -100;

				}
			}
		}
	}

	if (nombre != nullptr) {
		if (nombre->isActive()) {

			SDL_Point mouseP = { ih().getmx(), ih().getmy() };
			SDL_Rect sliderColl = nombre->getCollider();

			if (SDL_PointInRect(&mouseP, &sliderColl) == SDL_TRUE) {
				escribiendoNombre = !escribiendoNombre;
				ih().clearInputBuffer();
			}
		}
	}

	for (int i = 0; i < interfaz.size(); ++i)
	{
		if (interfaz[i]->isActive() && !paused)
		{
			if (interfaz[i]->onClick(mx, my, exit))
			{
				mx = -100;
				my = -100;
			}
		}
	}
}

void UIManager::slideEvent(int mx, int my, bool& exit, bool paused)
{
	for (auto i : sliders) {

		SDL_Point mouseP = { ih().getmx(), ih().getmy() };
		SDL_Rect sliderColl = i->getCollider();

		if (SDL_PointInRect(&mouseP, &sliderColl) == SDL_TRUE) {
			i->execute(exit);
			clickPrevio = true;
		}

		else if (!clickPrevio) {
			ih().resetHeld();
		}
	}
}

void UIManager::handleInput(bool& exit, bool paused)
{
	if (ih().getMouseButtonState(InputHandler::MOUSE_LEFT))
	{
		//cout << nombrePlayer << endl;
		uiEvent(ih().getmx(), ih().getmy(), exit, paused);
	}

	else if (escribiendoNombre) {
		nombre->execute(exit);
	}

	else if (ih().getMouseButtonHeld()) {
		slideEvent(ih().getmx(), ih().getmy(), exit);
	}

	if (!optionsButtons.empty() && (*optionsButtons.begin())->isActive()) {
		//Salimos de la configuracion
		if (ih().getKey(InputHandler::B)) {
			toggleOpciones();
			return;
		}

		//Movemos el slider de musica
		if (ih().getKey(InputHandler::LT) || ih().getKey(InputHandler::RT)) {
			auto barraVolumenMusica = optionsMenu[4];
			auto sliderMusica = sliders[0];
			volumenMusica += ih().getKey(InputHandler::RT) ? 12.8f : -12.8f;

			if (volumenMusica > 127) volumenMusica = 128;
			else if (volumenMusica < 1) volumenMusica = 0;

			std::map<std::string, Music>::iterator it = sdlutils().musics().begin();
			while (it != sdlutils().musics().end())
			{
				it->second.setMusicVolume(volumenMusica);
				++it;
			}

			sliderMusica->setPosition(barraVolumenMusica->getX() + (volumenMusica-64) / 64 * barraVolumenMusica->getWidth() / 2,
				slideMusica->getY());
		}

		//Movemos el slider de sonido
		if (ih().getKey(InputHandler::LB) || ih().getKey(InputHandler::RB)) {
			auto barraVolumenSonido = optionsMenu[6];
			auto sliderSonido = sliders[1];

			volumenSonido += ih().getKey(InputHandler::RB) ? 12.8f : -12.8f;

			if (volumenSonido > 127) volumenSonido = 128;
			else if (volumenSonido < 1) volumenSonido = 0;

			std::map<std::string, SoundEffect>::iterator it = sdlutils().soundEffects().begin();
			while (it != sdlutils().soundEffects().end())
			{
				it->second.setChannelVolume(volumenSonido);
				++it;
			}

			sliderSonido->setPosition(barraVolumenSonido->getX() + (volumenSonido - 64) / 64 * barraVolumenSonido->getWidth() / 2,
				slideSonido->getY());
		}

		if (ih().getKey(InputHandler::Y)) {
			sdlutils().toggleFullScreen();
		}
	}
}

void UIManager::update(bool paused)
{
	if (!paused ) {
		for (auto i : interfaz)
			if (i->isActive())
				i->update();
		for (auto i : comandas)
		{
			if(i->isActive())
			i->update();
		}
		if (enJornada)
		{
			if (ih().getKey(ih().LT))
			{
				if (getComanda() == nullptr)
				{
					creaComanda(mGame);
				}
				else
				{
					getComanda()->toggleactive();
				}
				if (barra->isBarraActive())
				{
				//barra->toggleBarra();
				}
			}
			if (ih().getKey(ih().RT))
			{

				barra->toggleBarra();
				if (getComanda()->isActive())
				{
					getComanda()->toggleactive();
				}

			}
			if (getBarra() != nullptr)
			{
				getBarra()->update();
			}
		}
		/*if (ih().getKey(ih().Y))
		{
			if (getComanda() == nullptr)
			{
				creaComanda(game);
			}
			else
			{
				getComanda()->toggleactive();
			}
			cout << "Y  ";
		}*/
	}

	if (enLobby && sdlutils().currRealTime() - initTime >= tiempoCreacion) {

		double x = cos(toRadians(alfa));
		double y = sin(toRadians(alfa));

		Imagen* circulo = new Imagen(mGame, sdlutils().width()/2 + x*50, sdlutils().height() - 75 + y*50, 16, 16, "circulo");

		circulo->setInitialDimension(16,16);

		addTween(1.0f, 0.75f, 800.0f,false).via(easing::exponentialOut).onStep([circulo, this](tweeny::tween<float>& t, float) mutable {
			circulo->setDimension(t.peek() * circulo->getInitialWidth(), t.peek() * circulo->getInitialHeight());

			if (t.progress() == 1.0f) {
				cargarAnimacion.pop_front();
				return true;
			}
			return false;
			});

		cargarAnimacion.push_back(circulo);

		alfa += 360 / 8;

		initTime = sdlutils().currRealTime();
	}

	auto i = activeTweens.begin();
	while (i != activeTweens.end() && !paused) {
		i->step(20);

		//Si el tween ha acabado, lo saco de la lista de tweens
		if (i->progress() == 1) {
			//Erase devuelve el iterador al siguiente tween
			i = activeTweens.erase(i);
		}
		else
			i++;
	}
	auto o = pauseTweens.begin();
	while (o != pauseTweens.end()) {
		o->step(20);

		//Si el tween ha acabado, lo saco de la lista de tweens
		if (o->progress() == 1) {
			//Erase devuelve el iterador al siguiente tween
			o = pauseTweens.erase(o);
		}
		else
			o++;
	}

	for (auto boton : botones) {
		boton->update();
	}
}

void UIManager::render(SDL_Rect* rect = nullptr)
{
	burnEffect->render(rect);

	if (barra != nullptr) {
		barra->render(rect);
		barra->renderComandas();
	}

	for (auto i : interfaz)
	{
		if (i->isActive())
		{
			i->render(rect);
		}
	}

	for (auto i : botones)
		if (i->isActive())
			i->render(rect);

	for (auto i : comandas)
	{
		if (i->isActive())
		{
			i->render(rect);
			i->dibujaPedido();
		}
	}

	for (auto i : teclado)
	{
		if (i->isActive())
		{
			i->render(rect);
		}
	}

	for (auto i : uicomandas)
	{
		if (i->isActive())
			i->render(rect);
	}

	for (auto i : pauseMenu) {
		if (i->isActive())
			i->render(rect);
	}

	for (auto i : pauseButtons) {
		if (i->isActive())
			i->render(rect);
	}

	for (auto i : optionsMenu) {
		if (i->isActive()) i->render(rect);
	}

	for (auto i : botonesMando) {
		if (i->isActive())i->render(rect);
	}

	for (auto i : creditsBase) {
		if (i->isActive()) i->render(rect);
	}

	for (auto i : creditsScreen) {
		for (auto j : i) {
			if (j->isActive()) j->render(rect);
		}
	}
	
	if (enLobby) {
		for (auto it = cargarAnimacion.begin(); it != cargarAnimacion.end(); it++) {
			auto i = *it;
			i->render(rect);
		}
	}
}

void UIManager::creaComanda(Game* mGame)
{
	actual = new Comanda(mGame, uiscale, this,false);
	comandas.push_back(actual);

	creaTeclado();
	actual->guardaTeclado();
	actual->toggleTeclado(false);
	actual->toggleTecaldotam(false);

	AceptaPaellaButton* aceptaPaellaButton = new AceptaPaellaButton(mGame, actual, "acepta", actual->getPosition().getX() + actual->getWidth() / 2 + anchobotones*0.7 , actual->getPosition().getY() + actual->getHeight() / 4, uiscale * anchobotones, uiscale * anchobotones);
	interfaz.push_back(aceptaPaellaButton);
	actual->guardaBoton(aceptaPaellaButton);
	DescartaCommandaButton* descartaComandaButton = new DescartaCommandaButton(actual, mGame, "cancela", actual->getPosition().getX() + actual->getWidth() / 2 + anchobotones*0.7, actual->getPosition().getY() + actual->getHeight() / 4 - anchobotones, uiscale * anchobotones, uiscale * anchobotones);
	interfaz.push_back(descartaComandaButton);
	actual->guardaBoton(descartaComandaButton);
	BorrarButton* borraComandaButton = new BorrarButton(mGame, actual, "borra", actual->getPosition().getX() + actual->getWidth() / 2 + anchobotones*0.7 , actual->getPosition().getY() + actual->getHeight() / 4 - 2 * anchobotones, uiscale * anchobotones, uiscale * anchobotones);
	interfaz.push_back(borraComandaButton);
	actual->guardaBoton(borraComandaButton);
	EnviaComandaButton* enviaComandaButton = new EnviaComandaButton(mGame, "envia", actual->getPosition().getX() + actual->getWidth() / 2 + anchobotones*0.7 , actual->getPosition().getY() + actual->getHeight() / 4 + anchobotones, uiscale * anchobotones, uiscale * anchobotones);
	interfaz.push_back(enviaComandaButton);
	actual->guardaBoton(enviaComandaButton);
	actual->setActive(false);
	actual->toggleactive();

	if (dynamic_cast<Tutorial*>(mGame->getCurrentScene()) && mGame->getCurrentScene()->getState() == States::abreLibreta)
		mGame->getCurrentScene()->changeState(States::pauasAbreLibreta);

}

Comanda* UIManager::getComanda()
{
	return actual;
}

ListaComandas* UIManager::getBarra()
{
	return barra;
}

void UIManager::setBarra(ListaComandas* b)
{
	barra = b;
	OcultabarraButton* o = new OcultabarraButton(this, mGame, "barraboton", b->getX() + b->getWidth()*0.7 + uiscale*anchobotones, b->getY(), 1.7 * anchobotones, uiscale * anchobotones, b);
	addInterfaz(o);
}

void UIManager::creaTeclado()
{
	int margenbotones = 5;
	double ix = actual->getPosition().getX() / 2 + margenbotones + anchobotones / 2;
	double iy = actual->getPosition().getY() / 2 + 2 * anchobotones;
	for (int i = 0; i < 9; ++i)
	{
		posicionesBotones.push_back(Point2D<double>(ix, iy));
		ix += anchobotones + margenbotones;
		if (ix >= actual->getPosition().getX() + actual->getWidth() / 2 - anchobotones / 2)
		{
			ix = actual->getPosition().getX() / 2 + margenbotones + anchobotones / 2;
			iy += anchobotones + margenbotones;
		}
	}
	int j = 0;
	vector <UiButton*> tecladonum;

	for (auto i : texturasNumeros)
	{
		//Comanda comanda,Game* game, TextureName texturename, int x, int y, int w, int h
		NumeroButton* a = new NumeroButton(this, mGame, i, (int)posicionesBotones[j].getX(), (int)posicionesBotones[j].getY(), uiscale * anchobotones, uiscale * anchobotones,j);
		tecladonum.push_back(a);
		uicomandas.push_back(a);
		j++;
	}

	actual->guardaTecladonum(tecladonum);
	if (!enVentanilla) {

		j = 0;
		for (auto i : texturasIngredienes)
		{
			//Comanda comanda,Game* game, TextureName texturename, int x, int y, int w, int h
			IngredienteButton* a = new IngredienteButton(this, mGame, i, (int)posicionesBotones[j].getX(), (int)posicionesBotones[j].getY(), uiscale * anchobotones, uiscale * anchobotones,j);
			teclado.push_back(a);
			//  objectmanager->creaTeclado(a);

			j++;
		}

		actual->guardaTeclado();

		j = 0;
		vector <UiButton*> tecladotam;
		for (auto i : texturasTamanos)
		{
			TamanoButton* a = new TamanoButton(this, mGame, i, (int)posicionesBotones[j].getX(), (int)posicionesBotones[j].getY(), uiscale * anchobotones, uiscale * anchobotones,j);

			tecladotam.push_back(a);
			uicomandas.push_back(a);
			j++;
		}
		actual->guardaTecladotam(tecladotam);
	}


}

void UIManager::randomizaTeclado()
{
	vector<Point2D<double>> posdis = posicionesBotones;
	if (!enVentanilla&&posdis.size()>0) {
		
		int j = rand() % posdis.size();
		for (auto i : teclado)
		{
			i->setPosition(posdis[j]);
			posdis.erase(posdis.begin() + j);
			if (posdis.size() > 0)
				j = rand() % posdis.size();
		}

		sort(teclado.begin(), teclado.end(), [](UiButton* u1, UiButton* u2) {return  u1->getPosition().getX() < u2->getPosition().getX(); });
		sort(teclado.begin(), teclado.end(), [](UiButton* u1, UiButton* u2) {return  u1->getPosition().getY() < u2->getPosition().getY(); });

		if (teclado[0]->isActive())actual->setActiveTeclado(teclado);
	}
}

void UIManager::addInterfaz(GameObject* elementoInterfaz)
{
	interfaz.push_back(elementoInterfaz);
}

void UIManager::addButton(UiButton* button)
{
	botones.push_back(button);
	button->setIterador(--botones.end());

	foco = botones.begin();
}

vector<Point2D<double>> UIManager::getPosTeclado()
{
	return posicionesBotones;
}

void UIManager::setPosTeclado(vector<Point2D<double>> t)
{
	posicionesBotones = t;
}

void UIManager::creaMenuPausa() {
	// crear menu
	Imagen* bg = new Imagen(mGame, sdlutils().width() / 2, sdlutils().height() / 2, sdlutils().width(), sdlutils().height(), "pause2");
	bg->setActive(false);
	pauseMenu.push_back(bg);

	Imagen* pauseImage = new Imagen(mGame, sdlutils().width() / 2, sdlutils().height() / 2, 500, 360, "pause1");
	pauseImage->setActive(false);
	pauseMenu.push_back(pauseImage);

	UiButton* resumeButton = new UiButton(mGame, "resumeBoton", sdlutils().width() / 2, sdlutils().height() / 2 - 40, 320, 130);
	resumeButton->setInitialDimension(320, 130);
	resumeButton->setActive(false);

	resumeButton->setAction([this, resumeButton](Game* mGame, bool& exit) {
		// Pausa
		if (dynamic_cast<Tutorial*>(mGame->getCurrentScene())) {
			Tutorial* currentScene = dynamic_cast<Tutorial*>(mGame->getCurrentScene());

			addTween(0.9f, 1.0f, 600.0f,false).via(easing::exponentialOut).onStep([resumeButton, currentScene](tweeny::tween<float>& t, float) mutable {
				resumeButton->setDimension(t.peek() * resumeButton->getInitialWidth(), t.peek() * resumeButton->getInitialHeight());

				if (t.progress() > .2f) { //Busco una respuesta mas rapida
					// Resume
					currentScene->togglePause();
					return true;
				}
				return false;
				});
		}
		else {
			Jornada* currentScene = dynamic_cast<Jornada*>(mGame->getCurrentScene());

			addTween(0.9f, 1.0f, 600.0f,false).via(easing::exponentialOut).onStep([mGame, resumeButton, currentScene](tweeny::tween<float>& t, float) mutable {
				resumeButton->setDimension(t.peek() * resumeButton->getInitialWidth(), t.peek() * resumeButton->getInitialHeight());

				if (t.progress() > .2f) { //Busco una respuesta mas rapida
					// Resume
					currentScene->togglePause();
					mGame->getNetworkManager()->syncPause();
					return true;
				}
				return false;
			});
		}
	});

	pauseButtons.push_back(resumeButton);

	UiButton* exitButton = new UiButton(mGame, "exitBoton", sdlutils().width() / 2, sdlutils().height() / 2 + 60, 320, 130);
	exitButton->setInitialDimension(320, 130);
	exitButton->setActive(false);

	exitButton->setAction([this, exitButton](Game* mGame, bool& exit) mutable {
		sdlutils().soundEffects().at("select").play(0, mGame->UI);

		addTween(0.9f, 1.0f, 600.0f,false).via(easing::exponentialOut).onStep([this, mGame, exitButton](tweeny::tween<float>& t, float) mutable {
			exitButton->setDimension(t.peek() * exitButton->getInitialWidth(), t.peek() * exitButton->getInitialHeight());

			
			if (t.progress() == 1.0f) {
				// Exit
				mGame->getNetworkManager()->setGameStarted(false);
				mGame->getNetworkManager()->close();
				mGame->sendMessageScene(new Menu(mGame));

				return true;
			}
			return false;
			});
		});

	pauseButtons.push_back(exitButton);
}

void UIManager::creaMenuOpciones()
{
	//Fondo De las opciones
	Imagen* opcPant = new Imagen(mGame, sdlutils().width() / 2, sdlutils().height() / 2, sdlutils().width(), sdlutils().height(), "fondoOpc");
	opcPant->setInitialPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	opcPant->setActive(false);

	optionsMenu.push_back(opcPant);

	//Boton salir
	UiButton* botonSalir = new UiButton(mGame, "cerrarOpc", opcPant->getWidth()-90, 75, 100, 100);
	botonSalir->setInitialPosition(opcPant->getWidth()-90, 75);
	botonSalir->setActive(false);
	botonSalir->setAction([this, botonSalir](Game* mGame, bool& exit) {
		sdlutils().soundEffects().at("botonesMenu").play(0, mGame->UI);
		addTween(0.9f, 1.0f, 600.0f, false).via(easing::exponentialOut).onStep([botonSalir, this](tweeny::tween<float>& t, float) mutable {
			botonSalir->setDimension(t.peek() * botonSalir->getInitialWidth(), t.peek() * botonSalir->getInitialHeight());

			if (t.progress() > .2f) {
				toggleOpciones();
				return true;
			}
			return false;
			});
		});

	optionsMenu.push_back(botonSalir);
	optionsButtons.push_back(botonSalir);

	//Boton pantalla completa
	UiButton* pantCompleta = new UiButton(mGame, "fondoMadera", sdlutils().width() / 2 + sdlutils().width() / 4 +30, opcPant->getHeight() - 200, 100, 100);
	pantCompleta->setInitialPosition(sdlutils().width() / 2 + sdlutils().width() / 4 + 30, opcPant->getHeight() - 200);
	pantCompleta->setActive(false);

	pantCompleta->setAction([this, pantCompleta](Game* mGame, bool& exit) {
		sdlutils().soundEffects().at("botonesMenu").play(0, mGame->UI);
		sdlutils().toggleFullScreen();
		});

	optionsButtons.push_back(pantCompleta);
	optionsMenu.push_back(pantCompleta);

	//Boton Pant Completa
	Imagen* botPantCompl = new Imagen(mGame, pantCompleta->getX() + pantCompleta->getWidth() / 2 + 35,
		pantCompleta->getY(), 50, 50, "Y");
	botPantCompl->setActive(false);
	botonesMando.push_back(botPantCompl);

	//Texto pantalla completa
	Imagen* textoPCompleta = new Imagen(mGame, pantCompleta->getX() - pantCompleta->getWidth() / 2 - 135, pantCompleta->getY(), 280, 80, "PCompleta");
	textoPCompleta->setInitialPosition(pantCompleta->getX() - pantCompleta->getWidth() / 2 - 135, pantCompleta->getY());
	textoPCompleta->setActive(false);

	optionsMenu.push_back(textoPCompleta);

	//Barra Musica
	Imagen* barraVol_musica = new Imagen(mGame, sdlutils().width() / 2 + 75, sdlutils().height() / 2 - 100, 700, 30, "barraVol");
	barraVol_musica->setInitialPosition(sdlutils().width() / 2 + 75, sdlutils().height() / 2 - 100);
	barraVol_musica->setActive(false);

	optionsMenu.push_back(barraVol_musica);

	//Bontes para Musica
	Imagen* botMusicaIzq = new Imagen(mGame, barraVol_musica->getX() - barraVol_musica->getWidth() / 2 + 35,
		barraVol_musica->getY() - barraVol_musica->getHeight() - 15, 60, 50, "LT");
	botMusicaIzq->setActive(false);
	botonesMando.push_back(botMusicaIzq);

	Imagen* botMusicaDer = new Imagen(mGame, barraVol_musica->getX() + barraVol_musica->getWidth() / 2 - 35,
		barraVol_musica->getY() - barraVol_musica->getHeight() - 15, 60, 50, "RT");
	botMusicaDer->setActive(false);
	botonesMando.push_back(botMusicaDer);

	//Texto barra musica
	Imagen* textoMusica = new Imagen(mGame, barraVol_musica->getX() - barraVol_musica->getWidth() / 2 - 100, barraVol_musica->getY(), 200, 80, "musica");
	textoMusica->setInitialPosition(barraVol_musica->getX() - barraVol_musica->getWidth() / 2 - 100, barraVol_musica->getY());
	textoMusica->setActive(false);

	optionsMenu.push_back(textoMusica);

	//Barra sonido
	Imagen* barraVol_sonido = new Imagen(mGame, sdlutils().width() / 2 + 75, sdlutils().height() / 2, 700, 30, "barraVol");
	barraVol_sonido->setInitialPosition(sdlutils().width() / 2 + 75, sdlutils().height() / 2);
	barraVol_sonido->setActive(false);

	optionsMenu.push_back(barraVol_sonido);

	//Bontes para sonido
	Imagen* botSonidoIzq = new Imagen(mGame, barraVol_sonido->getX() - barraVol_sonido->getWidth() / 2 + 35,
		barraVol_sonido->getY() - barraVol_sonido->getHeight() - 15, 60, 50, "LB");
	botSonidoIzq->setActive(false);
	botonesMando.push_back(botSonidoIzq);

	Imagen* botSonidoDer = new Imagen(mGame, barraVol_sonido->getX() + barraVol_sonido->getWidth() / 2 - 35,
		barraVol_sonido->getY() - barraVol_sonido->getHeight() - 15, 60, 50, "RB");
	botSonidoDer->setActive(false);
	botonesMando.push_back(botSonidoDer);

	//Texto barra sonido
	Imagen* textoSonido = new Imagen(mGame, barraVol_sonido->getX() - barraVol_sonido->getWidth() / 2 - 100, barraVol_sonido->getY(), 200, 80, "sonido");
	textoSonido->setInitialPosition(barraVol_sonido->getX() - barraVol_sonido->getWidth() / 2 - 100, barraVol_sonido->getY());
	textoSonido->setActive(false);

	optionsMenu.push_back(textoSonido);

	//Slide musica
	int x = barraVol_musica->getX(), y = barraVol_musica->getY();

	if (mGame->getSlideMus().getX() != 0) {
		x = mGame->getSlideMus().getX();
		y = mGame->getSlideMus().getY();
	}

	slideMusica = new UiButton(mGame, "paella", x, y, 80, 80);
	slideMusica->setInitialPosition(x, y);
	slideMusica->setActive(false);
	slideMusica->setAction([this, barraVol_musica](Game* mGame, bool& exit) {

		int newPos = ih().getmx() - slideMusica->getWidth();
		int extremoIzq = barraVol_musica->getX() - (barraVol_musica->getWidth() / 2) - slideMusica->getWidth();
		int extremoDer = barraVol_musica->getX() + (barraVol_musica->getWidth() / 2) - slideMusica->getWidth();

		if (newPos >= extremoIzq && newPos <= extremoDer) {

			slideMusica->setPosition(ih().getmx(), slideMusica->getY());
		}

		volumenMusica = (slideMusica->getX() - slideMusica->getWidth() - extremoIzq) / barraVol_musica->getWidth() * 128;

		if (volumenMusica > 127) volumenMusica = 128;
		else if (volumenMusica < 1) volumenMusica = 0;
		
		std::map<std::string, Music>::iterator it = sdlutils().musics().begin();
		while (it != sdlutils().musics().end())
		{
			it->second.setMusicVolume(volumenMusica);
			++it;
		}

		mGame->setSlidesMus(slideMusica->getPosition());

		});

	optionsMenu.push_back(slideMusica);
	sliders.push_back(slideMusica);

	//Slide sonido
	x = barraVol_sonido->getX(), y = barraVol_sonido->getY();

	if (mGame->getSlideSon().getX() != 0) {
		x = mGame->getSlideSon().getX();
		y = mGame->getSlideSon().getY();
	}

	slideSonido = new UiButton(mGame, "paella", x, y, 80, 80);
	slideSonido->setInitialPosition(x, y);
	slideSonido->setActive(false);
	slideSonido->setAction([this, barraVol_sonido](Game* mGame, bool& exit) {

	int newPos = ih().getmx() - slideSonido->getWidth();
	int extremoIzq = barraVol_sonido->getX() - (barraVol_sonido->getWidth() / 2) - slideSonido->getWidth();
	int extremoDer = barraVol_sonido->getX() + (barraVol_sonido->getWidth() / 2) - slideSonido->getWidth();

	if (newPos >= extremoIzq && newPos <= extremoDer) {

		slideSonido->setPosition(ih().getmx(), slideSonido->getY());
	}

	volumenSonido = (slideSonido->getX() - slideSonido->getWidth() - extremoIzq) / barraVol_sonido->getWidth() * 128;

	if (volumenSonido > 127) volumenSonido = 128;
	else if (volumenSonido < 1) volumenSonido = 0;

	std::map<std::string, SoundEffect>::iterator it = sdlutils().soundEffects().begin();
	while (it != sdlutils().soundEffects().end())
	{
		it->second.setChannelVolume(volumenSonido);
		++it;
	}

	mGame->setSlidesSon(slideSonido->getPosition());

	});

	optionsMenu.push_back(slideSonido);
	sliders.push_back(slideSonido);


	//Caja de texto del nombre
	Imagen* fondoNombre = new Imagen(mGame, sdlutils().width() / 2 - 120, opcPant->getHeight() - 200, 210, 50, "fondoMadera");
	fondoNombre->setInitialPosition(sdlutils().width() / 2 - 120, opcPant->getHeight() - 200);
	fondoNombre->setActive(false);

	optionsMenu.push_back(fondoNombre);

	//Boton para escribir el nombre
	nombre = new UiButton(mGame, mGame->getNombre(), "ip", { 255, 255, 255, 255 }, { 0, 0, 0, 0 }, sdlutils().width() / 2 - 120, opcPant->getHeight() - 200);
	cursor_ = new GameObject(mGame);
	nombre->setInitialPosition(sdlutils().width() / 2 - 120, opcPant->getHeight() - 200);
	nombre->setActive(false);
	
	nombre->setAction([this](Game* mGame, bool& exit) {

		char c = ih().getTypedKey();
		string nombrePlayer = mGame->getNombre();

		if (c != ' ')
		{
			if (c == '\b')
			{

				if (!nombrePlayer.empty())
					nombrePlayer.pop_back();
				else
					nombrePlayer = ' ';
			}

			else if (nombrePlayer.size() < maxCaracteres) {
				nombrePlayer += c;
			}

			if (nombrePlayer.empty())
				nombrePlayer = ' ';

			mGame->setNombre(nombrePlayer);

			nombre->setTexture(nombrePlayer, string("abadiNombre"), { 255, 255, 255, 255 }, { 0, 0, 0, 0 });
			nombre->setDimension();
		}
		nombre->render(nullptr);
		});

	optionsMenu.push_back(nombre);

	//Texto nombre
	Imagen* textoNombre = new Imagen(mGame, fondoNombre->getX() - fondoNombre->getWidth() / 2 - 120, fondoNombre->getY(), 170, 80, "nombre");
	textoNombre->setInitialPosition(fondoNombre->getX() - fondoNombre->getWidth() / 2 - 120, fondoNombre->getY());
	textoNombre->setActive(false);

	optionsMenu.push_back(textoNombre);

	volumenMusica = 64;
	volumenSonido = 64;
}

void UIManager::creaPantallaCreditos()
{
	creditsScreen.push_back({});
	creditsButtons.push_back({});

	//Fondo De las opciones
	Imagen* opcPant = new Imagen(mGame, sdlutils().width() / 2, sdlutils().height() / 2, sdlutils().width(), sdlutils().height(), "fondoOpc");

	opcPant->setInitialPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	opcPant->setActive(false);

	creditsBase.push_back(opcPant);

	//Boton salir
	UiButton* botonSalir = new UiButton(mGame, "cerrarOpc", opcPant->getWidth() - 90, 75, 100, 100);

	botonSalir->setInitialPosition(opcPant->getWidth()-90, 75);
	botonSalir->setActive(false);
	botonSalir->setAction([this, botonSalir](Game* mGame, bool& exit) {
		sdlutils().soundEffects().at("botonesMenu").play(0, mGame->UI);
		addTween(0.9f, 1.0f, 600.0f, false).via(easing::exponentialOut).onStep([botonSalir, this](tweeny::tween<float>& t, float) mutable {
			botonSalir->setDimension(t.peek() * botonSalir->getInitialWidth(), t.peek() * botonSalir->getInitialHeight());

			if (t.progress() > .2f) {
				salirCreditos();
				activaBot();
				return true;
			}
			return false;
			});
		});

	creditsBase.push_back(botonSalir);
	creditsButtons[0].push_back(botonSalir);

	//Boton siguiente página
	UiButton* botonNext = new UiButton(mGame, "nextCredits", opcPant->getWidth() - 70, sdlutils().height() / 2, 100, 100);
	botonNext->setInitialPosition(opcPant->getWidth()-70, sdlutils().height() / 2);
	botonNext->setActive(false);
	botonNext->setAction([this, botonNext](Game* mGame, bool& exit) {
		sdlutils().soundEffects().at("botonesMenu").play(0, mGame->UI);
		addTween(0.9f, 1.0f, 600.0f, false).via(easing::exponentialOut).onStep([botonNext, this](tweeny::tween<float>& t, float) mutable {
			botonNext->setDimension(t.peek() * botonNext->getInitialWidth(), t.peek() * botonNext->getInitialHeight());

			if (t.progress() > .2f) {
				setFromMenu(false);
				setFromRight(true);
				toggleCreditos(paginaCreditos + 1);

				return true;
			}
			return false;
			});
			
		});

	creditsScreen[0].push_back(botonNext);
	creditsButtons[0].push_back(botonNext);

	//Título de desarrolladores

	ShowText* TituloDev = new ShowText(mGame, "Desarrolladores", "lobby",
		sdlutils().width() / 2, 70, 0, 0);
	TituloDev->setInitialPosition(sdlutils().width() / 2, 70);
	TituloDev->setActive(false);

	creditsScreen[0].push_back(TituloDev);

	//Nombres de los desarrolladores

	ShowText* Dev1 = new ShowText(mGame, "Javier Cano", "creditsName",
		sdlutils().width() / 3, posInicialCreditos,0,0);
	Dev1->setInitialPosition(sdlutils().width() / 3, posInicialCreditos);
	Dev1->setActive(false);

	creditsScreen[0].push_back(Dev1);

	ShowText* Dev2 = new ShowText(mGame, "José Miguel Villacañas", "creditsName",
		2*(sdlutils().width()) / 3, posInicialCreditos, 0, 0);
	Dev2->setInitialPosition(2 * (sdlutils().width()) / 3, posInicialCreditos);
	Dev2->setActive(false);

	creditsScreen[0].push_back(Dev2);

	ShowText* Dev3 = new ShowText(mGame, "Víctor Estremera", "creditsName",
		sdlutils().width() / 3, posInicialCreditos + avanceCreditos, 0, 0);
	Dev3->setInitialPosition(sdlutils().width() / 3, posInicialCreditos + avanceCreditos);
	Dev3->setActive(false);

	creditsScreen[0].push_back(Dev3);

	ShowText* Dev4 = new ShowText(mGame, "Rodrigo Cabello", "creditsName",
		2 * (sdlutils().width()) / 3, posInicialCreditos + avanceCreditos, 0, 0);
	Dev4->setInitialPosition(2 * (sdlutils().width()) / 3, posInicialCreditos + avanceCreditos);
	Dev4->setActive(false);

	creditsScreen[0].push_back(Dev4);

	ShowText* Dev5 = new ShowText(mGame, "Diego Rol", "creditsName",
		sdlutils().width() / 3, posInicialCreditos + avanceCreditos * 2, 0, 0);
	Dev5->setInitialPosition(sdlutils().width() / 3, posInicialCreditos + avanceCreditos * 2);
	Dev5->setActive(false);

	creditsScreen[0].push_back(Dev5);

	ShowText* Dev6 = new ShowText(mGame, "Rocío Sanchez-Horcajuelo", "creditsName",
		2 * (sdlutils().width()) / 3, posInicialCreditos + avanceCreditos * 2, 0, 0);
	Dev6->setInitialPosition(2 * (sdlutils().width()) / 3, posInicialCreditos + avanceCreditos * 2);
	Dev6->setActive(false);

	creditsScreen[0].push_back(Dev6);

	ShowText* Dev7 = new ShowText(mGame, "Marta Croche", "creditsName",
		sdlutils().width() / 3, posInicialCreditos + avanceCreditos * 3, 0, 0);
	Dev7->setInitialPosition(sdlutils().width() / 3, posInicialCreditos + avanceCreditos * 3);
	Dev7->setActive(false);

	creditsScreen[0].push_back(Dev7);

	ShowText* Dev8 = new ShowText(mGame, "Pablo Arredondo", "creditsName",
		2 * (sdlutils().width()) / 3, posInicialCreditos + avanceCreditos * 3, 0, 0);
	Dev8->setInitialPosition(2 * (sdlutils().width()) / 3, posInicialCreditos + avanceCreditos * 3);
	Dev8->setActive(false);

	creditsScreen[0].push_back(Dev8);

	ShowText* Dev9 = new ShowText(mGame, "Elena Robert", "creditsName",
		sdlutils().width() / 3, posInicialCreditos + avanceCreditos * 4, 0, 0);
	Dev9->setInitialPosition(sdlutils().width() / 3, posInicialCreditos + avanceCreditos * 4);
	Dev9->setActive(false);

	creditsScreen[0].push_back(Dev9);

	ShowText* Dev10 = new ShowText(mGame, "Alfonso Rodulfo", "creditsName",
		2 * (sdlutils().width()) / 3, posInicialCreditos + avanceCreditos * 4, 0, 0);
	Dev10->setInitialPosition(2 * (sdlutils().width()) / 3, posInicialCreditos + avanceCreditos * 4);
	Dev10->setActive(false);

	creditsScreen[0].push_back(Dev10);

	//Página final de créditos.

	creditsScreen.push_back({});
	creditsButtons.push_back({});

	//Boton página anterior 2
	UiButton* botonPrevious2 = new UiButton(mGame, "previousCredits", 70, sdlutils().height() / 2, 100, 100);
	botonPrevious2->setInitialPosition(70, sdlutils().height() / 2);
	botonPrevious2->setActive(false);
	botonPrevious2->setAction([this, botonPrevious2](Game* mGame, bool& exit) {
		sdlutils().soundEffects().at("botonesMenu").play(0, mGame->UI);
		addTween(0.9f, 1.0f, 600.0f, false).via(easing::exponentialOut).onStep([botonPrevious2, this](tweeny::tween<float>& t, float) mutable {
			botonPrevious2->setDimension(t.peek() * botonPrevious2->getInitialWidth(), t.peek() * botonPrevious2->getInitialHeight());

			if (t.progress() > .2f) {
				setFromRight(false);
				toggleCreditos(paginaCreditos - 1);

				return true;
			}
			return false;
			});
		
		});

	creditsScreen[1].push_back(botonPrevious2);
	creditsButtons[1].push_back(botonPrevious2);

	//Título de testers

	ShowText* TituloPrueba2 = new ShowText(mGame, "Testing", "lobby",
		sdlutils().width() / 2, 70, 0, 0);
	TituloPrueba2->setInitialPosition(sdlutils().width() / 2, 70);
	TituloPrueba2->setActive(false);

	creditsScreen[1].push_back(TituloPrueba2);

	//Nombre de testers

	ShowText* Tester1 = new ShowText(mGame, "Andrea Otero", "creditsName",
		sdlutils().width() / 3, posInicialCreditos, 0, 0);
	Tester1->setInitialPosition(sdlutils().width() / 3, posInicialCreditos);
	Tester1->setActive(false);

	creditsScreen[1].push_back(Tester1);

	ShowText* Tester2 = new ShowText(mGame, "Ruben Andrade", "creditsName",
		2 * (sdlutils().width()) / 3, posInicialCreditos, 0, 0);
	Tester2->setInitialPosition(2 * (sdlutils().width()) / 3, posInicialCreditos);
	Tester2->setActive(false);

	creditsScreen[1].push_back(Tester2);

	ShowText* Tester3 = new ShowText(mGame, "David Andrade", "creditsName",
		sdlutils().width() / 3, posInicialCreditos + avanceCreditos, 0, 0);
	Tester3->setInitialPosition(sdlutils().width() / 3, posInicialCreditos + avanceCreditos);
	Tester3->setActive(false);

	creditsScreen[1].push_back(Tester3);

	ShowText* Tester4 = new ShowText(mGame, "Jaime Benedí", "creditsName",
		2 * (sdlutils().width()) / 3, posInicialCreditos + avanceCreditos, 0, 0);
	Tester4->setInitialPosition(2 * (sdlutils().width()) / 3, posInicialCreditos + avanceCreditos);
	Tester4->setActive(false);

	creditsScreen[1].push_back(Tester4);

	ShowText* Tester5 = new ShowText(mGame, "Mikele Lopez", "creditsName",
		sdlutils().width() / 3, posInicialCreditos + avanceCreditos * 2, 0, 0);
	Tester5->setInitialPosition(sdlutils().width() / 3, posInicialCreditos + avanceCreditos * 2);
	Tester5->setActive(false);

	creditsScreen[1].push_back(Tester5);

	ShowText* Tester6 = new ShowText(mGame, "Andrea Vega", "creditsName",
		2 * (sdlutils().width()) / 3, posInicialCreditos + avanceCreditos * 2, 0, 0);
	Tester6->setInitialPosition(2 * (sdlutils().width()) / 3, posInicialCreditos + avanceCreditos * 2);
	Tester6->setActive(false);

	creditsScreen[1].push_back(Tester6);

}

void UIManager::togglePause() {
	for (auto i : pauseMenu) {
		i->setActive(!i->isActive());
	}

	//Solo una imagen tiene el tween
	auto pauseImage = pauseMenu[1];
	pauseImage->setDimension(0, 0);

	if (pauseImage->isActive()) {
		addTween(0.8f, 1.0f, 600.0f,false).via(easing::exponentialOut).onStep([pauseImage](tweeny::tween<float>& t, float) mutable {
			pauseImage->setDimension(t.peek() * pauseImage->getInitialWidth(), t.peek() * pauseImage->getInitialHeight());

			if (t.progress() == 1.0f) {
				return true;
			}
			return false;
			});

		sdlutils().soundEffects().at("paella").pauseChannel();
	}
	else
		sdlutils().soundEffects().at("paella").resumeChannel();

	for (auto i : pauseButtons) {
		i->setDimension(0, 0);
		i->setActive(!i->isActive());

		if (i->isActive()) {
			addTween(0.8f, 1.0f, 600.0f,false).via(easing::exponentialOut).onStep([i](tweeny::tween<float>& t, float) mutable {
				i->setDimension(t.peek() * i->getInitialWidth(), t.peek() * i->getInitialHeight());

				if (t.progress() == 1.0f) {
					return true;
				}
				return false;
				});
		}
	}
}

void UIManager::toggleOpciones()
{
	enAjustes = !enAjustes;

	for (auto i : optionsMenu) {

		if (!i->isActive()) {
			i->setActive(!i->isActive());
			enterTweenSetter(i, enAjustes);
		}
		else {
			exitTweenSetter(i);
			activaBot();
		}

	}

	if (ih().isMandoActive() && enAjustes) {
		for (auto i : botonesMando) {
			i->setActive(true);
		}
	}

	else {
		for (auto i : botonesMando) {
			i->setActive(false);
		}
	}

	escribiendoNombre = false;
}

void UIManager::toggleCreditos(int pagina)
{
	for (auto i : creditsScreen[paginaCreditos]) {
		i->setActive(false);
	}
	for (auto i : creditsScreen[pagina]) {
		i->setActive(true);
		enterTweenSetter(i, enAjustes);
	}
	for (auto i : creditsBase) {
		i->setActive(true);
		enterTweenSetter(i, enAjustes);
	}
	paginaCreditos = pagina;
}

void UIManager::salirCreditos()
{
	for (auto i : creditsScreen[paginaCreditos]) {
		exitTweenSetter(i);
	}
	for (auto i : creditsBase) {
		exitTweenSetter(i);
	}
}

void UIManager::enterTweenSetter(GameObject* i, bool enAjustes_) {
	if (i->isActive() && !enAjustes_) {
		if (fromMenu) {
			addTween(2.0f, 1.0f, 600.0f, false).via(easing::exponentialOut).onStep([i](tweeny::tween<float>& t, float) mutable {
				i->setPosition(i->getInitialPositionX(), t.peek() * i->getInitialPositionY());

				if (t.progress() == 1.0f) {
					return true;
				}
				return false;
				});
		}
		else if (!fromMenu && fromRight) {
			addTween(2.0f, 1.0f, 600.0f, false).via(easing::exponentialOut).onStep([i](tweeny::tween<float>& t, float) mutable {
				i->setPosition(t.peek() * i->getInitialPositionX(), i->getInitialPositionY());

				if (t.progress() == 1.0f) {
					return true;
				}
				return false;
				});
		}
		else if (!fromMenu && !fromRight) {
			addTween(0.0f, 1.0f, 600.0f, false).via(easing::exponentialOut).onStep([i](tweeny::tween<float>& t, float) mutable {
				i->setPosition(t.peek() * i->getInitialPositionX(), i->getInitialPositionY());

				if (t.progress() == 1.0f) {
					return true;
				}
				return false;
				});
		}
	}
	else if (i->isActive() && enAjustes_) {
		addTween(2.0f, 1.0f, 600.0f, false).via(easing::exponentialOut).onStep([i](tweeny::tween<float>& t, float) mutable {
			i->setPosition(i->getInitialPositionX(), t.peek() * i->getInitialPositionY());

			if (t.progress() == 1.0f) {
				return true;
			}
			return false;
			});
	}
}

void UIManager::exitTweenSetter(GameObject* i)
{
	if (i->isActive()){
		addTween(1.0f, 1.5f, 600.0f, false).via(easing::exponentialOut).onStep([i](tweeny::tween<float>& t, float) mutable {
			i->setPosition(i->getInitialPositionX(), t.peek() * i->getInitialPositionY());

			if (t.progress() > 0.2f) {
				i->setActive(false);
				return true;
			}
			return false;
			});
	}
}

void UIManager::desactivaBot()
{
	for (auto i : botones) {
		i->setActive(false);
	}
}

void UIManager::activaBot()
{
	for (auto i : botones) {
		i->setActive(true);
	}
}

tweeny::tween<float>& UIManager::addTween(float from, float to, float during,bool activeTweens_) {
	tweeny::tween<float> tween = tweeny::tween<float>::from(from).to(to).during(during);
	if (activeTweens_) {
		activeTweens.push_front(tween);
		return activeTweens.front();
	}
	else {
		pauseTweens.push_front(tween);
		return pauseTweens.front();
	}
}

void UIManager::setEnLobby(bool enLobby_)
{
	enLobby = enLobby_;
}


void UIManager::setFromRight(bool fromRight_)
{
	fromRight = fromRight_;
}

void UIManager::setFromMenu(bool fromMenu_)
{
	fromMenu = fromMenu_;
}

void UIManager::quemarse()
{
	//Aparece el efecto de quemarse en la interfaz y suena un sonido
	addTween(2, 1, 300,true).via(easing::cubicOut).onStep([this](tweeny::tween<float>& t, float) mutable {
		burnEffect->setDimension(t.peek() * burnEffect->getInitialWidth(), t.peek() * burnEffect->getInitialHeight());

		if (t.progress() == 1.0f) {
			addTween(1, 2, 5000,true).via(easing::sinusoidalIn).onStep([this](tweeny::tween<float>& t, float) mutable {
				burnEffect->setDimension(t.peek() * burnEffect->getInitialWidth(), t.peek() * burnEffect->getInitialHeight());
				
				return t.progress() == 1.0f;
				});

			return true;
		}
		return false;
		});

	sdlutils().soundEffects().at("quemadura").play();
}

void UIManager::focoExecute(bool& exit)
{
	if (!botones.empty())
		(*foco)->execute(exit);
}

void UIManager::avanzaFoco()
{
	if (!botones.empty()) {
		auto anteriorFoco = *foco;
		addTween(1.1f, 1.0f, 600.0f,false).via(easing::exponentialOut).onStep([anteriorFoco](tweeny::tween<float>& t, float) mutable {
			anteriorFoco->setDimension(t.peek() * anteriorFoco->getInitialWidth(), t.peek() * anteriorFoco->getInitialHeight());

			return t.progress() == 1.0f;
			});

		foco++;
		if (foco == botones.end())
			foco = botones.begin();

		auto nuevoFoco = *foco;
		addTween(1.0f, 1.1f, 600.0f, false).via(easing::exponentialOut).onStep([nuevoFoco](tweeny::tween<float>& t, float) mutable {
			nuevoFoco->setDimension(t.peek() * nuevoFoco->getInitialWidth(), t.peek() * nuevoFoco->getInitialHeight());

			return t.progress() == 1.0f;
			});
	}
}

void UIManager::retrocedeFoco()
{
	if (!botones.empty()) {
		auto anteriorFoco = *foco;
		addTween(1.1f, 1.0f, 600.0f,false).via(easing::exponentialOut).onStep([anteriorFoco](tweeny::tween<float>& t, float) mutable {
			anteriorFoco->setDimension(t.peek() * anteriorFoco->getInitialWidth(), t.peek() * anteriorFoco->getInitialHeight());

			return t.progress() == 1.0f;
			});

		if (foco == botones.begin())
			foco = --botones.end();
		else 
			foco--;

		auto nuevoFoco = *foco;
		addTween(1.0f, 1.1f, 600.0f,false).via(easing::exponentialOut).onStep([nuevoFoco](tweeny::tween<float>& t, float) mutable {
			nuevoFoco->setDimension(t.peek() * nuevoFoco->getInitialWidth(), t.peek() * nuevoFoco->getInitialHeight());

			return t.progress() == 1.0f;
			});
	}
}

double UIManager::toRadians(double grados)
{
	return (grados * MATH_PI) / 180;
}
void UIManager::setEnJornada(bool b)
{
	enJornada = b;
}

void UIManager::cambiaFoco(list<UiButton*>::iterator nuevoFoco)
{
	if (!botones.empty()) {
		auto anteriorFoco = *foco;

		if (nuevoFoco != anteriorFoco->getIterador()) {
			addTween(1.1f, 1.0f, 600.0f, false).via(easing::exponentialOut).onStep([anteriorFoco](tweeny::tween<float>& t, float) mutable {
				anteriorFoco->setDimension(t.peek() * anteriorFoco->getInitialWidth(), t.peek() * anteriorFoco->getInitialHeight());

				return t.progress() == 1.0f;
				});
		}

		foco = nuevoFoco;

		addTween(1.0f, 1.1f, 600.0f, false).via(easing::exponentialOut).onStep([nuevoFoco](tweeny::tween<float>& t, float) mutable {
			(*nuevoFoco)->setDimension(t.peek() * (*nuevoFoco)->getInitialWidth(), t.peek() * (*nuevoFoco)->getInitialHeight());

			return t.progress() == 1.0f;
			});
	}
}

void UIManager::quitaFoco()
{
	if (!botones.empty()) {
		auto anteriorFoco = *foco;
		addTween(1.1f, 1.0f, 600.0f, false).via(easing::exponentialOut).onStep([anteriorFoco](tweeny::tween<float>& t, float) mutable {
			anteriorFoco->setDimension(t.peek() * anteriorFoco->getInitialWidth(), t.peek() * anteriorFoco->getInitialHeight());

			return t.progress() == 1.0f;
			});
	}
}
