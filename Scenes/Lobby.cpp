#include "Lobby.h"
#include "../Scenes/Restaurante.h"
#include "../GameObjects/UI/ShowText.h"

Lobby::Lobby(Game* game, string nombre):Scene(game)
{

	Imagen* cocinera = new Imagen(game, sdlutils().width() / 2 - 200, sdlutils().height() / 2, 450, 450, "cocinera");

	uiManager->addInterfaz(cocinera);

	ShowText* NombreCocinera = new ShowText(game, nombre, "abadiNombre",
		(int)cocinera->getX(), 100,
		200, 50);

	uiManager->addInterfaz(NombreCocinera);

 //= (uiManager->getNombre(), );

	//oLobby.push_back();

}
