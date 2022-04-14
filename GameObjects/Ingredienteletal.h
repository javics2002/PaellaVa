#pragma once
#include "Ingrediente.h";
class Game;
const vector<string> texturaIngredienteLetal = { "rata" };
enum tipoIngredienteLetal {rata, LEST};

class IngredienteLetal : public Ingrediente
{
	tipoIngredienteLetal miTipoL;
public:
	IngredienteLetal(Game* game);
	~IngredienteLetal();
	void onActivate() override;

	void cambiaTipo (int n);
};

