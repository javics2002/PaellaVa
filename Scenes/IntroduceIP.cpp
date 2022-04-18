#include "IntroduceIP.h"
#include "../GameObjects/UI/UiButton.h"
#include "../sdlutils/SDLUtils.h"
#include "../Control/NetworkManager.h"
#include "../Scenes/Lobby.h"

IntroduceIP::IntroduceIP(Game* game) : Scene(game)
{
	IP = new UiButton(game,textIP, "paella", { 255, 255, 255, 255 }, { 0, 0, 0, 0 },sdlutils().width() / 2 , sdlutils().height() / 2 );
	uiManager->addButton(IP);

	auto hostButton = new UiButton(game, "Introduce IP", "paella", { 255, 255, 255, 255 }, { 0, 0, 0, 0 },
		sdlutils().width() / 2, sdlutils().height() / 2 - 100);
	

	hostButton->setAction([game, this](Game* game, bool& exit) {
		//Host
		textIP.erase(remove(textIP.begin(), textIP.end(), ' '), textIP.end());

		if (game->getNetworkManager()->init('c', textIP.c_str(), game->getNombre())) {
			// crear el lobby
			game->sendMessageScene(new Lobby(game, game->getNetworkManager()->getOtherName()));
		}

		});

	uiManager->addInterfaz(hostButton);
}

void IntroduceIP::handleInput(bool& exit)
{
	char c = ih().getTypedKey();
	if (c!= ' ')
	{
		if (c == '\b')
		{
			if (!textIP.empty())
				textIP.pop_back();
			else
				textIP = ' ';
		}
		else
		textIP += c;

		if (textIP.empty())
			textIP = ' ';
		IP->setTexture(textIP, string("paella"), { 255, 255, 255, 255 }, { 0, 0, 0, 0 });
		IP->setDimension();
		
		//cout << hIP <<"h"; //se actualiza mal hay que hacer un callback
	}

	if (ih().getMouseButtonState(InputHandler::MOUSE_LEFT))
	{
		//cout << nombrePlayer << endl;
		int mx = ih().getmx();
		int my = ih().getmy();

		for (int i = 0u; i < uiManager->getInterfaz().size(); ++i)
		{
			if (uiManager->getInterfaz()[i]->isActive() && !paused)
			{
				if (uiManager->getInterfaz()[i]->onClick(mx, my, exit))
				{
					mx = -1;
					my = -1;
				}
			}
		}

	}

	
}