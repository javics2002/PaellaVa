#pragma once
#include "../GameObjects/GameObject.h"
#include <list>
#include <vector>
#include<queue>

class Comanda;
class UIManager;
class Game;

using namespace std;

class ListaComandas :public GameObject
{
	int maxvisibles = 4;
	int  numcomandas=0;
	vector<Comanda*> lista;
	queue<Comanda*> listanovisibles;
public:
	//Añade la comanda que escribe el camarero a la lista
	ListaComandas(Game* game, UIManager* m);
	~ListaComandas();
	void AñadeComanda(Comanda* comanda);
	void finalizacomanda(Comanda* comanda);
	//Interfaz
	void renderComandas();
	void update() override;
	vector<Comanda*> getlista() { return lista; };
private:
	int ancho = 400;
	int alto = 50;
	int cX = 0;
	int cY = 0;
	int desplazamineto = 0;
	UIManager* uimt;
};

