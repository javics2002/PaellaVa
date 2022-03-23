#pragma once

#include <list>


class PoolObject;

class Silla;
class Mesa;

class Ingrediente;
class GrupoClientes;
class Paella;

class Player;



class Collider {

public:
    virtual std::pair<bool, std::list<PoolObject*>::const_iterator> ingredientCollide() {
        return { false , std::list<PoolObject*>::const_iterator() };
    };
    virtual bool colisionClientes() {
        return false;
    }
    virtual bool ratonEncima() {
        return false;
    }
    virtual Silla* initMesa(Mesa* mesa) {
        return nullptr;
    }

    virtual bool receiveIngrediente(Ingrediente* ingr) { return false; };
    virtual bool receiveGrupoClientes(GrupoClientes* gc) { return false; };
    virtual bool receivePaella(Paella* pa) { return false; };
    virtual bool returnObject(Player* p) { return false; }
};
