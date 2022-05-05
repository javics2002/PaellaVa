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
	Imagen* mCocinera = new Imagen(mGame, sdlutils().width() / 2 - 300, sdlutils().height() / 2, 450, 450, "cocineraLobby");

	mUiManager->addInterfaz(mCocinera);

	//Nombre cocinera
	
	ShowText* mCocineraName = new ShowText(mGame, mGame->getNombre(), "lobby",
		(int)mCocinera->getX(), sdlutils().height()- 165);

	mUiManager->addInterfaz(mCocineraName);

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

	UiButton *mReturnButton = new UiButton(mGame, "back", 100, 50, 125, 60);
	mReturnButton->setInitialDimension(mReturnButton->getWidth(), mReturnButton->getHeight());
	mReturnButton->setAction([](Game* mGame, bool& exit) {
		mGame->sendMessageScene(new HostClient(mGame));
		mGame->getNetworkManager()->close();
	});

	mUiManager->addButton(mReturnButton);

	mUiManager->setEnLobby(true);
}

Lobby::Lobby(Game* mGame, string nombreHost) : Scene(mGame)
{
	//fondo
	mBackground->setTexture("lobbyBg");
	mBackground->setPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	mBackground->setDimension(sdlutils().width(), sdlutils().height()+100);

	//Cocinera
	Imagen* mCocinera = new Imagen(mGame, sdlutils().width() / 2 - 300, sdlutils().height() / 2, 450, 450, "cocineraLobby");

	mUiManager->addInterfaz(mCocinera);

	// Feedback
	ShowText* mReady = new ShowText(mGame, "LISTO PARA COMENZAR", "lobby", sdlutils().width() / 2, sdlutils().height() - 115);

	mUiManager->addInterfaz(mReady);

	//Nombre cocinera (recibido de paquete)
	ShowText* mCocineraName = new ShowText(mGame, nombreHost, "ip",
		(int)mCocinera->getX(), sdlutils().height() - 165);

	mUiManager->addInterfaz(mCocineraName);

	// Imagen del camarero conectandose 
	mCamarero = new Imagen(mGame, sdlutils().width() / 2 + 300, sdlutils().height() / 2, 450, 450, "camareroLobby");

	mUiManager->addInterfaz(mCamarero);

	//Nombre del camarero (tu propio nombre)
	ShowText* mCamareroName = new ShowText(mGame, mGame->getNombre(), "ip",
		(int)mCamarero->getX(), sdlutils().height() - 165);

	mUiManager->addInterfaz(mCamareroName);
}

void Lobby::clienteUnido(std::string nombreCliente)
{
	mStart->setTexture("start");
	mCamarero->setTexture("camareroLobby");

	mUiManager->setEnLobby(false);

	//Nombre del camarero
	ShowText* mCamareroName = new ShowText(mGame, nombreCliente, "ip",
		(int)mCamarero->getX(), sdlutils().height() - 165);

	mUiManager->addInterfaz(mCamareroName);
	mSearching->setText("LISTO PARA COMENZAR");

}
