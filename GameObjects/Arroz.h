#pragma once
#include "Ingrediente.h"

class Game;

class Arroz : public Ingrediente
{
	const unsigned int DIMENSION = 70;

public:

	Arroz(Game* game,string claveTextura);
	~Arroz() = default;

	void update() override;

	void onActivate() override;
	void onDesactivate()override;

	void arrozRecogido();

	void setEnPaella();

	void onObjectPicked() override;
	void onObjectDropped() override;

};

