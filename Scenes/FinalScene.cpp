#include "FinalScene.h"
#include "../Scenes/Menu.h"
#include "../Scenes/Jornada.h"
#include "../Control/NetworkManager.h"
#include "../sdlutils/SDLUtils.h"

using tweeny::easing;

FinalScene::FinalScene(Game* mGame) : Scene(mGame)
{
	mBackground->setTexture("hostClientBg");
	mBackground->setPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	mBackground->setDimension(sdlutils().width(), sdlutils().height() + 100);

	int mSizePuntW = 120;
	int mSizePuntH = 70;
	int mPosPuntX = sdlutils().width() / 2 / 2;
	int mPosPuntY = mSizePuntH * 2;

	GameObject* mReward = new GameObject(mGame);
	mReward->setTexture("medalla");
	mReward->setInitialDimension(200, 200);
	mReward->setDimension(200, 200);
	mReward->setPosition(sdlutils().width() - 200, 70 * 2);
	mUiManager->addInterfaz(mReward);

	GameObject* mText = new GameObject(mGame);
	mText->setTexture("GRACIAS POR JUGAR!", string("finalScene"), SDL_Color{ 255, 255, 255, 255 }, SDL_Color{ 0, 0, 0, 0 });
	mText->setDimension();
	mText->setPosition(Vector2D<double>(mPosPuntX + 50, mPosPuntY + mText->getHeight()));
	mUiManager->addInterfaz(mText);

	auto mContinueButton = new UiButton(mGame, "continue",
		sdlutils().width() / 2, sdlutils().height() / 2 + 300, 300, 120);
	mContinueButton->setInitialDimension(300, 120);

	mContinueButton->setAction([this, mContinueButton](Game* mGame, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, mGame->UI);

		mUiManager->addTween(0.9f, 1.0f, 600.0f, false).via(easing::exponentialOut).onStep(
			[mGame, mContinueButton, this](tweeny::tween<float>& t, float) mutable {
				mContinueButton->setDimension(t.peek() * mContinueButton->getInitialWidth(),
					t.peek() * mContinueButton->getInitialHeight());

				if (t.progress() > .2f) {
					//Start game
					mGame->getNetworkManager()->close();

					mGame->sendMessageScene(new Menu(mGame));

					return true;
				}
				return false;
			});
		});
	mUiManager->addButton(mContinueButton);
}

void FinalScene::render()
{
	mBackground->render(mCamera->renderRect());
	mObjectManager->render(mCamera->renderRect());
	mUiManager->render(nullptr); // ponemos nullptr para que se mantenga en la pantalla siempre
}
