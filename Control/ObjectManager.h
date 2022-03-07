#pragma once
#include "../Data/Pool.h"
#include "../GameObjects/Ingrediente.h"
#include "../GameObjects/Cliente.h"
#include "../GameObjects/GrupoClientes.h"
#include <queue>
#include <vector>

class Game;

using namespace std;

class ObjectManager
{
	//esto va a ser nuestra version de capas
	//en el orden en el que lo renderizamos nos aseguramos que la UI esta por encima de todo
	Pool<GrupoClientes> *grupoClientes;
	Pool<Cliente> *clientes;
	Pool<Ingrediente> *ingredientes;
	vector<GameObject*> muebles;
	vector<GameObject*> paellas;
	vector<GameObject*> interfaz;
	vector<GameObject*> comandas; 

	//Para formar el rect del raton
	int x, y, range = 2;

public:
	ObjectManager(Game* game);
	~ObjectManager();

	void handleInput();
	void update();
	void render();
	void debug();

	vector<Collider*> getGrupoClientes(SDL_Rect gOC);
	vector<Collider*> getClientes(SDL_Rect gOC);
	vector<Collider*> getIngredientes(SDL_Rect gOC);

	void addMueble(GameObject* mueble);
	//void addPaella(GameObject* paella);
	//void addComanda(GameObject* comanda);
	//void addInterfaz(GameObject* interfaz);

	Pool<Cliente>* getPoolClientes() { return clientes; };
	Pool<GrupoClientes>* getPoolGrupoClientes() { return grupoClientes; };
	Pool<Ingrediente>* getPoolIngredientes() { return ingredientes; };
};

