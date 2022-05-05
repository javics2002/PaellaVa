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

	int sizePuntW = 120;
	int sizePuntH = 70;
	int posPuntX = sdlutils().width() / 2 / 2;
	int posPuntY = sizePuntH * 2;

	GameObject* medalla = new GameObject(mGame);
	medalla->setTexture("medalla");
	medalla->setInitialDimension(200, 200);
	medalla->setDimension(200, 200);
	medalla->setPosition(sdlutils().width() - 200, 70 * 2);
	mUiManager->addInterfaz(medalla);

	GameObject* puntos = new GameObject(mGame);
	puntos->setTexture("GRACIAS POR JUGAR!", string("finalScene"), SDL_Color{ 255, 255, 255, 255 }, SDL_Color{ 0, 0, 0, 0 });
	puntos->setDimension();
	puntos->setPosition(Vector2D<double>(posPuntX + 50, posPuntY + puntos->getHeight()));
	mUiManager->addInterfaz(puntos);

	auto continueButton = new UiButton(mGame, "continue",
		sdlutils().width() / 2, sdlutils().height() / 2 + 300, 300, 120);
	continueButton->setInitialDimension(300, 120);

	continueButton->setAction([this, continueButton](Game* mGame, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, mGame->UI);

		mUiManager->addTween(0.9f, 1.0f, 600.0f, false).via(easing::exponentialOut).onStep(
			[mGame, continueButton, this](tweeny::tween<float>& t, float) mutable {
				continueButton->setDimension(t.peek() * continueButton->getInitialWidth(),
					t.peek() * continueButton->getInitialHeight());

				if (t.progress() > .2f) {
					//Start game
					mGame->getNetworkManager()->close();

					mGame->sendMessageScene(new Menu(mGame));

					return true;
				}
				return false;
			});
		});
	mUiManager->addButton(continueButton);
}

void FinalScene::render()
{
	mBackground->render(mCamera->renderRect());
	mObjectManager->render(mCamera->renderRect());
	mUiManager->render(nullptr); // ponemos nullptr para que se mantenga en la pantalla siempre
}
