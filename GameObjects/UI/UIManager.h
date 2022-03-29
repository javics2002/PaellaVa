#pragma once
#include "../UI/UiButton.h"
class Game;
class IngredienteButton;
class Comanda;
class ListaComandas;
using namespace std;

class UIManager
{
	vector<string> texturasIngredienes = { "alcachofa",  "calamar", "cangrejo", "gamba",
		   "langosta", "mejillon", "pimientoRojo", "pimientoVerde", "pollo" };
	vector<string> texturasNumeros= { "uno",  "dos", "tres", "cuatro",
	   "cinco", "seis", "siete", "ocho", "nueve" };
	vector<string> texturasTamanos = {"small","medium","large"};
	vector<Point2D<double> >posicionesBotones;
	vector<GameObject*> interfaz;//el resto de iconos 
	vector<Comanda*> comandas;//comandas de la lista de comandas
	vector<	IngredienteButton*> teclado;//iconos que se usan en la comanda qeu esta redactando
	vector<	UiButton*> uicomandas;
	//vector <GameObject*> pedido;//pedido que se est� redactando lo lleva la comanda

	vector<GameObject*> pauseMenu; // menú de pausa
	vector<UiButton*> pauseButtons; // botones del menú de pausa

	vector<tweeny::tween<int>> activeTweens;


	int mx;
	int my;
	int anchobotones = 25;
	Game* game;
	Comanda* actual;
	ListaComandas* barra;

public:
	UIManager(Game* game);
	~UIManager();
	void uiEvent(int mx, int my, bool& exit);

	void handleInput(bool& exit);
	void update();

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
	void togglePause();

	vector<GameObject*> getPauseMenu() { return pauseMenu; }

};

