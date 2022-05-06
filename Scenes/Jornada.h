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
	Map* mTilemap;
	string mRuta;
	int mFilas, mColumnas;
	int mAnchoTile, mAltoTile;
	int mAnchoFondo, mAltoFondo;

	unordered_map<uint, Texture*> mTilesets;

	MapInfo() {
		mTilemap = nullptr;
		mRuta = "";
		mFilas = mColumnas = mAnchoTile = mAltoTile = 0;
	}
	~MapInfo() {
		if (mTilemap != nullptr)
			delete mTilemap;
	}
};

class Jornada : public Scene
{
	const float LERP_INTERPOLATION = 0.2f;

	MapInfo mMapInfo;

	list<double> mPuntuacionesComandas;

	double mPuntuacionTotal;

	RedactaComandabutton* mRedactaBut;

	Vector2D<int> mTamRestaurante = Vector2D<int>(0, 1216);
	Vector2D<double> mPositionCamarero;
	Vector2D<double> mPositionCocinera;

	int mNumJornada = 0;

	bool mHost;

	int mIdCount = 0;

public:
	Jornada(Game* mGame, string tilemap, int numeroJornada, bool cocinera = true);
	~Jornada();

	void handleInput(bool& exit)  override;
	void update() override;
	void addPuntuaciones(double puntosComanda) override;

	void refresh() override;
	void mediaPuntuaciones();

	void loadMap(string const& path);
	void togglePause();

	int getNumJornada() { return mNumJornada; }
	double getPunctuationJornada() {
		mediaPuntuaciones();
		return mPuntuacionTotal; }

};

