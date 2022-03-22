#include "HostClient.h"
#include "../Scenes/Restaurante.h"

HostClient::HostClient(Game* game) : Scene(game)
{
	int offsetX = 300, offsetY = 150;
	
	auto cocineraSprite = new GameObject(game), camareroSprite = new GameObject(game);
	cocineraSprite->setTexture("cocinera");
	cocineraSprite->setPosition(Vector2D<double>(sdlutils().width() / 2 - offsetX, sdlutils().height() / 2 + offsetY));
	cocineraSprite->setDimension(1024, 1024);
	camareroSprite->setTexture("camarero");
	camareroSprite->setPosition(Vector2D<double>(sdlutils().width() / 2 + offsetX, sdlutils().height() / 2 + offsetY));
	camareroSprite->setDimension(1024, 1024);

	uiManager->addInterfaz(cocineraSprite);
	uiManager->addInterfaz(camareroSprite);

	auto hostButton = new UiButton(game, "Abrir restaurante", "paella", { 255, 255, 255, 255 }, { 0, 0, 0, 0 },
		sdlutils().width() / 2 - offsetX, sdlutils().height() / 2 - offsetY);
	hostButton->setAction([](Game* game) { 
			//Aqui rayito mete el host
		});
	uiManager->addInterfaz(hostButton);

	auto clientButton = new UiButton(game, "Buscar restaurante", "paella", { 255, 255, 255, 255 }, { 0, 0, 0, 0 },
		sdlutils().width() / 2 + offsetX, sdlutils().height() / 2 - offsetY);
	clientButton->setAction([](Game* game) {
			//Aqui rayito mete el client
		});
	uiManager->addInterfaz(clientButton);
}
