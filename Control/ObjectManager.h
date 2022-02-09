#pragma once
#include "../Data/Pool.h"
#include "../GameObjects/Ingrediente.h"
#include <queue>
#include <vector>


///class Ingrediente;
//class GrupoClientes;
//class Paella;
class Game;

using namespace std;

class ObjectManager
{
	//Vector de paelleras
	// 
	//vector<Paella*> paelleras;

	//Pool de clientes
	// 
	//Pool<GrupoClientes*> clientes;
	//queue<GrupoClientes*> cola;
	//int capacidadCola;

	//Pool de ingredientes

	Pool<Ingrediente> *ingredientes;

	vector<GameObject*> muebles;

public:
	ObjectManager(Game* game);
	~ObjectManager();

	void render();
	void update();
};

