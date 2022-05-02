#pragma once
#include "../Scenes/Scene.h"

#include "./tmxlite/Map.hpp"
#include "./tmxlite/Layer.hpp"
#include "./tmxlite/TileLayer.hpp"
#include "./tmxlite/ObjectGroup.hpp"
#include "./tmxlite/Tileset.hpp"

#include "../Control/Camera.h"

#include <unordered_map>

using namespace std;
using namespace tmx;
using uint = unsigned int;

class Player;
class Mueble;
class NetworkManager;

struct MapInfo {
	Map* tilemap;
	string ruta;
	int filas, columnas;
	int anchoTile, altoTile;
	int anchoFondo, altoFondo;
	bool tutorial = false;
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

class Jornada : public Scene
{
	const float LERP_INTERPOLATION = 0.2f;

	MapInfo mapInfo;

	list<double> puntuacionesComandas;

	double puntuacionTotal;

	RedactaComandabutton* redactaBut;

	Vector2D<int> tamRestaurante = Vector2D<int>(0, 1216);
	Vector2D<double> positionCamarero;
	Vector2D<double> positionCocinera;

	int nJornada = 0;

	bool host;

	int idCount = 0;

public:
	Jornada(Game* game, string tilemap, int numeroJornada, bool cocinera = true);
	~Jornada();

	void handleInput(bool& exit)  override;
	void update() override;
	void addPuntuaciones(double puntosComanda) override;

	void refresh() override;
	void mediaPuntuaciones();

	void loadMap(string const& path);
	void togglePause();

	int getNumJornada() { return nJornada; }
	int getPunctuationJornada() {
		mediaPuntuaciones();
		return puntuacionTotal; }

};

