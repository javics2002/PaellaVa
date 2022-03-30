#include "Menu.h"

#include "../sdlutils/SDLUtils.h"
#include "../Control/Game.h"
#include "HostClient.h"

#include <iostream>

using tweeny::easing;

Menu::Menu(Game* game) : Scene(game)
{
	fondo->setTexture("menufondo");
	fondo->setPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	fondo->setDimension(sdlutils().width(), sdlutils().height()+100);

	int posIni = 260;
	int aumento = 150;
	int w = 300;
	int h = 120;
	auto startButton = new UiButton(game, "start", 640, posIni, w, h);
	startButton->setInitialDimension(170, 100);
	startButton->setAction([this, startButton](Game* game, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, game->UI);

		tweeny::tween<int> startButtonTween = tweeny::tween<int>::from(90).to(105).during(600).via(easing::exponentialOut);

		startButtonTween.onStep([game, startButton](tweeny::tween<int>& t, int) mutable {
			startButton->setDimension((t.peek() / 100.0f) * startButton->getInitialWidth(), (t.peek() / 100.0f) * startButton->getInitialHeight());

			if (t.progress() > .2f) {
				//Start game
				game->changeScene(new HostClient(game));
				return true;
			}
			return false;
			});

		uiManager->addTween(startButtonTween);
		});
	uiManager->addInterfaz(startButton);

	auto settingsButton = new UiButton(game, "settings", 640, posIni + aumento, w, h);
	settingsButton->setInitialDimension(170, 100);
	settingsButton->setAction([this, settingsButton](Game* game, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, game->UI);

		tweeny::tween<int> settingsButtonTween = tweeny::tween<int>::from(90).to(105).during(600).via(easing::exponentialOut);

		settingsButtonTween.onStep([game, settingsButton](tweeny::tween<int>& t, int) mutable {
			settingsButton->setDimension((t.peek() / 100.0f) * settingsButton->getInitialWidth(), (t.peek() / 100.0f) * settingsButton->getInitialHeight());

			if (t.progress() > .2f) {
				//Settings
				return true;
			}
			return false;
			});

		uiManager->addTween(settingsButtonTween);
		});
	uiManager->addInterfaz(settingsButton);

	auto exitButton = new UiButton(game, "exit", 640, posIni + aumento * 2, w, h);
	exitButton->setInitialDimension(170, 100);
	exitButton->setAction([this, exitButton](Game* game, bool& exit) {
		sdlutils().soundEffects().at("cancel").play(0, game->UI);

		tweeny::tween<int> exitButtonTween = tweeny::tween<int>::from(90).to(105).during(600).via(easing::exponentialOut);

		exitButtonTween.onStep([game, exitButton, &exit](tweeny::tween<int>& t, int) mutable {
			exitButton->setDimension((t.peek() / 100.0f) * exitButton->getInitialWidth(), (t.peek() / 100.0f) * exitButton->getInitialHeight());

			if (t.progress() > .2f) {
				//exit game
				exit = true;
				return true;
			}
			return false;
			});

		uiManager->addTween(exitButtonTween);
		});
	uiManager->addInterfaz(exitButton);
}

void Menu::handleInput(bool& exit)
{
	Scene::handleInput(exit);

	if (ih().getKey(InputHandler::CANCEL))
		exit = true;
}
