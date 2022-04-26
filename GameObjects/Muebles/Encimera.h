#pragma once
#include "Mueble.h"

class Ingrediente;
class Herramienta;

class Encimera : public Mueble
{
private:
	Ingrediente* ingr_ = nullptr;
	Paella* paella_ = nullptr;
	Arroz* arroz_ = nullptr;
	Herramienta* h_ = nullptr;

public:
	Encimera(Game* game, Vector2D<double> pos);
	~Encimera() = default;

	virtual bool receiveIngrediente(Ingrediente* ingr) override;
	virtual bool returnObject(Player* p) override;
	virtual bool receivePaella(Paella* pa) override;
	virtual bool receiveArroz(Arroz* pa) override;
	virtual bool receiveHerramienta(Herramienta* h) override;

	Ingrediente* hasIngrediente() { return ingr_; }

	void clearIngrediente() { ingr_ = nullptr; }
};



