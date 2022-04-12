#pragma once
#include "Ingrediente.h";
class Game;
const vector<string> texturaIngredienteletal = { "rata" };
enum tipoIngredienteletal {rata, LEST};
class Ingredienteletal :public Ingrediente
{
	tipoIngredienteletal miTipoL;
public:
	
	Ingredienteletal(Game* game);
	~Ingredienteletal();
	void onActivate() override;

};

