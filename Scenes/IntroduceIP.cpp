#include "IntroduceIP.h"
#include "../GameObjects/UI/UiButton.h"
#include "../sdlutils/SDLUtils.h"
IntroduceIP::IntroduceIP(Game* game):Scene(game)
{
	IP= new UiButton(game,hIP, "paella", { 255, 255, 255, 255 }, { 0, 0, 0, 0 },sdlutils().width() / 2 , sdlutils().height() / 2 );
	uiManager->addInterfaz(IP);
}
void IntroduceIP::handleInput(bool& exit)
{
	char c = ih().getTypedKey();
	if (c!= ' ')
	{
		if (c == '\b')
		{
			if (!hIP.empty())
				hIP.pop_back();
			else
				hIP = ' ';
		}
		else
		hIP += c;

		if (hIP.empty())
			hIP = ' ';
		IP->setTexture(hIP, string("paella"), { 255, 255, 255, 255 }, { 0, 0, 0, 0 });
		IP->setDimension();
		
		//cout << hIP <<"h"; //se actualiza mal hay que hacer un callback
	}
	IP->render(nullptr);
}