#include "FinalScene.h"
#include "../Scenes/Menu.h"
#include "../Scenes/Jornada.h"
#include "../Control/NetworkManager.h"
#include "../sdlutils/SDLUtils.h"

using tweeny::easing;

FinalScene::FinalScene(Game* game, int mediaPuntuation) : Scene(game)
{
	fondo->setTexture("hostClientBg");
	fondo->setPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	fondo->setDimension(sdlutils().width(), sdlutils().height() + 100);

	int sizePuntW = 120;
	int sizePuntH = 70;
	int posPuntX = sdlutils().width() / 2 / 2;
	int posPuntY = sizePuntH * 2;

	GameObject* medalla = new GameObject(game);
	medalla->setTexture("medalla");
	medalla->setInitialDimension(200, 200);
	medalla->setDimension(200, 200);
	medalla->setPosition(sdlutils().width() - 200, 70 * 2);
	uiManager->addInterfaz(medalla);

	//starTexture = &sdlutils().images().at("estrella");
	//vector<GameObject*> estrellas = vector<GameObject*>(starNumber);
	//for (auto i = 0; i < starNumber; i++) {
	//	estrellas[i] = new GameObject(game);
	//	estrellas[i]->setInitialDimension(110, 100);
	//	estrellas[i]->setDimension(0, 0);
	//	estrellas[i]->setTexture("estrella");
	//	estrellas[i]->setPosition(sdlutils().width() / 2 - 110 * 2 + i * estrellas[i]->getInitialWidth(), 70 * 2);
	//	estrellas[i]->setActive(false);
	//	uiManager->addInterfaz(estrellas[i]);
	//}

	//tweenEstrellas(estrellas);

	GameObject* puntos = new GameObject(game);
	puntos->setTexture("GRACIAS POR JUGAR!", string("finalScene"), SDL_Color{ 255, 255, 255, 255 }, SDL_Color{ 0, 0, 0, 0 });
	puntos->setDimension();
	puntos->setPosition(Vector2D<double>(posPuntX + 50, posPuntY + puntos->getHeight()));
	uiManager->addInterfaz(puntos);

	auto continueButton = new UiButton(game, "continue",
		sdlutils().width() / 2, sdlutils().height() / 2 + 300, 300, 120);
	continueButton->setInitialDimension(300, 120);

	continueButton->setAction([this, continueButton](Game* game, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, game->UI);

		uiManager->addTween(0.9f, 1.0f, 600.0f, false).via(easing::exponentialOut).onStep(
			[game, continueButton, this](tweeny::tween<float>& t, float) mutable {
				continueButton->setDimension(t.peek() * continueButton->getInitialWidth(),
					t.peek() * continueButton->getInitialHeight());

				if (t.progress() > .2f) {
					//Start game
					game->getNetworkManager()->close();

					game->sendMessageScene(new Menu(game));

					return true;
				}
				return false;
			});
		});
	uiManager->addButton(continueButton);
}

void FinalScene::render()
{
	fondo->render(camara->renderRect());
	objectManager->render(camara->renderRect());
	uiManager->render(nullptr); // ponemos nullptr para que se mantenga en la pantalla siempre
}

//void GameOver::tweenEstrellas(vector<GameObject*> estrellas, int i)
//{
//	estrellas[i]->setActive(true);
//	uiManager->addTween(0.0f, 1.0f, 1100.0f, false).via(easing::bounceOut).onStep([=](tweeny::tween<float>& t, float) mutable {
//		estrellas[i]->setDimension(t.peek() * estrellas[i]->getInitialWidth(), t.peek() * estrellas[i]->getInitialHeight());
//		if (t.progress() == 1) {
//			cout << "estrella " << i + 1 << endl;
//			return true;
//		}
//		else if (t.progress() > 0.3f) {
//			//Si quedan m�s estrellas, empezamos su tween
//			if (i + 1 < estrellas.size() && !estrellas[i + 1]->isActive())
//			{
//				tweenEstrellas(estrellas, i + 1);
//			}
//		}
//		return false;
//		});
//
//	//Sonido
//	sdlutils().soundEffects().at(to_string(i + 1) + "star").play(0, -1);
//}
