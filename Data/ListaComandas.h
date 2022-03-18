#pragma once
#include "../GameObjects/GameObject.h"
#include <list>
#include <vector>

class Comanda;
class Game;

using namespace std;

class ListaComandas :public GameObject
{
	vector<Comanda> lista;
public:
	//Añade la comanda que escribe el camarero a la lista
	ListaComandas(Game* game );
	~ListaComandas();
	void AñadeComanda(Comanda* comanda);

	//Interfaz
	void renderComandas();
	
private:
	int ancho = 400;
	int alto = 50;
	int cX = 0;
	int cY = 0;
	int desplazamineto = 0;
};

