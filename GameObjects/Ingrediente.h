 #pragma once

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


class Ingrediente : public ObjetoPortable
{
protected:
	const unsigned int DIMENSION = 70;

	tipoIngrediente miTipo;
	bool procesado = false;
	bool letal = false;

	

public:


	Ingrediente(Game* game);
	~Ingrediente() = default;

	void update() override;	

	void onActivate() override;
	void onDeactivate() override;

	void cambiaTipo(int n);
	void ingredienteRecogido();
	
	void setProcesado(bool estadoIngr, Ingrediente *ingr);
	bool getProcesado();
	bool esLetal();

	tipoIngrediente getTipo();

	void onObjectPicked() override;
	void onObjectDropped() override;
};

