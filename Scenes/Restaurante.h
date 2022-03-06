#pragma once
#include "../Data/Posicion.h"
#include "../Control/ObjectManager.h"
#include "../GameObjects/UI/UIManager.h"
#include "../Scenes/Scene.h"
#include "SDL.h"

#include "./tmxlite/Map.hpp"
#include "./tmxlite/Layer.hpp"
#include "./tmxlite/TileLayer.hpp"
#include "./tmxlite/ObjectGroup.hpp"
#include "./tmxlite/Tileset.hpp"
#include "../GameObjects/Fondo.h"

#include <unordered_map>

using namespace std;
using namespace tmx;
using uint = unsigned int;

class Player;
class Mueble;
class Game;

struct MapInfo {
	Map* tilemap;
	string ruta;
	int filas, columnas;
	int anchoTile, altoTile;
	unordered_map<uint, Texture*> tilesets;

	MapInfo() {
		tilemap = nullptr;
		ruta = "";
		filas = columnas = anchoTile = altoTile = 0;
	}
	~MapInfo() {
		if (tilemap != nullptr)
			delete tilemap;
	}
};

class Restaurante: public Scene
{
	Game* game;

	//Punteros a los jugadores (Quizas deberian llamarse yo y amigo, en relativo)
	Player* host;
	Player* client;

	//Mapa de muebles. Podemos preguntar qué mueble se encuentra en x posición de forma constante.
	//unordered_map<Posicion, Mueble*> muebles;

	ObjectManager* objectManager;
	UIManager* uiManager;

	MapInfo mapInfo;
	Fondo* fondo;
public:
	Restaurante(Game* game);
	~Restaurante();

	void handleInput();
	void update();
	void render();

	ObjectManager* getObjectManager();
	void loadMap(string const & path);
};