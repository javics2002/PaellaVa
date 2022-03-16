 #pragma once
#include "PoolObject.h"
#include <vector>

class Game;


enum tipoIngrediente {
	alcachofa, calamar, cangrejo, gamba, langosta, mejillon, pimientoRojo, pimientoVerde, pollo, LAST
};

//vector<string> texturaIngrediente = { "alcachofa", "calamar", "cangrejo", "gamba",
//	"langosta", "mejillon", "pimientoRojo", "pimientoVerde", "pollo" };

class Ingrediente : public PoolObject
{
	tipoIngrediente miTipo;

	const unsigned int DIMENSION = 70;

	vector<string> texturaIngrediente = { "alcachofa", "calamar", "cangrejo", "gamba",
		"langosta", "mejillon", "pimientoRojo", "pimientoVerde", "pollo" };

	bool procesado = false;
	bool cogido = false;

public:

	Ingrediente(Game* game);
	~Ingrediente() = default;

	void update() override;	
	std::list<PoolObject*>::const_iterator ingredientCollide() override;

	void onActivate() override;

	void ingredienteRecogido();
	
	void setProcesado(bool estadoIngr);

	void setCogido(bool cogido_);

	bool getCogido();

	tipoIngrediente getTipo();
};

