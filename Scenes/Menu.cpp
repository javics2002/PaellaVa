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

	int mPosIni = 500;
	int mAumento = 100;
	int mWidth = 300;
	int mHeight = 120;
	int mPosIniX = mWidth;

	auto mStartButton = new UiButton(mGame, "start", mPosIniX, mPosIni - mAumento, mWidth, mHeight);
	mStartButton->setInitialDimension(mWidth, mHeight);
	mStartButton->setAction([this, mStartButton](Game* mGame, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, mGame->UI);
		
		mUiManager->addTween(0.9f, 1.0f, 600.0f,false).via(easing::exponentialOut).onStep([mGame, mStartButton](tweeny::tween<float>& t, float) mutable {
			mStartButton->setDimension(t.peek() * mStartButton->getInitialWidth(), t.peek() * mStartButton->getInitialHeight());

			if (t.progress() > .2f) {
				//Start game
				mGame->sendMessageScene(new HostClient(mGame));
				return true;
			}
			return false;
			});
		});

	mUiManager->addButton(mStartButton);

	auto mTutorial = new UiButton(mGame, "tutorialButton", mPosIniX, mPosIni, mWidth, mHeight);
	mTutorial->setInitialDimension(mWidth, mHeight);
	mTutorial->setAction([this, mTutorial](Game* mGame, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, mGame->UI);

		mUiManager->addTween(0.9f, 1.0f, 600.0f, false).via(easing::exponentialOut).onStep([mGame, mTutorial](tweeny::tween<float>& t, float) mutable {
			mTutorial->setDimension(t.peek() * mTutorial->getInitialWidth(), t.peek() * mTutorial->getInitialHeight());

			if (t.progress() > .2f) {
				//Start game
				mGame->sendMessageScene(new Tutorial(mGame,"Tutorial"));
				mGame->getNetworkManager()->init('h', nullptr, mGame->getNombre());
				return true;
			}
			return false;
			});
		});

	mUiManager->addButton(mTutorial);

	

	auto mCreditsButton = new UiButton(mGame, "credits", mPosIniX, mPosIni + mAumento , mWidth, mHeight);
	mCreditsButton->setInitialDimension(mWidth, mHeight);
	mCreditsButton->setAction([this, mCreditsButton](Game* mGame, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, mGame->UI);
		mUiManager->addTween(0.9f, 1.0f, 600.0f,false).via(easing::exponentialOut).onStep([mGame, mCreditsButton](tweeny::tween<float>& t, float) mutable {
			mCreditsButton->setDimension(t.peek() * mCreditsButton->getInitialWidth(), t.peek() * mCreditsButton->getInitialHeight());

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

	mUiManager->addButton(mCreditsButton);

	
	int mPosIniAbajo = sdlutils().width() - mHeight / 2;
	int mPosIniYAbajo = sdlutils().height() - mHeight / 2;
	auto mSettingsButton = new UiButton(mGame, "settings", mPosIniAbajo - mHeight, mPosIniYAbajo, mHeight, mHeight);
	mSettingsButton->setInitialDimension(mHeight, mHeight);
	mSettingsButton->setAction([this, mSettingsButton](Game* mGame, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, mGame->UI);
		mUiManager->addTween(0.9f, 1.0f, 600.0f, false).via(easing::exponentialOut).onStep([mGame, mSettingsButton](tweeny::tween<float>& t, float) mutable {
			mSettingsButton->setDimension(t.peek() * mSettingsButton->getInitialWidth(), t.peek() * mSettingsButton->getInitialHeight());

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

	mUiManager->addButton(mSettingsButton);

	auto mExitButton = new UiButton(mGame, "exit", mPosIniAbajo, mPosIniYAbajo, mHeight, mHeight);
	mExitButton->setInitialDimension(mHeight, mHeight);
	mExitButton->setAction([this, mExitButton](Game* mGame, bool& exit) {
		sdlutils().soundEffects().at("cancel").play(0, mGame->UI);

		mUiManager->addTween(0.9f, 1.0f, 600.0f,false).via(easing::exponentialOut).onStep([mGame, mExitButton, &exit](tweeny::tween<float>& t, float) mutable {
			mExitButton->setDimension(t.peek() * mExitButton->getInitialWidth(), t.peek() * mExitButton->getInitialHeight());

			if (t.progress() == 1.0f) {
				// Exit
				exit = true;
				return true;
			}
			return false;
			});
		});

	mUiManager->addButton(mExitButton);

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
