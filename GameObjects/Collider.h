#pragma once

#include <list>

class PoolObject;

class Collider {

public:
    virtual std::list<PoolObject*>::const_iterator ingredientCollide() {
        return std::list<PoolObject*>::const_iterator();
    };
};
