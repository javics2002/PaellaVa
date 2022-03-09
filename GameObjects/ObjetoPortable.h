#pragma once
#include "GameObject.h"

class ObjetoPortable : public GameObject
{
public:

	ObjetoPortable() :GameObject(game) {};
	~ObjetoPortable() {};

	virtual bool interaccionFogones();
	virtual bool interaccionMesa();
	virtual bool InteraccionLavaplatos();
	virtual bool InteraccionTabla();
};

