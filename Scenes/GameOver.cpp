#include "GameOver.h"
#include "../Scenes/Menu.h"
#include "../Scenes/Jornada.h"
#include "../Scenes/FinalScene.h"
#include "../Control/NetworkManager.h"
#include "../sdlutils/SDLUtils.h"

using tweeny::easing;

GameOver::GameOver(Game* mGame, int puntuation, int numeroJornada) : Scene(mGame)
{
	mNumJornada = numeroJornada;
	mPoints = puntuation;
	calculateStarNumber();
	createRandomReviews();
	createRandomUsernames();
	mBackground->setTexture("hostClientBg");
	mBackground->setPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	mBackground->setDimension(sdlutils().width(), sdlutils().height() + 100);

	int mSizePuntW = 120;
	int mSizePuntH = 70;
	int mPosPuntX = sdlutils().width() / 2 / 2;
	int mPosPuntY = mSizePuntH * 2;

	GameObject* mStarMold = new GameObject(mGame);
	mStarMold->setTexture("estrellaMolde");
	mStarMold->setInitialDimension(110*5, 110);
	mStarMold->setDimension(110*5, 110);
	mStarMold->setPosition(sdlutils().width() / 2, 70 * 2);
	mUiManager->addInterfaz(mStarMold);

	mStarTexture = &sdlutils().images().at("estrella");
	vector<GameObject*> mStars = vector<GameObject*>(mStarNumber);
	for (auto i = 0; i < mStarNumber; i++) {
		mStars[i] = new GameObject(mGame);
		mStars[i]->setInitialDimension(110, 100);
		mStars[i]->setDimension(0, 0);
		mStars[i]->setTexture("estrella");
		mStars[i]->setPosition(sdlutils().width() / 2 -110*2 + i * mStars[i]->getInitialWidth(), 70 * 2);
		mStars[i]->setActive(false);
		mUiManager->addInterfaz(mStars[i]);
	}

	tweenEstrellas(mStars);

	auto mContinueButton = new UiButton(mGame, "continue",
		sdlutils().width() / 2, sdlutils().height() / 2 + 300, 300, 120);
	mContinueButton->setInitialDimension(300, 120);

	mContinueButton->setAction([this, mContinueButton](Game* mGame, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, mGame->UI);

		mUiManager->addTween(0.9f, 1.0f, 600.0f,false).via(easing::exponentialOut).onStep(
			[mGame, mContinueButton,this](tweeny::tween<float>& t, float) mutable {
			mContinueButton->setDimension(t.peek() * mContinueButton->getInitialWidth(), 
				t.peek() * mContinueButton->getInitialHeight());
			
			if (t.progress() > .2f) {
				//Start game
				if (nombreTilemaps.size() > mNumJornada) {
					mGame->sendMessageScene(new Jornada(mGame, nombreTilemaps[mNumJornada], mNumJornada+1, true));

					// send info 
					mGame->getNetworkManager()->sendStartGame(mNumJornada+1);
				}
				else {
					mGame->sendMessageScene(new FinalScene(mGame));

					mGame->getNetworkManager()->sendFinishGame(100, 0, true);
				}
				return true;
			}
			return false;
			});
		});
	mUiManager->addButton(mContinueButton);
}

void GameOver::calculateStarNumber()
{
	if (mPoints > mPointStarValue * mMaxStars)
		mStarNumber = mMaxStars;
	else if (mPoints <= mPointStarValue)
		mStarNumber = 1;
	else
		mStarNumber = mPoints / mPointStarValue;
}

void GameOver::render()
{
	mBackground->render(mCamera->renderRect());
	mObjectManager->render(mCamera->renderRect());
	mUiManager->render(nullptr); // ponemos nullptr para que se mantenga en la pantalla siempre

	renderReviews();
}

void GameOver::createRandomReviews()
{
	//coge review random del json segun el numero de estrellas
	auto& mRand = sdlutils().rand();
	std::vector<int> mNumReviewsByStar{ 12, 10, 10, 15, 15 }; //pos 0 = 1 star

	int mNumOfReviews = 2;

	for (int i = 0; i < mNumOfReviews; i++) {
		string reviewId = "";

		reviewId = to_string(mStarNumber) + "stars" + to_string(mRand.nextInt(1, mNumReviewsByStar[mStarNumber - 1] + 1));

		mReviews.push_back(&sdlutils().messages().at(reviewId));
	}
}

