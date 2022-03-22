#pragma once
#include "../Data/Pool.h"
#include "../GameObjects/Ingrediente.h"
#include "../GameObjects/Cliente.h"
#include "../GameObjects/GrupoClientes.h"
#include "../GameObjects/Paella.h"
#include "../GameObjects/Player.h"

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

	vector<Player*> players;

	//Para formar el rect del raton
	int x, y, range = 2;

public:
	ObjectManager(Game* game);
	~ObjectManager();

	void handleInput();
	void update();

	void render(SDL_Rect* rect);
	void debug(SDL_Rect* rect);

	void addMueble(GameObject* mueble);
	void addPaella(GameObject *paella);
	void addComanda(GameObject* comanda);
	void addInterfaz(GameObject* interfaz);

	void addPlayer(Player* player);

	Pool<Cliente>* getPoolClientes() { return clientes; }
	Pool<GrupoClientes>* getPoolGrupoClientes() { return grupoClientes; }
	Pool<Ingrediente>* getPoolIngredientes() { return ingredientes; }

	vector<GameObject*> getPaellas() { return paellas; }

	vector<Player*> getPlayers() { return players; }

	Player* getHost() { 
		if (players.size() > 0) return players[0];
		return nullptr;
	}

	void initMuebles();
	vector<Collider*> getMueblesCollider();
	vector<Collider*> getMueblesCollider(SDL_Rect collider);
	vector<Collider*> getPaellasCollider(SDL_Rect collider);
};

