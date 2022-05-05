#include "Menu.h"

#include "../sdlutils/SDLUtils.h"
#include "../Control/Game.h"
#include "HostClient.h"
#include "GameOver.h"
#include "Tutorial.h"

#include <iostream>

using tweeny::easing;

Menu::Menu(Game* mGame) : Scene(mGame)
{
	mBackground->setTexture("menufondo");
	mBackground->setPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	mBackground->setDimension(sdlutils().width(), sdlutils().height() + 100);

	sdlutils().musics().at("musicaMenuInicio").play();

	int posIni = 500;
	int aumento = 100;
	int w = 300;
	int h = 120;
	int posIniX = w;

	auto startButton = new UiButton(mGame, "start", posIniX, posIni - aumento, w, h);
	startButton->setInitialDimension(w, h);
	startButton->setAction([this, startButton](Game* mGame, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, mGame->UI);
		
		mUiManager->addTween(0.9f, 1.0f, 600.0f,false).via(easing::exponentialOut).onStep([mGame, startButton](tweeny::tween<float>& t, float) mutable {
			startButton->setDimension(t.peek() * startButton->getInitialWidth(), t.peek() * startButton->getInitialHeight());

			if (t.progress() > .2f) {
				//Start game
				mGame->sendMessageScene(new HostClient(mGame));
				return true;
			}
			return false;
			});
		});

	mUiManager->addButton(startButton);

	auto tutorial = new UiButton(mGame, "tutorialButton", posIniX, posIni, w, h);
	tutorial->setInitialDimension(w, h);
	tutorial->setAction([this, tutorial](Game* mGame, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, mGame->UI);

		mUiManager->addTween(0.9f, 1.0f, 600.0f, false).via(easing::exponentialOut).onStep([mGame, tutorial](tweeny::tween<float>& t, float) mutable {
			tutorial->setDimension(t.peek() * tutorial->getInitialWidth(), t.peek() * tutorial->getInitialHeight());

			if (t.progress() > .2f) {
				//Start game
				mGame->sendMessageScene(new Tutorial(mGame,"Tutorial"));
				mGame->getNetworkManager()->init('h', nullptr, mGame->getNombre());
				return true;
			}
			return false;
			});
		});

	mUiManager->addButton(tutorial);

	

	auto creditsButton = new UiButton(mGame, "credits", posIniX, posIni + aumento , w, h);
	creditsButton->setInitialDimension(w, h);
	creditsButton->setAction([this, creditsButton](Game* mGame, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, mGame->UI);
		mUiManager->addTween(0.9f, 1.0f, 600.0f,false).via(easing::exponentialOut).onStep([mGame, creditsButton](tweeny::tween<float>& t, float) mutable {
			creditsButton->setDimension(t.peek() * creditsButton->getInitialWidth(), t.peek() * creditsButton->getInitialHeight());

			if (t.progress() > .2f) {
				// Credits
				mGame->getUIManager()->setFromMenu(true);
				mGame->getUIManager()->toggleCreditos(0);
				mGame->getUIManager()->desactivaBot();
				

				return true;
			}
			return false;
			});
		});

	mUiManager->addButton(creditsButton);

	
	int posIniXAbajo = sdlutils().width() - h/2;
	int posIniYAbajo = sdlutils().height() - h/2;
	auto settingsButton = new UiButton(mGame, "settings", posIniXAbajo - h, posIniYAbajo, h, h);
	settingsButton->setInitialDimension(h, h);
	settingsButton->setAction([this, settingsButton](Game* mGame, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, mGame->UI);
		mUiManager->addTween(0.9f, 1.0f, 600.0f, false).via(easing::exponentialOut).onStep([mGame, settingsButton](tweeny::tween<float>& t, float) mutable {
			settingsButton->setDimension(t.peek() * settingsButton->getInitialWidth(), t.peek() * settingsButton->getInitialHeight());

			if (t.progress() > .2f) {
				// Settings

				//Abrir pantalla

				mGame->getUIManager()->toggleOpciones();
				mGame->getUIManager()->desactivaBot();

				// -Poner pantalla completa (sdlutils().toggleFullScreen();)
				// -Volumen de musica y sonido (slides separados)

				return true;
			}
			return false;
			});
		});

	mUiManager->addButton(settingsButton);

	auto exitButton = new UiButton(mGame, "exit", posIniXAbajo, posIniYAbajo, h, h);
	exitButton->setInitialDimension(h, h);
	exitButton->setAction([this, exitButton](Game* mGame, bool& exit) {
		sdlutils().soundEffects().at("cancel").play(0, mGame->UI);

		mUiManager->addTween(0.9f, 1.0f, 600.0f,false).via(easing::exponentialOut).onStep([mGame, exitButton, &exit](tweeny::tween<float>& t, float) mutable {
			exitButton->setDimension(t.peek() * exitButton->getInitialWidth(), t.peek() * exitButton->getInitialHeight());

			if (t.progress() == 1.0f) {
				// Exit
				exit = true;
				return true;
			}
			return false;
			});
		});

	mUiManager->addButton(exitButton);

	mUiManager->creaMenuOpciones();

	mUiManager->creaPantallaCreditos();
}

void Menu::handleInput(bool& exit)
{
	if (!mPaused)mObjectManager->handleInput(exit);
	mUiManager->handleInput(exit, mPaused);

	if (ih().getKey(InputHandler::A) && !mUiManager->getEnAjustes())
		mUiManager->focoExecute(exit);

	if (ih().getKey(InputHandler::RIGHT) || ih().getKey(InputHandler::DOWN))
		mUiManager->avanzaFoco();
	if (ih().getKey(InputHandler::LEFT) || ih().getKey(InputHandler::UP))
		mUiManager->retrocedeFoco();
}
