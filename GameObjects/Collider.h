#pragma once

#include <list>

class PoolObject;

class Silla;

class Ingrediente;
class GrupoClientes;
class Paella;

class Player;

class Collider {

public:
    virtual std::list<PoolObject*>::const_iterator ingredientCollide() {
        return std::list<PoolObject*>::const_iterator();
    };
    virtual bool colisionClientes() {
        return false;
    }
    virtual bool ratonEncima() {
        return false;
    }
    virtual Silla* initMesa() {
        return nullptr;
    }

    virtual bool recieveIngrediente(Ingrediente* in) { return false; };
    virtual bool recieveGrupoClientes(GrupoClientes* gc) { return false; };
    virtual bool recievePaella(Paella* pa) { return false; };

    virtual bool returnObject(Player* p) { return false; }
};
