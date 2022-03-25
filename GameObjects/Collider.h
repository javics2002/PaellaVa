#pragma once

#include <list>

class Player;

class PoolObject;

class Silla;
class Mesa;

class Collider {

public:
    virtual std::pair<bool, std::list<PoolObject*>::const_iterator> colisionIngrediente() {
        return { false , std::list<PoolObject*>::const_iterator() };
    };
    virtual bool colisionClientes() {
        return false;
    }
    virtual bool colisionPlayer(Player* p) {
        return false;
    }
    virtual bool ratonEncima() {
        return false;
    }
    virtual Silla* initMesa(Mesa* mesa) {
        return nullptr;
    }
};
