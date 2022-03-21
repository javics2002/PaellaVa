#pragma once
#include "Mueble.h"
class UIManager;
class UiButton;
class Ventanilla : public Mueble
{
	Paella* mpaella = nullptr;
	UIManager* uimt=nullptr;
	UiButton* numeroactual = nullptr;
	Texture* numactex=nullptr;
	int xnumero = 0;
	int ynumero = 0;
	SDL_Rect* camerar;
	int lastnumact = 0;
	int screentimenumero=3000;

public:
	Ventanilla(Game* game, Vector2D<double> pos,SDL_Rect* r);
	bool receivePaella(Paella* pa)override;//return false para no dejar la paella (sin comanda seleccionada no se puede)
	~Ventanilla() = default;
	void muestraNumeroMesa();
	void render(SDL_Rect* cameraRect)override;
	virtual bool returnObject(Player* p) override;
	void update() override;
};

