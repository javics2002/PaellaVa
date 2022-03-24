#pragma once

#include <list>


class PoolObject;

class Silla;
class Mesa;

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
};
