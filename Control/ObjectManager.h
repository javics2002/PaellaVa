#pragma once
#include "../Data/Pool.h"
#include "../GameObjects/Ingrediente.h"
#include "../GameObjects/Cliente.h"
#include "../GameObjects/GrupoClientes.h"
#include "../GameObjects/Paella.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Arroz.h"

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
	Pool<Arroz> *arroces;
	vector<GameObject*> muebles;
	vector<GameObject*> paellas;

	vector<Player*> players;

	vector<GameObject*> renderAll;

	//Para formar el rect del raton
	int x = 0, y = 0, range = 2;

	Game* game;

public:
	ObjectManager(Game* game);
	~ObjectManager();

	void handleInput(bool& exit);
	void update();

	void render(SDL_Rect* rect);
	void debug(SDL_Rect* rect);

	void addMueble(GameObject* mueble);
	Paella* addPaella(int n);

	void addPlayer(Player* player);

	Pool<Cliente>* getPoolClientes() { return clientes; }
	Pool<GrupoClientes>* getPoolGrupoClientes() { return grupoClientes; }
	Pool<Ingrediente>* getPoolIngredientes() { return ingredientes; }
	Pool<Arroz>* getPoolArroces() { return arroces; }

	vector<GameObject*> getPaellas() { return paellas;}

	vector<Player*> getPlayers() { return players; }

	Player* getHost() { 
		if (players.size() > 0) return players[0];
		return nullptr;
	}

	void initMuebles();
	vector<GameObject*> getMuebles() { return muebles; }
	vector<Collider*> getMueblesCollider();
	vector<Collider*> getMueblesCollider(SDL_Rect collider);
	vector<Collider*> getPaellasCollider(SDL_Rect collider);

	void sortAllGO();
};

