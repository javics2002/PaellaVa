#include "Cliente.h"
#include "../../Control/game.h"
#include "../../Control/ObjectManager.h";

Cliente::Cliente(Game* game) : PoolObject(game)
{
	setDimension(DIMENSION, DIMENSION);
	setTexture("berenjena");
}

void Cliente::update()
{
	for (auto i : game->getObjectManager()->getClientes(getCollider())) {
		if (i != this) colisionClientes();
	}

	if (estado==CAMINANDO) {
		setPosition(getX() + 2.0f, getY());
	}
	else if (estado == ENCOLA) {

	}
}

void Cliente::onActivate()
{
	estado = CAMINANDO;
}


bool Cliente::colisionClientes()
{
	estado = ENCOLA;
	return true;
}

void Cliente::cambiaTextura(string nuevaClave)
{
	setTexture(nuevaClave);
}
