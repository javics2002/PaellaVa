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

	vector<string> texturaIngrProcesado = { "alcachofaP", "calamarP", "cangrejoP", "gambaP",
		"langostaP", "mejillonP", "pimientoRojoP", "pimientoVerdeP", "polloP" };

	bool procesado = false;

public:

	Ingrediente(Game* game);
	~Ingrediente() = default;

	void update() override;	
	std::list<PoolObject*>::const_iterator ingredientCollide() override;

	void onActivate() override;

	void ingredienteRecogido();
	
	void setProcesado(bool estadoIngr, Ingrediente ingr);

	tipoIngrediente getTipo();

	void onObjectPicked() override;
	void onObjectDropped() override;
};

