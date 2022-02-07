#pragma once
#include "../Data/Posicion.h"
#include "../Data/Collider.h"
#include <string>

using namespace std;

class GameObject
{
protected:
	Posicion posicion;
	//Texture textura;
	Collider collider;
	float escala;
public:
};

