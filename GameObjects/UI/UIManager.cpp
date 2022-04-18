#include "UIManager.h"
#include "../../Data/Comanda.h"
#include "../../GameObjects/UI/RedactaComandabutton.h"
#include "../../sdlutils/InputHandler.h"
#include "../UI/Ingredientebutton.h"
#include "../UI/Numerobutton.h"
#include "../UI/Tamanobutton.h"
#include "DescartaCommandaButton.h"
#include "EnviaComandaButton.h"
#include "BorrarButton.h"
#include "AceptaPaellaButton.h"
#include "EliminaComandaButton.h"
#include "OcultabarraButton.h"
#include "../../Data/ListaComandas.h"

#include "../../Scenes/Jornada.h"
#include "../../Scenes/Tutorial.h"
#include "../../Scenes/Menu.h"

#include "../../Utils/Texture.h"

#include <iostream>

UIManager::UIManager(Game* game)
{
	this->game = game;
	anchobotones *= uiscale;

	barra = nullptr;

	burnEffect = new Imagen(game, sdlutils().width() / 2, sdlutils().height() / 2, 
		sdlutils().width() * 2, sdlutils().height() * 2, "quemadura");
	burnEffect->setInitialDimension(sdlutils().width(), sdlutils().height());
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
}

void UIManager::update(bool paused)
{
	if (!paused ) {
		for (auto i : interfaz)
			if (i->isActive())
				i->update();
	}

	if (enLobby && sdlutils().virtualTimer().currTime() - initTime >= tiempoCreacion) {

		double x = cos(toRadians(alfa));
		double y = sin(toRadians(alfa));

		Imagen* circulo = new Imagen(game, sdlutils().width()/2 + x*50, sdlutils().height() - 75 + y*50, 16, 16, "circulo");

		circulo->setInitialDimension(16,16);

		addTween(1.0f, 0.75f, 800.0f).via(easing::exponentialOut).onStep([circulo, this](tweeny::tween<float>& t, float) mutable {
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
	while (i != activeTweens.end()) {
		i->step(20);

		//Si el tween ha acabado, lo saco de la lista de tweens
		if (i->progress() == 1) {
			//Erase devuelve el iterador al siguiente tween
			i = activeTweens.erase(i);
		}
		else
			i++;
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

	for (auto i : creditsBase) {
		if (i->isActive()) i->render(rect);
	}

	for (auto i : creditsScreen) {
		for (auto j : i) {
			if (j->isActive()) j->render(rect);
		}
	}
	
	for (auto it = cargarAnimacion.begin(); it != cargarAnimacion.end(); it++) {
		auto i = *it;
		i->render(rect);
	}
}

void UIManager::creaComanda(Game* game)
{
	actual = new Comanda(game, uiscale, this);
	comandas.push_back(actual);

	creaTeclado();
	actual->guardaTeclado();
	actual->toggleTeclado(false);
	actual->toggleTecaldotam(false);

	AceptaPaellaButton* aceptaPaellaButton = new AceptaPaellaButton(game, actual, "acepta", actual->getPosition().getX() + actual->getWidth() / 2 + anchobotones / 4, actual->getPosition().getY() + actual->getHeight() / 4, uiscale * anchobotones, uiscale * anchobotones);
	interfaz.push_back(aceptaPaellaButton);
	actual->guardaBoton(aceptaPaellaButton);
	DescartaCommandaButton* descartaComandaButton = new DescartaCommandaButton(actual, game, "cancela", actual->getPosition().getX() + actual->getWidth() / 2 + anchobotones / 4, actual->getPosition().getY() + actual->getHeight() / 4 - anchobotones, uiscale * anchobotones, uiscale * anchobotones);
	interfaz.push_back(descartaComandaButton);
	actual->guardaBoton(descartaComandaButton);
	BorrarButton* borraComandaButton = new BorrarButton(game, actual, "borra", actual->getPosition().getX() + actual->getWidth() / 2 + anchobotones / 4, actual->getPosition().getY() + actual->getHeight() / 4 - 2 * anchobotones, uiscale * anchobotones, uiscale * anchobotones);
	interfaz.push_back(borraComandaButton);
	actual->guardaBoton(borraComandaButton);
	EnviaComandaButton* enviaComandaButton = new EnviaComandaButton(game, "envia", actual->getPosition().getX() + actual->getWidth() / 2 + anchobotones / 4, actual->getPosition().getY() + actual->getHeight() / 4 + anchobotones, uiscale * anchobotones, uiscale * anchobotones);
	interfaz.push_back(enviaComandaButton);
	actual->guardaBoton(enviaComandaButton);

	if (dynamic_cast<Tutorial*>(game->getCurrentScene()) && game->getCurrentScene()->getState() == States::abreLibreta)
		game->getCurrentScene()->changeState(States::pauasAbreLibreta);

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
	OcultabarraButton* o = new OcultabarraButton(this, game, "barraboton", b->getX() + b->getWidth()/2 + uiscale*anchobotones, b->getY(), uiscale * anchobotones, uiscale * anchobotones, b);
	addButton(o);
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
	for (auto i : texturasIngredienes)
	{
		//Comanda comanda,Game* game, TextureName texturename, int x, int y, int w, int h
		IngredienteButton* a = new IngredienteButton(this, game, i, (int)posicionesBotones[j].getX(), (int)posicionesBotones[j].getY(), uiscale * anchobotones, uiscale * anchobotones);
		teclado.push_back(a);
		//  objectmanager->creaTeclado(a);

		j++;
	}
	j = 0;
	vector <NumeroButton*> tecladonum;

	for (auto i : texturasNumeros)
	{
		//Comanda comanda,Game* game, TextureName texturename, int x, int y, int w, int h
		NumeroButton* a = new NumeroButton(this, game, i, (int)posicionesBotones[j].getX(), (int)posicionesBotones[j].getY(), uiscale * anchobotones, uiscale * anchobotones);
		tecladonum.push_back(a);
		uicomandas.push_back(a);
		j++;
	}

	actual->guardaTecladonum(tecladonum);

	j = 0;
	vector <TamanoButton*> tecladotam;
	for (auto i : texturasTamanos)
	{
		TamanoButton* a = new TamanoButton(this, game, i, (int)posicionesBotones[j].getX(), (int)posicionesBotones[j].getY(), uiscale * anchobotones, uiscale * anchobotones);

		tecladotam.push_back(a);
		uicomandas.push_back(a);
		j++;
	}
	actual->guardaTecladotam(tecladotam);

}

void UIManager::randomizaTeclado()
{
	vector<Point2D<double>> posdis = posicionesBotones;
	int j = rand() % posdis.size();
	for (auto i : teclado)
	{
		i->setPosition(posdis[j]);
		posdis.erase(posdis.begin() + j);
		if (posdis.size() > 0)
			j = rand() % posdis.size();
	}
}

void UIManager::addInterfaz(GameObject* elementoInterfaz)
{
	interfaz.push_back(elementoInterfaz);
}

void UIManager::addButton(UiButton* button)
{
	botones.push_back(button);

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
	Imagen* bg = new Imagen(game, sdlutils().width() / 2, sdlutils().height() / 2, sdlutils().width(), sdlutils().height(), "pause2");
	bg->setActive(false);
	pauseMenu.push_back(bg);

	Imagen* pauseImage = new Imagen(game, sdlutils().width() / 2, sdlutils().height() / 2, 300, 300, "pause1");
	pauseImage->setActive(false);
	pauseMenu.push_back(pauseImage);

	UiButton* resumeButton = new UiButton(game, "resumeBoton", sdlutils().width() / 2, sdlutils().height() / 2 - 25, 200, 100);
	resumeButton->setInitialDimension(200, 100);
	resumeButton->setActive(false);

	resumeButton->setAction([this, resumeButton](Game* game, bool& exit) {
		// Pausa
		Jornada* currentScene = dynamic_cast<Jornada*>(game->getCurrentScene());

		addTween(0.9f, 1.0f, 600.0f).via(easing::exponentialOut).onStep([resumeButton, currentScene](tweeny::tween<float>& t, float) mutable {
			resumeButton->setDimension(t.peek() * resumeButton->getInitialWidth(), t.peek() * resumeButton->getInitialHeight());

			if (t.progress() > .2f) { //Busco una respuesta mas rapida
				// Resume
				currentScene->togglePause();
				return true;
			}
			return false;
			});
		});

	pauseButtons.push_back(resumeButton);

	UiButton* exitButton = new UiButton(game, "exitBoton", sdlutils().width() / 2, sdlutils().height() / 2 + 30, 200, 100);
	exitButton->setInitialDimension(200, 100);
	exitButton->setActive(false);

	exitButton->setAction([this, exitButton](Game* game, bool& exit) mutable {
		sdlutils().soundEffects().at("select").play(0, game->UI);

		addTween(0.9f, 1.0f, 600.0f).via(easing::exponentialOut).onStep([this, game, exitButton](tweeny::tween<float>& t, float) mutable {
			exitButton->setDimension(t.peek() * exitButton->getInitialWidth(), t.peek() * exitButton->getInitialHeight());

			
			if (t.progress() == 1.0f) {
				// Exit

				game->getNetworkManager()->close();
				game->sendMessageScene(new Menu(game));

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
	Imagen* opcPant = new Imagen(game, sdlutils().width() / 2, sdlutils().height() / 2, sdlutils().width() - 100, sdlutils().height()-100, "fondoOpc");
	
	opcPant->setActive(false);

	optionsMenu.push_back(opcPant);

	//Boton salir
	UiButton* botonSalir = new UiButton(game, "cerrarOpc", opcPant->getWidth(), 100, 100, 100);

	botonSalir->setActive(false);
	botonSalir->setAction([this, botonSalir](Game* game, bool& exit) {
		activaBot();
		salirOpciones();
		});

	optionsMenu.push_back(botonSalir);
	optionsButtons.push_back(botonSalir);

	//Boton pantalla completa
	UiButton* pantCompleta = new UiButton(game, "reloj", sdlutils().width() / 2 + sdlutils().width()/5, opcPant->getHeight() - 100, 100, 100);

	pantCompleta->setActive(false);

	pantCompleta->setAction([this, pantCompleta](Game* game, bool& exit) {
		sdlutils().toggleFullScreen();
		});

	optionsButtons.push_back(pantCompleta);
	optionsMenu.push_back(pantCompleta);

	//Texto pantalla completa
	Imagen* textoPCompleta = new Imagen(game, pantCompleta->getX() - pantCompleta->getWidth() / 2 - 75, pantCompleta->getY(), 125, 50, "PCompleta");

	textoPCompleta->setActive(false);

	optionsMenu.push_back(textoPCompleta);

	//Barra Musica
	Imagen* barraVol_musica = new Imagen(game, sdlutils().width() / 2 + 75, sdlutils().height() / 2 - 100, 700, 30, "barraVol");

	barraVol_musica->setActive(false);

	optionsMenu.push_back(barraVol_musica);

	//Texto barra musica
	Imagen* textoMusica = new Imagen(game, barraVol_musica->getX() - barraVol_musica->getWidth()/2 - 75, barraVol_musica->getY(), 125, 50, "musica");

	textoMusica->setActive(false);

	optionsMenu.push_back(textoMusica);

	//Barra sonido
	Imagen* barraVol_sonido = new Imagen(game, sdlutils().width() / 2 + 75, sdlutils().height() / 2, 700, 30, "barraVol");

	barraVol_sonido->setActive(false);

	optionsMenu.push_back(barraVol_sonido);

	//Texto barra sonido
	Imagen* textoSonido = new Imagen(game, barraVol_sonido->getX() - barraVol_sonido->getWidth() / 2 - 75, barraVol_sonido->getY(), 125, 50, "sonido");

	textoSonido->setActive(false);

	optionsMenu.push_back(textoSonido);

	//Slide musica
	int x = barraVol_musica->getX(), y = barraVol_musica->getY();

	if (game->getSlideMus().getX() != 0) {
		x = game->getSlideMus().getX();
		y = game->getSlideMus().getY();
	}

	slideMusica = new UiButton(game, "paella", x, y, 80, 80);

	slideMusica->setActive(false);
	slideMusica->setAction([this, barraVol_musica](Game* game, bool& exit) {

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
		});

	optionsMenu.push_back(slideMusica);
	sliders.push_back(slideMusica);

	//Slide sonido
	x = barraVol_sonido->getX(), y = barraVol_sonido->getY();

	if (game->getSlideSon().getX() != 0) {
		x = game->getSlideSon().getX();
		y = game->getSlideSon().getY();
	}

	slideSonido = new UiButton(game, "paella", x, y, 80, 80);

	slideSonido->setActive(false);
	slideSonido->setAction([this, barraVol_sonido](Game* game, bool& exit) {

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
		});

	optionsMenu.push_back(slideSonido);
	sliders.push_back(slideSonido);

	game->setSlidesPos(slideSonido->getPosition(), slideMusica->getPosition());

	//Caja de texto del nombre
	Imagen* fondoNombre = new Imagen(game, sdlutils().width() / 2 - 100, opcPant->getHeight() - 100, 210, 50, "reloj");

	fondoNombre->setActive(false);

	optionsMenu.push_back(fondoNombre);

	//Boton para escribir el nombre
	nombre = new UiButton(game, game->getNombre(), "abadiNombre", { 255, 255, 255, 255 }, { 0, 0, 0, 0 }, sdlutils().width() / 2 - 100, opcPant->getHeight() - 100);
	cursor_ = new GameObject(game);

	nombre->setActive(false);
	
	nombre->setAction([this](Game* game, bool& exit) {

		char c = ih().getTypedKey();
		string nombrePlayer = game->getNombre();

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

			game->setNombre(nombrePlayer);

			nombre->setTexture(nombrePlayer, string("abadiNombre"), { 255, 255, 255, 255 }, { 0, 0, 0, 0 });
			nombre->setDimension();

			//cursor_->setPosition(nombre->getX() + nombre->getWidth(), nombre->getY());

			//cursor_->setTexture(cursor, string("abadiNombre"), { 255, 255, 255, 255 }, { 0, 0, 0, 0 });
			//cursor_->setDimension();
		}
		nombre->render(nullptr);
		//cursor_->render(nullptr);
		});

	optionsMenu.push_back(nombre);

	//Texto nombre
	Imagen* textoNombre = new Imagen(game, fondoNombre->getX() - fondoNombre->getWidth() / 2 - 75, fondoNombre->getY(), 125, 50, "nombre");

	textoNombre->setActive(false);

	optionsMenu.push_back(textoNombre);
}

void UIManager::creaPantallaCreditos()
{
	creditsScreen.push_back({});
	creditsButtons.push_back({});

	//Fondo De las opciones
	Imagen* opcPant = new Imagen(game, sdlutils().width() / 2, sdlutils().height() / 2, sdlutils().width() - 100, sdlutils().height() - 100, "fondoOpc");

	opcPant->setActive(false);

	creditsBase.push_back(opcPant);

	//Boton salir
	UiButton* botonSalir = new UiButton(game, "cerrarOpc", opcPant->getWidth(), 100, 100, 100);

	botonSalir->setActive(false);
	botonSalir->setAction([this, botonSalir](Game* game, bool& exit) {
		activaBot();
		salirCreditos();
		});

	creditsBase.push_back(botonSalir);
	creditsButtons[0].push_back(botonSalir);

	//Boton siguiente página
	UiButton* botonNext = new UiButton(game, "nextCredits", opcPant->getWidth(), sdlutils().height() / 2, 100, 100);

	botonNext->setActive(false);
	botonNext->setAction([this, botonNext](Game* game, bool& exit) {
		toggleCreditos(paginaCreditos + 1);
		});

	creditsScreen[0].push_back(botonNext);
	creditsButtons[0].push_back(botonNext);

	//Título de desarrolladores

	Imagen* fondoTitulo = new Imagen(game, sdlutils().width() / 2, 180, 260, 60, "reloj");

	fondoTitulo->setActive(false);

	creditsBase.push_back(fondoTitulo);

	ShowText* TituloDev = new ShowText(game, "Desarrolladores", "paella",
		sdlutils().width() / 2, 190, 0, 0);

	TituloDev->setActive(false);

	creditsScreen[0].push_back(TituloDev);

	//Nombres de los desarrolladores

	Imagen* fondoColumna1 = new Imagen(game, sdlutils().width() / 3, posInicialCreditos + avanceCreditos * 2, 360, 360, "reloj");

	fondoColumna1->setActive(false);

	creditsBase.push_back(fondoColumna1);

	Imagen* fondoColumna2 = new Imagen(game, 2 * (sdlutils().width()) / 3, posInicialCreditos + avanceCreditos * 2, 360, 360, "reloj");

	fondoColumna2->setActive(false);

	creditsBase.push_back(fondoColumna2);

	ShowText* Dev1 = new ShowText(game, "Javier Cano", "abadiNombre",
		sdlutils().width() / 3, posInicialCreditos,0,0);

	Dev1->setActive(false);

	creditsScreen[0].push_back(Dev1);

	ShowText* Dev2 = new ShowText(game, "José Miguel Villacañas", "abadiNombre",
		2*(sdlutils().width()) / 3, posInicialCreditos, 0, 0);

	Dev2->setActive(false);

	creditsScreen[0].push_back(Dev2);

	ShowText* Dev3 = new ShowText(game, "Víctor Manuel Estremera", "abadiNombre",
		sdlutils().width() / 3, posInicialCreditos + avanceCreditos, 0, 0);

	Dev3->setActive(false);

	creditsScreen[0].push_back(Dev3);

	ShowText* Dev4 = new ShowText(game, "Diego Rol", "abadiNombre",
		2 * (sdlutils().width()) / 3, posInicialCreditos + avanceCreditos, 0, 0);

	Dev4->setActive(false);

	creditsScreen[0].push_back(Dev4);

	ShowText* Dev5 = new ShowText(game, "Rodrigo Cabello", "abadiNombre",
		sdlutils().width() / 3, posInicialCreditos + avanceCreditos * 2, 0, 0);

	Dev5->setActive(false);

	creditsScreen[0].push_back(Dev5);

	ShowText* Dev6 = new ShowText(game, "Rocío Sanchez-Horcajuelo", "abadiNombre",
		2 * (sdlutils().width()) / 3, posInicialCreditos + avanceCreditos * 2, 0, 0);

	Dev6->setActive(false);

	creditsScreen[0].push_back(Dev6);

	ShowText* Dev7 = new ShowText(game, "Marta Croche", "abadiNombre",
		sdlutils().width() / 3, posInicialCreditos + avanceCreditos * 3, 0, 0);

	Dev7->setActive(false);

	creditsScreen[0].push_back(Dev7);

	ShowText* Dev8 = new ShowText(game, "Pablo Arredondo", "abadiNombre",
		2 * (sdlutils().width()) / 3, posInicialCreditos + avanceCreditos * 3, 0, 0);

	Dev8->setActive(false);

	creditsScreen[0].push_back(Dev8);

	ShowText* Dev9 = new ShowText(game, "Elena Robert", "abadiNombre",
		sdlutils().width() / 3, posInicialCreditos + avanceCreditos * 4, 0, 0);

	Dev9->setActive(false);

	creditsScreen[0].push_back(Dev9);

	ShowText* Dev10 = new ShowText(game, "Alfonso Rodulfo", "abadiNombre",
		2 * (sdlutils().width()) / 3, posInicialCreditos + avanceCreditos * 4, 0, 0);

	Dev10->setActive(false);

	creditsScreen[0].push_back(Dev10);

	//Página 1 de créditos

	creditsScreen.push_back({});
	creditsButtons.push_back({});
	//Boton página anterior
	UiButton* botonNext2 = new UiButton(game, "nextCredits", opcPant->getWidth(), sdlutils().height() / 2, 100, 100);

	botonNext2->setActive(false);
	botonNext2->setAction([this, botonNext2](Game* game, bool& exit) {
		toggleCreditos(paginaCreditos + 1);
		});

	creditsScreen[1].push_back(botonNext2);
	creditsButtons[1].push_back(botonNext2);

	//Boton página anterior
	UiButton* botonPrevious = new UiButton(game, "previousCredits", sdlutils().width() - opcPant->getWidth(), sdlutils().height() / 2, 100, 100);

	botonPrevious->setActive(false);
	botonPrevious->setAction([this, botonPrevious](Game* game, bool& exit) {
		toggleCreditos(paginaCreditos - 1);
		});

	creditsScreen[1].push_back(botonPrevious);
	creditsButtons[1].push_back(botonPrevious);

	//Título de prueba

	ShowText* TituloPrueba = new ShowText(game, "Prueba_1", "paella",
		sdlutils().width() / 2, 190, 0, 0);

	TituloPrueba->setActive(false);

	creditsScreen[1].push_back(TituloPrueba);

	//Página final de créditos.

	creditsScreen.push_back({});
	creditsButtons.push_back({});

	//Boton página anterior
	UiButton* botonPrevious2 = new UiButton(game, "previousCredits", sdlutils().width() - opcPant->getWidth(), sdlutils().height() / 2, 100, 100);

	botonPrevious2->setActive(false);
	botonPrevious2->setAction([this, botonPrevious2](Game* game, bool& exit) {
		toggleCreditos(paginaCreditos - 1);
		});

	creditsScreen[2].push_back(botonPrevious2);
	creditsButtons[2].push_back(botonPrevious2);

	//Título de prueba 2

	ShowText* TituloPrueba2 = new ShowText(game, "Prueba_2", "paella",
		sdlutils().width() / 2, 190, 0, 0);

	TituloPrueba2->setActive(false);

	creditsScreen[2].push_back(TituloPrueba2);

}

void UIManager::togglePause() {
	for (auto i : pauseMenu) {
		i->setActive(!i->isActive());
	}

	//Solo una imagen tiene el tween
	auto pauseImage = pauseMenu[1];
	pauseImage->setDimension(0, 0);

	if (pauseImage->isActive()) {
		addTween(0.8f, 1.0f, 600.0f).via(easing::exponentialOut).onStep([pauseImage](tweeny::tween<float>& t, float) mutable {
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
			addTween(0.8f, 1.0f, 600.0f).via(easing::exponentialOut).onStep([i](tweeny::tween<float>& t, float) mutable {
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
	for (auto i : optionsMenu) {
		i->setActive(true);
	}
}
void UIManager::toggleCreditos(int pagina)
{
	for (auto i : creditsScreen[paginaCreditos]) {
		i->setActive(false);
	}
	for (auto i : creditsScreen[pagina]) {
		i->setActive(true);
	}
	for (auto i : creditsBase) {
		i->setActive(true);
	}
	paginaCreditos = pagina;
}

void UIManager::salirOpciones()
{
	for (auto i : optionsMenu) {
		i->setActive(false);
	}

	escribiendoNombre = false;
}
void UIManager::salirCreditos()
{
	for (auto i : creditsScreen) {
		for (auto j : i) {
			j->setActive(false);
		}
	}
	for (auto i : creditsBase) {
		i->setActive(false);
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

tweeny::tween<float>& UIManager::addTween(float from, float to, float during) {
	tweeny::tween<float> tween = tweeny::tween<float>::from(from).to(to).during(during);
	activeTweens.push_front(tween);

	return activeTweens.front();
}

void UIManager::setEnLobby(bool enLobby_)
{
	enLobby = enLobby_;
}

void UIManager::quemarse()
{
	//Aparece el efecto de quemarse en la interfaz y suena un sonido
	addTween(2, 1, 300).via(easing::cubicOut).onStep([this](tweeny::tween<float>& t, float) mutable {
		burnEffect->setDimension(t.peek() * burnEffect->getInitialWidth(), t.peek() * burnEffect->getInitialHeight());

		if (t.progress() == 1.0f) {
			addTween(1, 2, 5000).via(easing::sinusoidalIn).onStep([this](tweeny::tween<float>& t, float) mutable {
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
		addTween(1.1f, 1.0f, 600.0f).via(easing::exponentialOut).onStep([anteriorFoco](tweeny::tween<float>& t, float) mutable {
			anteriorFoco->setDimension(t.peek() * anteriorFoco->getInitialWidth(), t.peek() * anteriorFoco->getInitialHeight());

			return t.progress() == 1.0f;
			});

		foco++;
		if (foco == botones.end())
			foco = botones.begin();

		addTween(1.0f, 1.1f, 600.0f).via(easing::exponentialOut).onStep([this](tweeny::tween<float>& t, float) mutable {
			(*foco)->setDimension(t.peek() * (*foco)->getInitialWidth(), t.peek() * (*foco)->getInitialHeight());

			return t.progress() == 1.0f;
			});
	}
}

void UIManager::retrocedeFoco()
{
	if (!botones.empty()) {
		auto anteriorFoco = *foco;
		addTween(1.1f, 1.0f, 600.0f).via(easing::exponentialOut).onStep([anteriorFoco](tweeny::tween<float>& t, float) mutable {
			anteriorFoco->setDimension(t.peek() * anteriorFoco->getInitialWidth(), t.peek() * anteriorFoco->getInitialHeight());

			return t.progress() == 1.0f;
			});

		if (foco == botones.begin())
			foco = botones.end() - 1;
		else 
			foco--;

		addTween(1.0f, 1.1f, 600.0f).via(easing::exponentialOut).onStep([this](tweeny::tween<float>& t, float) mutable {
			(*foco)->setDimension(t.peek() * (*foco)->getInitialWidth(), t.peek() * (*foco)->getInitialHeight());

			return t.progress() == 1.0f;
			});
	}
}

double UIManager::toRadians(double grados)
{
	return (grados * MATH_PI) / 180;
}
