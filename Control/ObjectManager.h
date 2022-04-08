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
	Pool<GrupoClientes> *grupoClientes;
	Pool<Cliente> *clientes;
	Pool<Ingrediente> *ingredientes;
	Pool<Arroz> *arroces;

	vector<Paella*> paellas;

	vector<Mueble*> muebles;
	
	vector<Player*> players;

	vector<GameObject*> renderAll;

	Game* game;

public:
	ObjectManager(Game* game);
	~ObjectManager();

	void handleInput(bool& exit);

	void update();
	void refresh();
	void render(SDL_Rect* rect);
	void debug(SDL_Rect* rect);

	void addMueble(Mueble* mueble);
	Paella* addPaella(int n);

	void addPlayer(Player* player);

	Pool<Cliente>* getPoolClientes() { return clientes; }
	Pool<GrupoClientes>* getPoolGrupoClientes() { return grupoClientes; }
	Pool<Ingrediente>* getPoolIngredientes() { return ingredientes; }
	Pool<Arroz>* getPoolArroces() { return arroces; }

	vector<Mueble*> getMueblesCollisions(SDL_Rect collider);
	vector<Mueble*> getMueblesOverlaps(SDL_Rect collider);

	vector<Player*> getPlayers() { return players; }

	Player* getHost() { 
		if (players.size() > 0) return players[0];
		return nullptr;
	}

	void initMuebles();

	void sortAllGO();
};

