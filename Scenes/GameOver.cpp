#include "GameOver.h"
#include "../Scenes/Menu.h"
#include "../Scenes/Jornada.h"
#include "../Control/NetworkManager.h"
#include "../sdlutils/SDLUtils.h"

using tweeny::easing;

GameOver::GameOver(Game* game, int puntuation, int numeroJornada) : Scene(game)
{
	nJornada = numeroJornada;
	points = puntuation;
	calculateStarNumber();
	createRandomReviews();
	createRandomUsernames();
	fondo->setTexture("hostClientBg");
	fondo->setPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	fondo->setDimension(sdlutils().width(), sdlutils().height() + 100);

	int sizePuntW = 120;
	int sizePuntH = 70;
	int posPuntX = sdlutils().width() / 2 / 2;
	int posPuntY = sizePuntH * 2;

	starTexture = &sdlutils().images().at("estrella");
	vector<GameObject*> estrellas = vector<GameObject*>(starNumber);
	for (auto i = 0; i < starNumber; i++) {
		estrellas[i] = new GameObject(game);
		estrellas[i]->setInitialDimension(110, 100);
		estrellas[i]->setDimension(0, 0);
		estrellas[i]->setTexture("estrella");
		estrellas[i]->setPosition(sdlutils().width() / 4 - estrellas[i]->getInitialWidth() + i * estrellas[i]->getInitialWidth(), 70 * 2);
		estrellas[i]->setActive(false);
		uiManager->addInterfaz(estrellas[i]);
	}

	tweenEstrellas(estrellas);
	
	GameObject* puntos = new GameObject(game);
	puntos->setTexture(string("Puntuacion: " + to_string(starNumber)), string("paella"), SDL_Color{ 255, 255, 255, 255 }, SDL_Color{ 0, 0, 0, 0 });
	puntos->setDimension();
	puntos->setPosition(Vector2D<double>(posPuntX, posPuntY + puntos->getHeight()));
	uiManager->addInterfaz(puntos);

	auto continueButton = new UiButton(game, "continue",
		sdlutils().width() / 2 + 350, sdlutils().height() / 2 + 300, 300, 120);
	continueButton->setInitialDimension(300, 120);

	continueButton->setAction([this, continueButton](Game* game, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, game->UI);

		uiManager->addTween(0.9f, 1.0f, 600.0f,false).via(easing::exponentialOut).onStep(
			[game, continueButton,this](tweeny::tween<float>& t, float) mutable {
			continueButton->setDimension(t.peek() * continueButton->getInitialWidth(), 
				t.peek() * continueButton->getInitialHeight());
			
			if (t.progress() > .2f) {
				//Start game
				if (nombreTilemaps.size() > nJornada) {
					game->sendMessageScene(new Jornada(game, nombreTilemaps[nJornada], nJornada+1, true));

					// send info 
					game->getNetworkManager()->sendStartGame(nJornada+1);
				}
				else {
					game->getNetworkManager()->close();
					game->sendMessageScene(new Menu(game));
				}
				return true;
			}
			return false;
			});
		});
	uiManager->addButton(continueButton);
}

void GameOver::calculateStarNumber()
{
	if (points > pointStarValue * maxStars)
		starNumber = maxStars;
	else if (points <= pointStarValue)
		starNumber = 1;
	else
		starNumber = points / pointStarValue;
}

void GameOver::render()
{
	fondo->render(camara->renderRect());
	objectManager->render(camara->renderRect());
	uiManager->render(nullptr); // ponemos nullptr para que se mantenga en la pantalla siempre

	renderReviews();
}

void GameOver::createRandomReviews()
{
	//coge review random del json segun el numero de estrellas
	auto& rand = sdlutils().rand();
	std::vector<int> numReviewsByStar{ 12, 10, 10, 15, 15 }; //pos 0 = 1 star

	int numberOfReviews = 2;

	for (int i = 0; i < numberOfReviews; i++) {
		string reviewId = "";

		reviewId = to_string(starNumber) + "stars" + to_string(rand.nextInt(1, numReviewsByStar[starNumber - 1] + 1));

		m_reviews.push_back(&sdlutils().messages().at(reviewId));
	}
}

void GameOver::createRandomUsernames()
{
	//coge review random del json segun el numero de estrellas
	auto& rand = sdlutils().rand();
	int numUsernames = 21;
	int numberOfReviews = 2; //tiene que ser igual al de arriba

	for (int i = 0; i < numberOfReviews; i++) {
		string userId = "";

		userId = "username" + to_string(rand.nextInt(1, numUsernames + 1));

		m_usernames.push_back(&sdlutils().messages().at(userId));
	}
}

void GameOver::renderReviews() {
	
	for (int i = 0; i < m_reviews.size(); i++) {
		int letterHeight = m_reviews[i]->height();
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
		SDL_Rect destBox = build_sdlrect(posTextBox, sdlutils().width() * 0.8, m_reviews[i]->height() * espacioBlanco);
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
		SDL_Rect destUser = build_sdlrect(posUser, m_usernames[i]->width(), m_usernames[i]->height());

		//render del username
		m_usernames[i]->render(destUser, 0.0f);

		//rect de las estrellas
		//render de las estrellas
		for (int i = 0; i < starNumber; i++) {
			Vector2D<int> posStar = Vector2D<int>(posX + wProfile + i*15, posYUser+letterHeight);
			SDL_Rect destStar = build_sdlrect(posStar, 15, 15);

			Texture* reviewStar = &sdlutils().images().at("estrellaReview");
			reviewStar->render(destStar, 0.0f);
		}

		//rect del texto
		Vector2D<int> pos = Vector2D<int>(posX+letterWidth, posY+wProfile+letterHeight);
		SDL_Rect dest = build_sdlrect(pos, m_reviews[i]->width(), m_reviews[i]->height());

		//render del texto
		m_reviews[i]->render(dest, 0.0f);
		

	}
}

void GameOver::tweenEstrellas(vector<GameObject*> estrellas, int i)
{
	estrellas[i]->setActive(true);
	uiManager->addTween(0.0f, 1.0f, 1100.0f,false).via(easing::bounceOut).onStep([=](tweeny::tween<float>& t, float) mutable {
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
