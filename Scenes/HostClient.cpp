#include "HostClient.h"
#include "../Scenes/Lobby.h"
#include "../Control/NetworkManager.h"
#include "../Scenes/Jornada.h"
#include "../GameObjects/UI/ShowText.h"
#include "IntroduceIP.h"

HostClient::HostClient(Game* game) : Scene(game)
{
	fondo->setTexture("hostClientBg");
	fondo->setPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	fondo->setDimension(sdlutils().width(), sdlutils().height() + 100);

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
	hostButton->setInitialDimension(hostButton->getWidth(), hostButton->getHeight());
	hostButton->setAction([](Game* game, bool& exit) {
		//Host
		game->getNetworkManager()->init('h', nullptr, game->getNombre());

		game->sendMessageScene(new Lobby(game));
		});
	uiManager->addButton(hostButton);

	auto clientButton = new UiButton(game, "Buscar restaurante", "paella", { 255, 255, 255, 255 }, { 0, 0, 0, 0 },
		sdlutils().width() / 2 + offsetX, sdlutils().height() / 2 - offsetY);
	clientButton->setInitialDimension(clientButton->getWidth(), clientButton->getHeight());

	introIp = new UiButton(game, "libreta", clientButton->getX(), clientButton->getY() - 110 , libretaTexture->width() / 12, libretaTexture->height());
	introIp->setActive(false);

	ShowText* ip = new ShowText(game, " ", "abadiNombre", introIp->getX(), introIp->getY());
	ip->setActive(true);

	clientButton->setAction([this, ip](Game* game, bool& exit) {
		//Client
		//game->sendMessageScene(new IntroduceIP(game));
		if (!introIp->isActive()) {
			uiManager->setIpButton(introIp);
		}

		else {
			uiManager->setIpButton(nullptr);
			ip_ = " ";
			ip->setTexture(ip_, string("abadiNombre"), { 0, 0, 0, 0 }, { 0, 0, 0, 0 });
			ip->setDimension();
		}

		introIp->setActive(!introIp->isActive());
		//ip->setActive(introIp->isActive());
		});

	introIp->setAction([ip, this](Game* game, bool& exit) {

		char c = ih().getTypedKey();

		if (c != ' ')
		{
			if (c == '\b')
			{

				if (!ip_.empty())
					ip_.pop_back();
				else
					ip_ = ' ';
			}

			else if (ip_.size() < maxCaracteres) {
				ip_ += c;
			}

			if (ip_.empty())
				ip_ = ' ';

			ip->setTexture(ip_, string("abadiNombre"), { 0, 0, 0, 0 }, { 0, 0, 0, 0 });
			ip->setDimension();

		}
		ip->render(nullptr);
		});

	uiManager->addInterfaz(ip);
	uiManager->addButton(clientButton);
}




