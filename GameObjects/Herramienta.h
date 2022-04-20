#pragma once

#include "ObjetoPortable.h"
#include <vector>

class Game;

class Herramienta : public ObjetoPortable
{
protected:
	const unsigned int DIMENSION = 70;

public:


	Herramienta(Game* game);
	~Herramienta() = default;

	void update() override;

	void onActivate() override;
	void onDeactivate() override;

	void onObjectPicked() override;
	void onObjectDropped() override;
};



