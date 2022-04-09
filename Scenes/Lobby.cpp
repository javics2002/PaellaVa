#include "Lobby.h"
#include "../Scenes/Jornada.h"
#include "../Control/NetworkManager.h"
#include "../GameObjects/UI/ShowText.h"

Lobby::Lobby(Game* game, string nombre, Scene* hostClient):Scene(game)
{
	//Cocinera
	Imagen* cocinera = new Imagen(game, sdlutils().width() / 2 - 300, sdlutils().height() / 2, 450, 450, "cocinera");

	uiManager->addInterfaz(cocinera);

	//Imagen con la info del ip
	Imagen* bocadillo = new Imagen(game, cocinera->getX() - 200, sdlutils().height() / 2 - 100, 200, 200, "bocadillo");

	uiManager->addInterfaz(bocadillo);

	//IP ??
	ShowText* ip = new ShowText(game, "ss", "abadiNombre",
		(int)bocadillo->getX(), bocadillo->getY());

	uiManager->addInterfaz(ip);

	//Nombre cocinera
	ShowText* NombreCocinera = new ShowText(game, nombre, "abadiNombre",
		(int)cocinera->getX(), 100);

	uiManager->addInterfaz(NombreCocinera);

	//Imagen del camarero conectandose
	camarero = new Imagen(game, sdlutils().width() / 2 + 300, sdlutils().height() / 2, 450, 450, "CamareroConect");

	uiManager->addInterfaz(camarero);

	//Nombre del camarero
	ShowText* NombreCamarero = new ShowText(game, "elvergalarga", "abadiNombre",
		(int)camarero->getX(), 100);

	uiManager->addInterfaz(NombreCamarero);

	//Boton comenzar
	comenzar = new UiButton(game, "start2", sdlutils().width()/2, sdlutils().height()/2, 200, 70);

	comenzar->setAction([this](Game* game, bool& exit) {
		game->changeScene(new Jornada(game, "restaurante", 0));
		game->getNetworkManager()->init('h');
		//clienteUnido();
		});

	uiManager->addInterfaz(comenzar);

	UiButton *regresar = new UiButton(game, "back", 100, 50, 125, 60);

	regresar->setAction([hostClient](Game* game, bool& exit) {
		game->changeScene(hostClient);
		});

	uiManager->addInterfaz(regresar);
}

void Lobby::clienteUnido()
{
	comenzar->setTexture("start");
	camarero->setTexture("camarero");
}
