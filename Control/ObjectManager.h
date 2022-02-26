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
	//Vector de paelleras
	//vector<Paella*> paelleras;

	//Pool de clientes
	Pool<Cliente> *clientes;
	Pool<GrupoClientes> *grupoClientes;

	//Pool de ingredientes
	Pool<Ingrediente> *ingredientes;

	//esto va a ser nuestra version de capas
	//en el orden en el que lo renderizamos nos aseguramos que la UI esta por encima de todo
	vector<GameObject*> muebles;
	vector<GameObject*> interfaz;
	vector<GameObject*> comandas; 

public:
	ObjectManager(Game* game);
	~ObjectManager();

	void handleInput();
	void update();
	void render();

	void uiEvent(int mx,int my);
	void creaComanda(Game*game);

	vector<Collider*> getClientes(SDL_Rect gOC);
};

