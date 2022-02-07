#pragma once
#include "../Data/Posicion.h"
#include "../Control/ObjectManager.h"
#include "SDL.h"
#include <unordered_map>

#include <tmxlite/Map.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/LayerGroup.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Types.hpp>
#include "TMXLoader/TMXLoader.h"

using namespace std;
using namespace tmx;

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

	//Tileset
	Map map;
	
	TMXLoader* t;

public:
	Restaurante();
	~Restaurante();

	void Update();
	void Render(SDL_Renderer* renderer, SDL_Texture* texture, TMXLoader* loader);
};