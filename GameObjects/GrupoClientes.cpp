#include "GrupoClientes.h"

GrupoClientes::GrupoClientes(Game* game, vector<Cliente*>* clientes_) : GameObject(game), clientes(clientes_) {
	
}


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

void GrupoClientes::setPosCola(list<GrupoClientes*>::const_iterator pos)
{
	posCola = pos;
}
