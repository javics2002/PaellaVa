#include "GameOver.h"
#include "../GameObjects/UI/Text.h"
#include "../Scenes/Menu.h"
#include "../Scenes/Restaurante.h"
#include "../Control/NetworkManager.h"
#include "../sdlutils/SDLUtils.h"

GameOver::GameOver(Game* game, int puntuation) : Scene(game)
{
	points = puntuation;
	points = 10;
	calculateStarNumber();
	createRandomReviews();

	fondo->setTexture("reloj");
	fondo->setPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	fondo->setDimension(sdlutils().width(), sdlutils().height() + 100);

	int sizePuntW = 120;
	int sizePuntH = 70;
	int posPuntX = sdlutils().width() / 2 / 2;
	int posPuntY = sizePuntH * 2;

	int numEstrellas = 3;

	starTexture = &sdlutils().images().at("estrella");

	//pos = pos + Vector2D<int>(100 + 80, 0.0f);




	GameObject* estrella = new GameObject(game);
	estrella->setTexture("estrella");
	estrella->setDimension(110, 100);
	estrella->setPosition(Vector2D<double>(posPuntX - estrella->getWidth(), posPuntY));
	uiManager->addInterfaz(estrella);
	

	
	GameObject* puntos = new GameObject(game);
	puntos->setTexture(string("Puntuacion: " + to_string(starNumber)), string("paella"), SDL_Color{ 255, 255, 255, 255 }, SDL_Color{ 0, 0, 0, 0 });
	puntos->setDimension();
	puntos->setPosition(Vector2D<double>(posPuntX, posPuntY + puntos->getHeight()));
	uiManager->addInterfaz(puntos);

	auto continueButton = new UiButton(game, "Volver", "paella", { 255, 0, 0, 255 }, { 0, 0, 0, 0 },
		sdlutils().width() / 2 + 300, sdlutils().height() / 2 + 200);
	continueButton->setAction([](Game* game, bool& exit) {
		game->getNetworkManager()->close();
		game->changeScene(new Menu(game));
		});
	uiManager->addInterfaz(continueButton);

	auto mejoraButton = new UiButton(game, "mas velocidad", "paella", { 0, 0, 255, 255 }, { 0, 0, 0, 0 },
		300, sdlutils().height() / 2 + 200);
	mejoraButton->setAction([](Game* game, bool& exit) {

		});
	uiManager->addInterfaz(mejoraButton);

	auto mejoraButton2 = new UiButton(game, "mas propina", "paella", { 0, 0, 255, 255 }, { 0, 0, 0, 0 },
		300, sdlutils().height() / 2 + 250);
	mejoraButton2->setAction([](Game* game, bool& exit) {

		});
	uiManager->addInterfaz(mejoraButton2);
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
	auto& rand = sdlutils().rand();
	std::vector<int> numReviewsByStar{ 7, 5, 5, 6, 10 }; //pos 0 = 1 star

	int numberOfReviews = 2;

	for (int i = 0; i < numberOfReviews; i++) {
		string reviewId = "";

		reviewId = to_string(starNumber) + "stars" + to_string(rand.nextInt(1, numReviewsByStar[starNumber - 1] + 1));

		m_reviews.push_back(&sdlutils().messages().at(reviewId));
	}
	

}

void GameOver::renderReviews() {
	int letterHeight = 16;
	int letterWidth = 16;
	for (int i = 0; i < m_reviews.size(); i++) {
		//rect del texto
		int posX = 100;
		int posY = 300 + i * letterHeight * 3;
		Vector2D<int> pos = Vector2D<int>(posX, posY);
		SDL_Rect dest = build_sdlrect(pos, m_reviews[i]->width(), m_reviews[i]->height());

		//rect de la caja de texto
		Vector2D<int> pos2 = Vector2D<int>(posX - letterWidth, posY - letterHeight);
		SDL_Rect destBox = build_sdlrect(pos2, sdlutils().width() * 0.8, m_reviews[i]->height() * 3);

		//render de text box
		Texture* textBox = &sdlutils().images().at("review");
		textBox->render(destBox, 0.0f);

		//render del texto
		m_reviews[i]->render(dest, 0.0f);
	}
}
