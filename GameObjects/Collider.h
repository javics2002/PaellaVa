#pragma once

#include <list>

class PoolObject;

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
};
