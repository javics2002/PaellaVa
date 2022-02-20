#include "Cliente.h"
#include "../../Control/game.h"

Cliente::Cliente(Game* game) : PoolObject(game)
{
	setDimension(DIMENSION, DIMENSION);
	textureName = berenjenaTexture;
}

void Cliente::update()
{
	for (auto i : game->getClientes(getCollider())) {
		if (i != this)colisionClientes();
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

void Cliente::cambiaTextura(TextureName textureN)
{
	textureName = textureN;
}
