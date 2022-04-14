#pragma once

#include "../Data/Pool.h"
#include "../GameObjects/Ingrediente.h"
#include "../GameObjects/Cliente.h"
#include "../GameObjects/GrupoClientes.h"
#include "../GameObjects/Paella.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Arroz.h"
#include "../GameObjects/Muebles/InicioCinta.h"
#include "../GameObjects/Muebles/FinalCinta.h"
#include "../GameObjects/Muebles/Puerta.h"
#include "../GameObjects/Ingredienteletal.h"
#include "../GameObjects/Muebles/Encimera.h"
#include "../GameObjects/Muebles/Ventanilla.h"
#include "../GameObjects/Muebles/Cinta.h"
#include "../GameObjects/Muebles/Cartel.h"
#include "../GameObjects/Muebles/Puerta.h"
#include "../GameObjects/Muebles/Pila.h"
#include "../GameObjects/Muebles/BolsaArroz.h"
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

	vector<Silla*> sillas;
	vector<Mesa*> mesas;
	vector<Encimera*> encimeras;
	vector<Ventanilla*> ventanillas;
	vector<Cinta*> cintas;
	vector<Pila*> pilas;
	Cartel* cartel;
	Puerta* puerta;
	BolsaArroz* bolsaDeArroz;


public:
	ObjectManager(Game* game);
	~ObjectManager();

	void handleInput(bool& exit);

	void update();
	void refresh();
	void render(SDL_Rect* rect);
	void debug(SDL_Rect* rect);

	void addMueble(Mueble* mueble);

	void addSilla(Silla* s);
	void addMesa(Mesa* m);
	void addVentanilla(Ventanilla* v);
	void addEncimera(Encimera* e);
	void addCinta(Cinta* c);
	void addCartel(Cartel* c);
	void addPuerta(Puerta* p);
	void addPilas(Pila* p);
	void addBolsa(BolsaArroz* b);

	vector<Silla*> getSillas();
	vector<Mesa*> getMesas();
	vector<Encimera*> getEncimeras();
	vector<Ventanilla*> getVentanilla();
	vector<Cinta*> getCintas();
	vector<Pila*> getPilas();
	Cartel* getCartel();
	Puerta* getPuerta();
	BolsaArroz* getBolsa();


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

