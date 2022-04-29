#pragma once
#include "../UI/UiButton.h"
#include "ShowText.h"

class Game;
class IngredienteButton;
class Comanda;
class Texture;
class ListaComandas;
class Imagen;
using namespace std;
using tweeny::easing;

#define MATH_PI 3.14159265359

class UIManager
{
	vector<string> texturasIngredienes = { "alcachofa",  "calamar", "cangrejo", "gamba",
		   "langosta", "mejillon", "pimientoRojo", "pimientoVerde", "pollo" };
	vector<string> texturasNumeros = { "uno",  "dos", "tres", "cuatro",
	   "cinco", "seis", "siete", "ocho", "nueve" };
	vector<string> texturasTamanos = { "small","medium","large" };
	vector<Point2D<double> >posicionesBotones;
	vector<GameObject*> interfaz;//el resto de iconos 
	list<UiButton*> botones;//botones
	vector<Comanda*> comandas;//comandas de la lista de comandas
	vector<UiButton*> teclado;//iconos que se usan en la comanda qeu esta redactando
	vector<UiButton*> uicomandas;
	//vector <GameObject*> pedido;//pedido que se est� redactando lo lleva la comanda

	float uiscale = 1.1;
	GameObject* burnEffect; //Efecto cuando te quemas con una paella caliente
	vector<GameObject*> pauseMenu; // menú de pausa
	vector<GameObject*> optionsMenu; // menú de opciones
	vector<GameObject*> creditsBase; //partes de los créditos que no cambian al cambiar de página
	vector<vector<GameObject*>> creditsScreen; // pantalla de créditos
	vector<UiButton*> pauseButtons; // botones del menú de pausa
	vector<UiButton*> optionsButtons; // botones del menú de opciones
	vector<vector<UiButton*>> creditsButtons; // botones de la pantalla de créditos
	vector<UiButton*> sliders; // sliders del menú de opciones

	list<tweeny::tween<float>> activeTweens;
	list<tweeny::tween<float>> pauseTweens;

	list<Imagen*> cargarAnimacion;

	int mx;
	int my;
	int anchobotones = 25;
	float volumenMusica;
	float volumenSonido;
	Game* game;
	Comanda* actual;
	ListaComandas* barra;

	string musica;
	UiButton* slideMusica;
	UiButton* slideSonido;

	bool clickPrevio = true;
	bool fromMenu = false;
	bool fromRight = true;

	const int posInicialCreditos = 260;
	const int avanceCreditos = 70;
	int paginaCreditos = 0;

	UiButton* nombre;
	UiButton* ip = nullptr;
	GameObject* cursor_;
	string cursor = "|";
	const int maxCaracteres = 15;
	bool escribiendoNombre = false;
	bool escribiendoIP = false;

	bool enLobby = false;
	uint8_t tiempoCreacion = 2000;
	uint32_t initTime = 0;

	double alfa = 270;

	double toRadians(double grados);

	list<UiButton*>::iterator foco;
	bool enAjustes;
	bool enJornada = false, enVentanilla=false;

	Texture* libretaTexture = &sdlutils().images().at("cuadernillo");
	SDL_Rect clip{0,0,libretaTexture->width() / 12,libretaTexture->height()};

	const double TIEMPO_PROCESADO = 1000.0;
	int i = 0;
	double sigAnimg = 0.0;
	double tiempo = 0.0;

public:
	UIManager(Game* game);
	~UIManager();
	void uiEvent(int mx, int my, bool& exit, bool paused = false);
	void slideEvent(int mx, int my, bool& exit, bool paused = false);

	void handleInput(bool& exit, bool paused = false);
	void update(bool paused);

	// void render();
	void render(SDL_Rect* rect);

	void creaComanda(Game* game);
	void creaComandaVentanilla(Game* game);
	Comanda* getComanda();
	ListaComandas* getBarra();
	void setBarra(ListaComandas* b);
	vector< Point2D<double>> getPosTeclado();
	vector<UiButton*> getTeclado() { return teclado; };
	void setPosTeclado(vector< Point2D<double>> t);
	void creaTeclado();
	void randomizaTeclado();

	void addInterfaz(GameObject* comanda);
	void addButton(UiButton* button);

	void creaMenuPausa();
	void creaMenuOpciones();
	void creaPantallaCreditos();
	void togglePause();
	void toggleOpciones();
	void toggleCreditos(int pagina);
	void salirCreditos();

	void tweenSetter(GameObject* i, bool enAjustes_);

	void desactivaBot();
	void activaBot();

	vector<UiButton*> getPauseButtons() { return pauseButtons; }
	vector<GameObject*> getInterfaz() { return interfaz; }

	//string getNombre() { return nombrePlayer; };

	tweeny::tween<float>& addTween(float from, float to, float during,bool activeTweens_);

	void setEnLobby(bool enLobby_);
	void setFromMenu(bool fromMenu_);
	void setFromRight(bool fromRight_);

	void quemarse();
	void focoExecute(bool& exit);
	void avanzaFoco();
	void retrocedeFoco();
	void cambiaFoco(list<UiButton*>::iterator nuevoFoco);
	void quitaFoco();

	bool getEnAjustes() { return enAjustes; };
	void setEnJornada(bool b);

	void setIpButton(UiButton *ip_);
};