void GameOver::createRandomUsernames()
{
	//coge review random del json segun el numero de estrellas
	auto& mRand = sdlutils().rand();
	int mNumUsernames = 21;
	int mNumOfReviews = 2; //tiene que ser igual al de arriba

	for (int i = 0; i < mNumOfReviews; i++) {
		string userId = "";

		userId = "username" + to_string(mRand.nextInt(1, mNumUsernames + 1));

		mUsernames.push_back(&sdlutils().messages().at(userId));
	}
}

void GameOver::renderReviews() {
	
	for (int i = 0; i < mReviews.size(); i++) {
		int letterHeight = mReviews[i]->height();
		int letterWidth = letterHeight;
		int espacioBlanco = 8;
		int espacioEntreRevs = letterHeight*i;
		//pos de la primera linea del cuadro de texto
		int posX = 100;
		int posY = 300 + (i * letterHeight * espacioBlanco) + espacioEntreRevs;

		//rect de la caja de texto
		int posXBox = posX - letterWidth * 2;
		int posYBox = posY - letterHeight;
		Vector2D<int> posTextBox = Vector2D<int>(posXBox, posYBox);
		SDL_Rect destBox = build_sdlrect(posTextBox, sdlutils().width() * 0.8, mReviews[i]->height() * espacioBlanco);
		//render de text box
		Texture* textBox = &sdlutils().images().at("review");
		textBox->render(destBox, 0.0f);

		//rect de la profile picture
		int wProfile = letterWidth * 3.5;
		Vector2D<int> posProfile = Vector2D<int>(posX, posY);
		SDL_Rect destProfile = build_sdlrect(posProfile, wProfile, wProfile);

		//render de la prof pic
		Texture* profile = &sdlutils().images().at("profilePicture");
		profile->render(destProfile, 0.0f);

		//rect del username
		int posYUser = posY + letterHeight/2;
		Vector2D<int> posUser = Vector2D<int>(posX + wProfile, posYUser);
		SDL_Rect destUser = build_sdlrect(posUser, mUsernames[i]->width(), mUsernames[i]->height());

		//render del username
		mUsernames[i]->render(destUser, 0.0f);

		//rect de las estrellas
		//render de las estrellas
		for (int i = 0; i < mStarNumber; i++) {
			Vector2D<int> posStar = Vector2D<int>(posX + wProfile + i*15, posYUser+letterHeight);
			SDL_Rect destStar = build_sdlrect(posStar, 15, 15);

			Texture* reviewStar = &sdlutils().images().at("estrellaReview");
			reviewStar->render(destStar, 0.0f);
		}

		//rect del texto
		Vector2D<int> pos = Vector2D<int>(posX+letterWidth, posY+wProfile+letterHeight);
		SDL_Rect dest = build_sdlrect(pos, mReviews[i]->width(), mReviews[i]->height());

		//render del texto
		mReviews[i]->render(dest, 0.0f);
		

	}
}

void GameOver::tweenEstrellas(vector<GameObject*> estrellas, int i)
{
	estrellas[i]->setActive(true);
	mUiManager->addTween(0.0f, 1.0f, 1100.0f,false).via(easing::bounceOut).onStep([=](tweeny::tween<float>& t, float) mutable {
		estrellas[i]->setDimension(t.peek() * estrellas[i]->getInitialWidth(), t.peek() * estrellas[i]->getInitialHeight());
		if (t.progress() == 1) {
			cout << "estrella " << i + 1 << endl;
			return true;
		}
		else if (t.progress() > 0.3f) {
			//Si quedan más estrellas, empezamos su tween
			if (i + 1 < estrellas.size() && !estrellas[i + 1]->isActive())
			{
				tweenEstrellas(estrellas, i + 1);
			}
		}
		return false;
		});

	//Sonido
	sdlutils().soundEffects().at(to_string(i + 1) + "star").play(0, -1);
}
