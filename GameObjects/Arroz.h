#pragma once
#include "PoolObject.h"

class Game;

class Arroz : public PoolObject
{
    const unsigned int DIMENSION = 70;

public:
    Arroz(Game* game);
    ~Arroz() = default;

    void onObjectPicked() override {};
    void onObjectDropped() override {};
};
