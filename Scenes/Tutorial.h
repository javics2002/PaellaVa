#pragma once
#include "../Scenes/Scene.h"

#include "./tmxlite/Map.hpp"
#include "./tmxlite/Layer.hpp"
#include "./tmxlite/TileLayer.hpp"
#include "./tmxlite/ObjectGroup.hpp"
#include "./tmxlite/Tileset.hpp"
#include "../Scenes//Jornada.h"

#include "../Control/Camera.h"

#include <unordered_map>

using namespace std;
using namespace tmx;
using uint = unsigned int;

class Player;
class Mueble;
class NetworkManager;




class Tutorial : public Scene
{

	const float LERP_INTERPOLATION = 0.2f;

	MapInfo mapInfo;

	Vector2D<int> tamRestaurante = Vector2D<int>(0, 1216);

public:
	Tutorial(Game* game, string tilemap);
	~Tutorial();

	void handleInput(bool& exit)  override;
	void update() override;

	void changeState(States state_) override;
	States getState() override;

	void refresh() override;

	void loadMap(string const& path);
	void togglePause();
};

