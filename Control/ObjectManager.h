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
class CajaHerramientas;
class TorreCajaTakeaway;

#include "../GameObjects/Ingredienteletal.h"

#include <queue>
#include <vector>

class Game;
class Herramienta;

using namespace std;

enum poolType { _p_INGREDIENTELETAL, _p_INGREDIENTE, _p_HERRAMIENTA, _p_CAJATAKEAWAY, _p_ARROZ, _p_REPARTIDOR, _p_GRUPO, _p_CLIENTE};


class ObjectManager
{
	vector<Pool<GameObject>*> pools;

	vector<Paella*> paellas;

	vector<Mueble*> muebles;
	
	vector<Player*> players;

	vector<GameObject*> renderAll;

	Game* mGame;


	// Varibales necesarias para que vayan apareciendo poco a poco en el tutorial
	vector<Silla*> mSillas;
	vector<Mesa*> mMesas;
	vector<Encimera*> mEncimeras;
	vector<Encimera*> mVentanillas;
	vector<Cinta*> mCintas;
	vector<Pila*> mPilas;
	vector<Fogon*> mFogones;
	vector<TablaProcesado*> mTablasProcesados;
	Cartel* mCartel;
	Puerta* mPuerta;
	BolsaArroz* mBolsaDeArroz;
	InicioCinta* mIniCinta;
	FinalCinta* mFinCinta;
	Lavavajillas* mLavavajillas;
	CajaHerramientas* mCajaHerramientas;
	TorreCajaTakeaway* mTorreCajaTakeaway;



public:
	ObjectManager(Game* mGame);
	~ObjectManager();

	void handleInput(bool& exit);

	void update();
	void refresh();
	void render(SDL_Rect* rect);
	void debug(SDL_Rect* rect);

	void addMueble(Mueble* mueble);

	//Metodos para anadir por separado los muebles para hacerlos aparecer segun el estado en el que se encuentre el jugador

	void addIniCinta(InicioCinta* i) { mIniCinta = i; };
	void addFinCinta(FinalCinta* f) {mFinCinta = f;};
	void addSilla(Silla* s) { mSillas.push_back(s); };
	void addMesa(Mesa* m){ mMesas.push_back(m); };
	void addVentanilla(Encimera* v){ mVentanillas.push_back(v); };
	void addEncimera(Encimera* e){ mEncimeras.push_back(e); };
	void addCinta(Cinta* c) { mCintas.push_back(c); };
	void addCartel(Cartel* c) { mCartel = c; };
	void addPuerta(Puerta* p) { mPuerta = p; };
	void addPilas(Pila* p) { mPilas.push_back(p); };
	void addBolsa(BolsaArroz* b) { mBolsaDeArroz = b; };
	void addFogon(Fogon* f) { mFogones.push_back(f); };
	void addTabla(TablaProcesado* t) { mTablasProcesados.push_back(t); };
	void addLavavajillas(Lavavajillas* l) { mLavavajillas = l; };
	void addCaja(CajaHerramientas* c) { mCajaHerramientas = c; };
	void addTorreCajaTakeaway(TorreCajaTakeaway* c) { mTorreCajaTakeaway = c; };

	vector<Silla*> getSillas() { return mSillas; };
	vector<Mesa*> getMesas() { return mMesas; };
	vector<Encimera*> getEncimeras() { return mEncimeras; };
	vector<Encimera*> getVentanilla() { return mVentanillas; };
	vector<Cinta*> getCintas() { return mCintas; };
	vector<Pila*> getPilas() { return mPilas; };
	vector<Fogon*> getFogones() { return mFogones; };
	vector<TablaProcesado*> getTablas() { return mTablasProcesados; };
	Cartel* getCartel() { return mCartel; };
	Puerta* getPuerta() { return mPuerta; };
	BolsaArroz* getBolsa() { return mBolsaDeArroz; };
	InicioCinta* getIniCinta() { return mIniCinta; };
	FinalCinta* getFinCinta() { return mFinCinta; };
	Lavavajillas* getLavavajillas() { return mLavavajillas; };
	CajaHerramientas* getCajaHerramientas() { return mCajaHerramientas; };
	TorreCajaTakeaway* getTorreCajaTakeaway() { return mTorreCajaTakeaway; };



	Paella* addPaella(int n);

	void addPlayer(Player* player);

	template<typename T>
	inline Pool<T>* getPool(int i) {
		return (Pool<T>*) pools[i];
	}

	vector<Mueble*> getMuebles() { return muebles; }
	vector<Mueble*> getMueblesCollisions(SDL_Rect collider);
	vector<Mueble*> getMueblesOverlaps(SDL_Rect collider);

	vector<Paella*> getPaellas() { return paellas; };

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

