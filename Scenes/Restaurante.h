#pragma once
#include "../Data/Posicion.h"
#include "../Control/ObjectManager.h"
#include "SDL.h"
#include <unordered_map>

using namespace std;

class Player;
class Mueble;

class Restaurante
{
	//Punteros a los jugadores (Quizas deberian llamarse yo y amigo, en relativo)
	Player* host;
	Player* client;

	//Mapa de muebles. Podemos preguntar qué mueble se encuentra en x posición de forma constante.
	//unordered_map<Posicion, Mueble*> muebles;

	ObjectManager* objectManager;

public:
	Restaurante();
	~Restaurante();

	void Update();
	void Render();
};