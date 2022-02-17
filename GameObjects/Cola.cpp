#include "Cola.h"

bool Cola::esValido(int tamGrupo)
{
    return capacidadActual + tamGrupo <= T_Max;
}

void Cola::add(GrupoClientes* grupo, int tam)
{
    capacidadActual += tam;
    cola.push_back(grupo);
}

list<GrupoClientes*>::const_iterator Cola::getPos()
{
    return --cola.end();
}

