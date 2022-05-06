#pragma once
#include "Ingrediente.h";
class Game;
const vector<string> texturaIngredienteLetal = { "rata", "pimientoVerdeMalo", "pimientoRojoMalo"};
enum tipoIngredienteLetal {rata, pVMalo, pRMalo, LEST};
const vector<string> texturaIngrProcesadoLetal = { "rataP", "pimientoVerdeMaloProcesado", "pimientoRojoMaloProcesado"};

class IngredienteLetal : public Ingrediente
{
	tipoIngredienteLetal miTipoL;
public:
	IngredienteLetal(Game* mGame);
	~IngredienteLetal();
	void onActivate() override;

	void setProcesado(bool estadoIngr, Ingrediente* ingr) override;
	void cambiaTipo (int n);
	tipoIngredienteLetal getTipoLetal();
};

