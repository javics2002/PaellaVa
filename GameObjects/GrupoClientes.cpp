#include "GrupoClientes.h"

void GrupoClientes::Pedir()
{

}

void GrupoClientes::Comer()
{

}

float GrupoClientes::Puntuacion()
{
	return 0.0f;
}


void GrupoClientes::setGrupo(list<GrupoClientes*>::const_iterator pos, vector<Cliente*> clientes_)
{
	posCola = pos;
	clientes = clientes_;
}

void GrupoClientes::update()
{
	SDL_GetMouseState(&x, &y);

	auto posX = clientes[clientes.size() - 1]->getX();
	auto posY = clientes[clientes.size() - 1]->getY();


	if (x >= posX && x <= posX+(clientes[0]->getWidth() *clientes.size())) {
		if (y >= posY && y <= posY + clientes[0]->getWidth()) {
			cout << "100%";
		}
	}
}
