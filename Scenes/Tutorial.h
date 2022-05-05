#pragma once
#include "../Scenes/Scene.h"

#include "./tmxlite/Map.hpp"
#include "./tmxlite/Layer.hpp"
#include "./tmxlite/TileLayer.hpp"
#include "./tmxlite/ObjectGroup.hpp"
#include "./tmxlite/Tileset.hpp"
#include "../Scenes//Jornada.h"
#include  "../GameObjects/UI/RedactaComandabutton.h"
#include "../Data/ListaComandas.h"

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

	vector<string> textos = { "texto1" ,"texto2", "texto3","texto4","texto5","texto6","texto7","texto8","texto9","texto10",
		"texto11","texto12","texto13","texto14","texto15","texto16","texto17","texto18","texto19","texto20"};

	MapInfo mapInfo;

	bool continua = false;

	Vector2D<int> tamRestaurante = Vector2D<int>(0, 1216);
	Vector2D<double> positionCamarero;
	Vector2D<double> positionCocinera;

	Imagen* cuadroTexto = new Imagen(mGame, sdlutils().width() / 2, sdlutils().height() - 100, sdlutils().width()/1.5, 250, "cuadroTextoCocinera");
	Imagen* cuadradoPlay = new Imagen(mGame, 990, sdlutils().height() - 35, 20, 25, "X");

	RedactaComandabutton* rC = new RedactaComandabutton(mGame, mUiManager, "redactaboton", 10, 10, 30, 30);
	ListaComandas* lC = new ListaComandas(mGame, mUiManager);

	ShowText* text = new ShowText(mGame, " ", "abadiNombre", sdlutils().width() / 2, sdlutils().height() - 100);
	


public:
	Tutorial(Game* mGame, string tilemap);
	~Tutorial();

	void handleInput(bool& exit)  override;
	void update() override;

	void changeState(States state_) override;
	States getState() override;

	void refresh() override;
	void render() override;

	void loadMap(string const& path);
	void togglePause();
	void pauseTutorial() override;
	void nextStates();
	void activaCuadro(string texto_);
	void desactivaCuadro();

};

