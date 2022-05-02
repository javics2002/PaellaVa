#include "Cola.h"

bool Cola::esValido(int tamGrupo)
{
    return capacidadActual + tamGrupo <= t_Max;
}

bool Cola::estaLlena()
{
    return capacidadActual == t_Max;
}

void Cola::add(GrupoClientes* grupo, int tam)
{
    capacidadActual += tam;
    cola.push_back(grupo);
}

void Cola::remove(list<GrupoClientes*>::const_iterator it, int tam)
{
    capacidadActual -= tam;
    cola.erase(it);
    colaAvanza();
}

list<GrupoClientes*>::const_iterator Cola::getPos()
{
    return --cola.end();
}

void Cola::colaAvanza()
{
    for (auto it : cola) {
        it->setState(EstadoClientes::CAMINANDO);
    }
        
}

