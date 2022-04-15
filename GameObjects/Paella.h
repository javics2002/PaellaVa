#pragma once
#include "ObjetoPortable.h"
#include "Ingrediente.h"
#include "../Control/ObjectManager.h"
#include <list>



enum EstadoPaellas { Preparacion, Coccion, Hecha };
enum Resultado { Cruda, PocoHecha, Perfecta, MuyHecha, Quemada, Incomestible };
enum Contenido { Limpia, Entera, Mitad, Sucia };
enum TipoPaella { Pequena, Mediana, Grande };

class Game;
class Arroz;
class ParticleExample;

class Paella : public ObjetoPortable
{	
public:
	
	Paella(Game* game, int volumen_);
	
	~Paella() {
		delete humo;
	};

	bool ingrValido(Ingrediente* ingr);
	void anadeIngr(Ingrediente* ingr_);
	void anadeArroz(Arroz* arroz);

	void paellaRecogida();
	void update() override;
	void setLavado(Contenido contenidoPaella,string texturaPaella);

	void setEnsuciada();

	void onObjectPicked() override;
	void onObjectDropped() override;

	bool canPick() override;
	bool conArroz();
	void enLaMesa(bool estaEnLaMesa);

	list<tipoIngrediente> getVIngredientes();
	vector<bool> getIngrPaella();

	EstadoPaellas getState();
	void setState(EstadoPaellas estado);

	int getContenido();
	void setContenido(Contenido contenidoP);

	int getTipo();
	
	int getCoccion();

	void comerPaella();

	void contaminaPaella();
	bool estaContaminada();

	int ingredientesEnPaella();

	void render(SDL_Rect* cameraRect);

private:
	vector<string> coccionTex = { "paellaCruda", "paellaPocoHecha", "paella", "paellaMuyHecha", "paellaQuemada", "paellaIncomestible" };
	vector<int> tiemposDeCoccion = { 3000, 6000, 9000, 12000, 15000 };

	const int MAX_INGR = 3;
	double mTiempoCoccion = 0.0, mTiempo = 0.0, mTiempoHumo = 2000.0;
	int mSumaIntervalo = 0, i = 0;
	bool mEnMesa = false;
	bool mArroz = false;

	EstadoPaellas estado = Preparacion;
	int estadoCoccion = 0;
	int contenido = Limpia;

	int miTipo;
	
	int initCocTime = 0;
	int initHumoTime = 0;

	list<tipoIngrediente> ingredientes;

	vector<bool> ingrEnPaella;
	bool contaminada = false;

	ParticleExample* humo;
};

