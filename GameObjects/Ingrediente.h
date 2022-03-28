 #pragma once
#include "PoolObject.h"
#include "ObjetoPortable.h"
#include <vector>

class Game;


enum tipoIngrediente {
	 alcachofa, calamar, cangrejo, gamba, langosta, mejillon, pimientoRojo, pimientoVerde, pollo, LAST
};

const vector<string> texturaIngrediente = { "alcachofa", "calamar", "cangrejo", "gamba",
	"langosta", "mejillon", "pimientoRojo", "pimientoVerde", "pollo" };

const vector<string> texturaIngrProcesado = {  "alcachofaP", "calamarP", "cangrejoP", "gambaP",
	"langostaP", "mejillonP", "pimientoRojoP", "pimientoVerdeP", "polloP" };


class Ingrediente : public PoolObject
{
	const unsigned int DIMENSION = 70;

	tipoIngrediente miTipo;
	bool procesado = false;

public:

	Ingrediente(Game* game);
	~Ingrediente() = default;

	void update() override;	
	pair<bool, std::list<PoolObject*>::const_iterator> colisionIngrediente() override;

	void onActivate() override;
	void onDesactivate()override;

	void ingredienteRecogido();
	
	void setProcesado(bool estadoIngr, Ingrediente *ingr);
	bool getProcesado();

	tipoIngrediente getTipo();

	void setEnPaella();

	void onObjectPicked() override;
	void onObjectDropped() override;
};

