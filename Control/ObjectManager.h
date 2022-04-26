#pragma once

#include "../Data/Pool.h"
#include "../GameObjects/Ingrediente.h"
#include "../GameObjects/Cliente.h"
#include "../GameObjects/GrupoClientes.h"
#include "../GameObjects/Paella.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Arroz.h"

class InicioCinta;
class FinalCinta;
class Puerta;
class Encimera;
class Ventanilla;
class Cinta;
class Pila;
class BolsaArroz;
class Cartel;
class Fogon;
class Silla;
class Mesa;
class TablaProcesado;
class Lavavajillas;

#include "../GameObjects/Ingredienteletal.h"

#include <queue>
#include <vector>

class Game;
class Herramienta;

using namespace std;

enum poolType { _p_INGREDIENTELETAL, _p_INGREDIENTE, _p_HERRAMIENTA, _p_ARROZ, _p_GRUPO, _p_CLIENTE};


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
	vector<Fogon*> fogones;
	vector<TablaProcesado*> tablasProcesados;
	Cartel* cartel;
	Puerta* puerta;
	BolsaArroz* bolsaDeArroz;
	InicioCinta* iniCinta;
	FinalCinta* finCinta;
	Lavavajillas* lavavajillas;


public:
	ObjectManager(Game* game);
	~ObjectManager();

	void handleInput(bool& exit);

	void update();
	void refresh();
	void render(SDL_Rect* rect);
	void debug(SDL_Rect* rect);

	void addMueble(Mueble* mueble);

	void addIniCinta(InicioCinta* i);
	void addFinCinta(FinalCinta* f);
	void addSilla(Silla* s);
	void addMesa(Mesa* m);
	void addVentanilla(Ventanilla* v);
	void addEncimera(Encimera* e);
	void addCinta(Cinta* c);
	void addCartel(Cartel* c);
	void addPuerta(Puerta* p);
	void addPilas(Pila* p);
	void addBolsa(BolsaArroz* b);
	void addFogon(Fogon* f);
	void addTabla(TablaProcesado* t);
	void addLavavajillas(Lavavajillas* l);

	vector<Silla*> getSillas();
	vector<Mesa*> getMesas();
	vector<Encimera*> getEncimeras();
	vector<Ventanilla*> getVentanilla();
	vector<Cinta*> getCintas();
	vector<Pila*> getPilas();
	vector<Fogon*> getFogones();
	vector<TablaProcesado*> getTablas();
	Cartel* getCartel();
	Puerta* getPuerta();
	BolsaArroz* getBolsa();
	InicioCinta* getIniCinta();
	FinalCinta* getFinCinta();
	Lavavajillas* getLavavajillas();



	Paella* addPaella(int n);

	void addPlayer(Player* player);

	template<typename T>
	inline Pool<T>* getPool(int i) {
		return (Pool<T>*) pools[i];
	}

	vector<Mueble*> getMuebles() { return muebles; }
	vector<Mueble*> getMueblesCollisions(SDL_Rect collider);
	vector<Mueble*> getMueblesOverlaps(SDL_Rect collider);

	vector<Player*> getPlayers() { return players; }

	Player* getPlayerOne() { 
		if (players.size() > 0) return players[0];
		return nullptr;
	}

	Player* getPlayerTwo() {
		if (players.size() > 1) return players[1];
		return nullptr;
	}

	void initMuebles();

	void sortAllGO();

	void resetMueblesCounter();
};

