#pragma once
#include "ObjetoPortable.h"

class Paella : public ObjetoPortable
{
public:
	enum Coccion { Cruda, PocoHecha, Perfecta, MuyHecha, Quemada, Incomestible};
	enum Contenido { Limpia, Entera, TresCuartos, Mitad, UnCuarto, Sucia };
};

