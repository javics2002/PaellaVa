#include "Menu.h"

#include "../sdlutils/SDLUtils.h"
#include "../Control/Game.h"
#include "HostClient.h"
#include "GameOver.h"
#include "Tutorial.h"

#include <iostream>

using tweeny::easing;

Menu::Menu(Game* game) : Scene(game)
{

	fondo->setTexture("menufondo");
	fondo->setPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	fondo->setDimension(sdlutils().width(), sdlutils().height() + 100);

	sdlutils().musics().at("musicaMenuInicio").play();

	int posIni = 260;
	int aumento = 100;
	int w = 300;
	int h = 120;
	auto startButton = new UiButton(game, "start", 640, posIni - aumento, w, h);
	startButton->setInitialDimension(w, h);
	startButton->setAction([this, startButton](Game* game, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, game->UI);
		
		uiManager->addTween(0.9f, 1.0f, 600.0f,false).via(easing::exponentialOut).onStep([game, startButton](tweeny::tween<float>& t, float) mutable {
			startButton->setDimension(t.peek() * startButton->getInitialWidth(), t.peek() * startButton->getInitialHeight());

			if (t.progress() > .2f) {
				//Start game
				game->sendMessageScene(new Jornada(game,"Jornada1",0,true));
				return true;
			}
			return false;
			});
		});

	uiManager->addButton(startButton);

	auto tutorial = new UiButton(game, "start", 640, posIni, w, h);
	tutorial->setInitialDimension(w, h);
	tutorial->setAction([this, tutorial](Game* game, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, game->UI);

		uiManager->addTween(0.9f, 1.0f, 600.0f, false).via(easing::exponentialOut).onStep([game, tutorial](tweeny::tween<float>& t, float) mutable {
			tutorial->setDimension(t.peek() * tutorial->getInitialWidth(), t.peek() * tutorial->getInitialHeight());

			if (t.progress() > .2f) {
				//Start game
				game->sendMessageScene(new Tutorial(game,"Tutorial"));
				game->getNetworkManager()->init('h', nullptr, game->getNombre());
				return true;
			}
			return false;
			});
		});

	uiManager->addButton(tutorial);

	auto settingsButton = new UiButton(game, "settings", 640, posIni + aumento, w, h);
	settingsButton->setInitialDimension(w, h);
	settingsButton->setAction([this, settingsButton](Game* game, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, game->UI);
		uiManager->addTween(0.9f, 1.0f, 600.0f,false).via(easing::exponentialOut).onStep([game, settingsButton](tweeny::tween<float>& t, float) mutable {
			settingsButton->setDimension(t.peek() * settingsButton->getInitialWidth(), t.peek() * settingsButton->getInitialHeight());

			if (t.progress() > .2f) {
				// Settings

				//Abrir pantalla

				game->getUIManager()->toggleOpciones();
				game->getUIManager()->desactivaBot();

				// -Poner pantalla completa (sdlutils().toggleFullScreen();)
				// -Volumen de musica y sonido (slides separados)

				return true;
			}
			return false;
			});
		});

	uiManager->addButton(settingsButton);

	auto creditsButton = new UiButton(game, "credits", 640, posIni + aumento * 2, w, h);
	creditsButton->setInitialDimension(w, h);
	creditsButton->setAction([this, creditsButton](Game* game, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, game->UI);
		uiManager->addTween(0.9f, 1.0f, 600.0f,false).via(easing::exponentialOut).onStep([game, creditsButton](tweeny::tween<float>& t, float) mutable {
			creditsButton->setDimension(t.peek() * creditsButton->getInitialWidth(), t.peek() * creditsButton->getInitialHeight());

			if (t.progress() > .2f) {
				// Settings

				//Abrir pantalla
				game->getUIManager()->toggleCreditos(0);
				game->getUIManager()->desactivaBot();

				// -Poner pantalla completa (sdlutils().toggleFullScreen();)
				// -Volumen de musica y sonido (slides separados)

				return true;
			}
			return false;
			});
		});

	uiManager->addButton(creditsButton);

	auto exitButton = new UiButton(game, "exit", 640, posIni + aumento * 3, w, h);
	exitButton->setInitialDimension(w, h);
	exitButton->setAction([this, exitButton](Game* game, bool& exit) {
		sdlutils().soundEffects().at("cancel").play(0, game->UI);

		uiManager->addTween(0.9f, 1.0f, 600.0f,false).via(easing::exponentialOut).onStep([game, exitButton, &exit](tweeny::tween<float>& t, float) mutable {
			exitButton->setDimension(t.peek() * exitButton->getInitialWidth(), t.peek() * exitButton->getInitialHeight());

			if (t.progress() == 1.0f) {
				// Exit
				exit = true;
				return true;
			}
			return false;
			});
		});

	uiManager->addButton(exitButton);

	uiManager->creaMenuOpciones();

	uiManager->creaPantallaCreditos();
}

void Menu::handleInput(bool& exit)
{
	Scene::handleInput(exit);

	if (ih().getKey(InputHandler::CANCEL))
		exit = true;
}
