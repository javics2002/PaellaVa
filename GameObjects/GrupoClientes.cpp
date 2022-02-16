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


void GrupoClientes::setGrupo(list<GrupoClientes*>::const_iterator pos, vector<Cliente*>* clientes_)
{
	posCola = pos;
	clientes = clientes_;
}
