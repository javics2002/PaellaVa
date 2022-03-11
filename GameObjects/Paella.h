#pragma once
#include "ObjetoPortable.h"
#include "Ingrediente.h"
#include <list>


enum Estado { Preparacion, Coccion, Preparada };
enum Resultado { Cruda, PocoHecha, Perfecta, MuyHecha, Quemada, Incomestible };
enum Contenido { Limpia, Entera, Mitad, Sucia };

class Paella : public ObjetoPortable
{	
	const int MAX_INGR = 3;
	double tiempoCocion = 0.0, tiempo = 0.0;
	int sumaIntervalo = 0, i = 0;

	Estado estado;
	Resultado estadoFinal = Cruda;

	vector<int> tiemposDeCoccion = {14000, 20000, 25000, 28000, 33000, 38000};

	list<Ingrediente> ingredientes;

public:
	
	enum Tamaño { Minima, Mediana, Grande };

	Tamaño tamaño;

	Paella(Tamaño tamaño_);
	~Paella() {};

	void añadeIngr(Ingrediente ingr_);
	void setState(Estado estado_);
	void update() override;
	Estado getState();

};

