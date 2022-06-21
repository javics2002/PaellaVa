#pragma once
#include "Mueble.h"

class Ingrediente;
class Herramienta;
class CajaTakeaway;

class Encimera : public Mueble
{
private:
	Ingrediente* ingr_ = nullptr;
	Paella* paella_ = nullptr;
	Arroz* arroz_ = nullptr;
	Herramienta* herramienta_ = nullptr;
	CajaTakeaway* cajaTakeaway_ = nullptr;

public:
	Encimera(Game* mGame, Vector2D<double> pos);
	~Encimera() = default;

	virtual bool receiveIngrediente(Ingrediente* ingr) override;
	virtual bool returnObject(Player* p) override;
	virtual bool receivePaella(Paella* pa) override;
	virtual bool receiveArroz(Arroz* pa) override;
	virtual bool receiveHerramienta(Herramienta* h) override;
	virtual bool receiveCajaTakeaway(CajaTakeaway* h) override;

	Ingrediente* hasIngrediente() { return ingr_; }
	CajaTakeaway* hasCajaTakeaway() override { return cajaTakeaway_; };
	Paella* hasPaella() override { return paella_; };

	void clearIngrediente() { ingr_ = nullptr; }
};



