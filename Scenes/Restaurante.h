#pragma once
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

#include "../Control/Camera.h"

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

class Restaurante : public Scene
{
	Game* game;

	//Punteros a los jugadores (Quizas deberian llamarse yo y amigo, en relativo)
	Player* host;
	Player* client;

	Camera* camara;

	ObjectManager* objectManager;
	UIManager* uiManager;

	MapInfo mapInfo;
	Fondo* fondo;

	Vector2D<int> tamRestaurante = Vector2D<int>(0, 1216);
	bool playerInRestaurante = true;
	bool lerpEmpezado = true;


public:
	Restaurante(Game* game);
	~Restaurante();

	void handleInput();
	void update();
	void render();
	void debug();

	ObjectManager* getObjectManager();
	UIManager* getUIManager();
	void loadMap(string const & path);
};