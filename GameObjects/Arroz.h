#pragma once

#include "ObjetoPortable.h"

class Game;

class Arroz : public ObjetoPortable
{
    const unsigned int DIMENSION = 70;

public:
    Arroz(Game* mGame);
    ~Arroz() = default;

    void onObjectPicked() override {};
    void onObjectDropped() override {};
};
