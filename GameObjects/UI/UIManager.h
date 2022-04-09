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
	vector<Comanda*> comandas;//comandas de la lista de comandas
	vector<	IngredienteButton*> teclado;//iconos que se usan en la comanda qeu esta redactando
	vector<	UiButton*> uicomandas;
	//vector <GameObject*> pedido;//pedido que se est� redactando lo lleva la comanda

	float uiscale = 1.1;
	vector<GameObject*> pauseMenu; // menú de pausa
	vector<GameObject*> optionsMenu; // menú de opciones
	vector<GameObject*> creditsScreen; // pantalla de créditos
	vector<UiButton*> pauseButtons; // botones del menú de pausa
	vector<UiButton*> optionsButtons; // botones del menú de opciones
	vector<UiButton*> creditsButtons; // botones de la pantalla de créditos
	vector<UiButton*> sliders; // sliders del menú de opciones

	list<tweeny::tween<float>> activeTweens;

	list<Imagen*> cargarAnimacion;

	int mx;
	int my;
	int anchobotones = 25;
	float volumenMusica;
	float volumenSonido;
	Game* game;
	Comanda* actual;
	ListaComandas* barra;

	UiButton* slideMusica;
	UiButton* slideSonido;

	bool clickPrevio = true;

	UiButton* nombre;
	string nombrePlayer = "elbaginon";
	const int maxCaracteres = 15;
	bool escribiendoNombre = false;

	bool enLobby = false;
	uint8_t tiempoCreacion = 2000;
	uint32_t initTime = 0;

	double alfa = 270;

	double toRadians(double grados);

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
	Comanda* getComanda();
	ListaComandas* getBarra();
	void setBarra(ListaComandas* b);
	vector< Point2D<double>> getPosTeclado();
	vector<IngredienteButton*> getTeclado() { return teclado; };
	void setPosTeclado(vector< Point2D<double>> t);
	void creaTeclado();
	void randomizaTeclado();

	void addInterfaz(GameObject* comanda);

	void creaMenuPausa();
	void creaMenuOpciones();
	void creaPantallaCreditos();
	void togglePause();
	void toggleOpciones();
	void toggleCreditos();
	void salirOpciones();
	void salirCreditos();

	void desactivaBot();
	void activaBot();

	vector<GameObject*> getPauseMenu() { return pauseMenu; }
	vector<GameObject*> getInterfaz() { return interfaz; }

	string getNombre() { return nombrePlayer; };

	tweeny::tween<float>& addTween(float from, float to, float during);

	void setEnLobby(bool enLobby_);
};

