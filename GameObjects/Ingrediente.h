 #pragma once
#include "PoolObject.h"

#include <vector>

class Game;

static const int numIngredientes = 10;

class Ingrediente : public PoolObject
{
	const unsigned int DIMENSION = 70;

	vector<string> texturaIngrediente = { "alcachofa", "calamar", "cangrejo", "gamba",
		"langosta", "mejillon", "pimientoRojo", "pimientoVerde", "pollo" };

	bool procesado = false;

public:
	Ingrediente(Game* game);
	~Ingrediente() = default;

	void update() override;	
	std::list<PoolObject*>::const_iterator ingredientCollide() override;

	void onActivate() override;

	void ingredienteRecogido();
	
	void setProcesado(bool estadoIngr);
};

