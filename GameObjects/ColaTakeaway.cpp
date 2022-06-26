#include "ColaTakeaway.h"

bool ColaTakeaway::esValido()
{
    return capacidadActual + 1 <= t_Max;
}

bool ColaTakeaway::estaLlena()
{
    return capacidadActual == t_Max;
}

void ColaTakeaway::add(Repartidor* repartidor)
{
    capacidadActual++;
    cola.push_back(repartidor);
}

void ColaTakeaway::remove(list<Repartidor*>::const_iterator it)
{
    capacidadActual--;
    cola.erase(it);
    colaAvanza();
}

list<Repartidor*>::const_iterator ColaTakeaway::getPos()
{
    return --cola.end();
}

void ColaTakeaway::colaAvanza()
{
    for (auto it : cola) {
        it->setState(repCAMINANDO);
    }

}
