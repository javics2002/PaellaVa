#pragma once
#include "ObjetoPortable.h"
#include "Ingrediente.h"
#include "../Control/ObjectManager.h"
#include <list>



enum EstadoPaellas { Preparacion, Coccion, Hecha };
enum Resultado { Cruda, PocoHecha, Perfecta, MuyHecha, Quemada, Incomestible };
enum Contenido { Limpia, Entera, Mitad, Sucia };
enum TipoPaella { Minima, Mediana, Grande };

class Game;
class Arroz;

class Paella : public ObjetoPortable
{	
public:
	
	Paella(Game* game, TipoPaella volumen_);
	
	~Paella() {};

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


	TipoPaella getTipo();
	
	int getCoccoin();

	void comerPaella();

	void render(SDL_Rect* cameraRect);

private:
	vector<string> coccionTex = { "paellaCruda", "paella", "paella", "paella", "paellaQuemada", "paellaQuemada" };
	vector<int> tiemposDeCoccion = { 3000, 6000, 9000, 12000, 15000 };

	const int MAX_INGR = 3;
	double mTiempoCoccion = 0.0, mTiempo = 0.0;
	int mSumaIntervalo = 0, i = 0;
	bool mEnMesa = false;
	bool mArroz = false;

	EstadoPaellas estado = Preparacion;
	int estadoCoccion = 0;
	int contenido = Limpia;

	TipoPaella miTipo;

	
	int initCocTime = 0;

	list<tipoIngrediente> ingredientes;

	vector<bool> ingrEnPaella;
};

