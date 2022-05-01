#include "HostClient.h"
#include "../Scenes/Lobby.h"
#include "../Control/NetworkManager.h"
#include "../Scenes/Jornada.h"
#include "../GameObjects/UI/ShowText.h"
#include "IntroduceIP.h"
#include "../Scenes/Menu.h"

HostClient::HostClient(Game* game) : Scene(game)
{
	fondo->setTexture("hostClientBg");
	fondo->setPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	fondo->setDimension(sdlutils().width(), sdlutils().height() + 100);

	int offsetX = 300, offsetY = 150;

	light = new ParticleExample();
	light->setRenderer(sdlutils().renderer());
	light->setStyle(ParticleExample::NONE);

	UiButton* regresar = new UiButton(game, "back", 70, 50, 125, 60);
	regresar->setInitialDimension(regresar->getWidth(), regresar->getHeight());
	regresar->setAction([](Game* game, bool& exit) {
		game->sendMessageScene(new Menu(game));
		});

	uiManager->addButton(regresar);

	hostButton = new UiButton(game, "cocinera",
		sdlutils().width() / 2 - offsetX, sdlutils().height() - 350, 500, 700);
	hostButton->setInitialDimension(500, 700);
	hostButton->setAction([](Game* game, bool& exit) {
		//Host
		game->getNetworkManager()->init('h', nullptr, game->getNombre());

		game->sendMessageScene(new Lobby(game));
		});
	uiManager->addButton(hostButton);

	clientButton = new UiButton(game, "camarero",
		sdlutils().width() / 2 + offsetX, sdlutils().height() - 350, 500, 700);
	clientButton->setInitialDimension(500, 700);

	posYNotOver = 165;
	posYOver = 145;
	ip = new ShowText(game, " ", "ip", clientButton->getX() + 20, posYNotOver);
	ip->setActive(true);

	cursor = new ShowText(game, "|", "ipCursor", clientButton->getX() + 20, posYNotOver);
	cursor->setActive(false);

	clientButton->setAction([this](Game* game, bool& exit) {
		//Client
		if (!escribiendo) {
			escribiendo = true;
			ih().clearInputBuffer();
			cursor->setActive(true);
			tiempo = sdlutils().currRealTime();
		}

		else {
			if (esValida(ip_)) {
				game->sendMessageScene(new IntroduceIP(game));
			}

			else {
				cout << "Ip no valida so zorra";
			}
		}
		});

	uiManager->addButton(clientButton);

	
}

bool HostClient::esValida(string ipText)
{
	if (ipText == " ") return false;

	vector<string> aux = split(ipText);

	if (aux.size() < 4) return false;

	for (int i = 0; i < aux.size(); i++) {

		string digito = aux[i];

		if (digito.size() == 0) return false;

		for (int j = 0; j < digito.size(); j++) {
			
			if (!std::isdigit(digito[j])) {
				return false;
			}
		}

		if (stoi(digito) >= 256) {
			return false;
		}
	}

	return true;
}

void HostClient::update()
{
	Scene::update();
	light->update();

	if (escribiendo) {

		char c = ih().getTypedKey();

		if (c != ' ' && c != '\r')
		{
			if (!escrito) escrito = true;

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

			ip->setTexture(ip_, string("ip"), {255,255,255,0}, { 0, 0, 0, 0 });
			ip->setDimension();

		}
		ip->render(nullptr);
	}
	//Preguntar isHover y 
	if (clientButton->hover()) {
		ip->setPosition(ip->getX(), posYOver);
		cursor->setPosition(cursor->getX(), posYOver);
		light->setStyle(ParticleExample::LIGHT);
		light->setPosition(clientButton->getX(), sdlutils().height());
	}

	else {
		ip->setPosition(ip->getX(), posYNotOver);
		cursor->setPosition(cursor->getX(), posYNotOver);	
	}

	if (hostButton->hover()) {
		light->setStyle(ParticleExample::LIGHT);
		light->setPosition(hostButton->getX(), sdlutils().height());
	}

}

vector<string> HostClient::split(string ipText)
{
	vector<string> ip;
	char delimitador = '.';

	string digitos = "";

	for (int i = 0; i < ipText.size(); i++) {
		if (ipText[i] != delimitador) {
			digitos += ipText[i];
			if(i + 1 == ipText.size()) ip.push_back(digitos);
		}

		else {
			ip.push_back(digitos);
			digitos = "";
		}
	}

	return ip;
}

void HostClient::render() {
	fondo->render(camara->renderRect());
	objectManager->render(camara->renderRect());
	light->draw(camara->renderRect());
	uiManager->render(nullptr); // ponemos nullptr para que se mantenga en la pantalla siempre
	ip->render(nullptr);

	if (!escrito && sdlutils().currRealTime() - tiempo > frameRate) {
		cursor->render(nullptr);
		tiempo = sdlutils().currRealTime();
	}

	textMngr->render();
}



