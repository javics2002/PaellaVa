#include "Lobby.h"
#include "../Scenes/Jornada.h"
#include "../Scenes/Tutorial.h"
#include "../Control/NetworkManager.h"
#include "../GameObjects/UI/ShowText.h"
#include "../Scenes/HostClient.h"

Lobby::Lobby(Game* mGame) : Scene(mGame)
{
	//fondo
	mBackground->setTexture("lobbyBg");
	mBackground->setPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	mBackground->setDimension(sdlutils().width(), sdlutils().height()+100);

	//Cocinera
	Imagen* cocinera = new Imagen(mGame, sdlutils().width() / 2 - 300, sdlutils().height() / 2, 450, 450, "cocineraLobby");

	mUiManager->addInterfaz(cocinera);

	//Nombre cocinera
	
	ShowText* NombreCocinera = new ShowText(mGame, mGame->getNombre(), "lobby",
		(int)cocinera->getX(), sdlutils().height()- 165);

	mUiManager->addInterfaz(NombreCocinera);

	//Imagen del camarero conectandose
	mCamarero = new Imagen(mGame, sdlutils().width() / 2 + 300, sdlutils().height() / 2, 450, 450, "CamareroConect");

	mUiManager->addInterfaz(mCamarero);

	//IP ??
	mSearching = new ShowText(mGame, "ESPERANDO             JUGADORES...", "lobby", sdlutils().width() / 2+20, sdlutils().height() - 115);

	mUiManager->addInterfaz(mSearching);

	////Nombre del camarero
	//ShowText* NombreCamarero = new ShowText(game, "elvergalarga", "abadiNombre",
	//	(int)camarero->getX(), 100);

	//uiManager->addInterfaz(NombreCamarero);

	//Boton comenzar
	mStart = new UiButton(mGame, "start2", sdlutils().width()/2, sdlutils().height()/2, 200, 70);
	mStart->setInitialDimension(mStart->getWidth(), mStart->getHeight());
	mStart->setAction([this](Game* mGame, bool& exit) {
		// crear player
		
		mGame->sendMessageScene(new Jornada(mGame, "Jornada1", 0 , true));

		sdlutils().virtualTimer().reset();
		// send info 
		mGame->getNetworkManager()->sendStartGame(0);
		mUiManager->setEnJornada(true);
		});

	mUiManager->addButton(mStart);

	UiButton *regresar = new UiButton(mGame, "back", 100, 50, 125, 60);
	regresar->setInitialDimension(regresar->getWidth(), regresar->getHeight());
	regresar->setAction([](Game* mGame, bool& exit) {
		mGame->sendMessageScene(new HostClient(mGame));
		mGame->getNetworkManager()->close();
	});

	mUiManager->addButton(regresar);

	mUiManager->setEnLobby(true);
}

Lobby::Lobby(Game* mGame, string nombreHost) : Scene(mGame)
{
	//fondo
	mBackground->setTexture("lobbyBg");
	mBackground->setPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	mBackground->setDimension(sdlutils().width(), sdlutils().height()+100);

	//Cocinera
	Imagen* cocinera = new Imagen(mGame, sdlutils().width() / 2 - 300, sdlutils().height() / 2, 450, 450, "cocineraLobby");

	mUiManager->addInterfaz(cocinera);

	// Feedback
	ShowText* listo = new ShowText(mGame, "LISTO PARA COMENZAR", "lobby", sdlutils().width() / 2, sdlutils().height() - 115);

	mUiManager->addInterfaz(listo);

	//Nombre cocinera (recibido de paquete)
	ShowText* NombreCocinera = new ShowText(mGame, nombreHost, "ip",
		(int)cocinera->getX(), sdlutils().height() - 165);

	mUiManager->addInterfaz(NombreCocinera);

	// Imagen del camarero conectandose 
	mCamarero = new Imagen(mGame, sdlutils().width() / 2 + 300, sdlutils().height() / 2, 450, 450, "camareroLobby");

	mUiManager->addInterfaz(mCamarero);

	//Nombre del camarero (tu propio nombre)
	ShowText* NombreCamarero = new ShowText(mGame, mGame->getNombre(), "ip",
		(int)mCamarero->getX(), sdlutils().height() - 165);

	mUiManager->addInterfaz(NombreCamarero);
}

void Lobby::clienteUnido(std::string nombreCliente)
{
	mStart->setTexture("start");
	mCamarero->setTexture("camareroLobby");

	mUiManager->setEnLobby(false);

	//Nombre del camarero
	ShowText* NombreCamarero = new ShowText(mGame, nombreCliente, "ip",
		(int)mCamarero->getX(), sdlutils().height() - 165);

	mUiManager->addInterfaz(NombreCamarero);
	mSearching->setText("LISTO PARA COMENZAR");

}
