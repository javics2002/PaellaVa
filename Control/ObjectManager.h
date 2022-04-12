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

enum poolType { _p_INGREDIENTELETAL,_p_INGREDIENTE, _p_ARROZ, _p_GRUPO, _p_CLIENTE};


class ObjectManager
{
	vector<Pool<GameObject>*> pools;

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

	template<typename T>
	inline Pool<T>* getPool(int i) {
		return (Pool<T>*) pools[i];
	}

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

