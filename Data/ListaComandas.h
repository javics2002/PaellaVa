#pragma once
#include <list>

class Comanda;

using namespace std;

class ListaComandas
{
	list<Comanda*> lista;
public:
	//Añade la comanda que escribe el camarero a la lista
	void AñadeComanda(Comanda* comanda);

	//Interfaz
	void render();
};

