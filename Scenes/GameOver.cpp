#include "GameOver.h"
#include "../GameObjects/UI/Text.h"

GameOver::GameOver(Game* game, int puntuacion) : Scene(game)
{
	fondo->setTexture("reloj");
	fondo->setPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	fondo->setDimension(sdlutils().width(), sdlutils().height() + 100);

	int sizePuntW = 120;
	int sizePuntH = 70;
	int posPuntX = sdlutils().width()/2/2;
	int posPuntY = sizePuntH*2;

	int numEstrellas = 3;
	
	GameObject* estrella = new GameObject(game);
	estrella->setTexture("estrella");
	estrella->setDimension(50, 50);
	estrella->setPosition(Vector2D<double>(posPuntX-estrella->getWidth(), posPuntY));
	uiManager->addInterfaz(estrella);
	
	

	GameObject* puntos = new GameObject(game);
	puntos->setTexture(string("Puntuacion: " + to_string(puntuacion)), string("paella"), SDL_Color{ 255, 255, 255, 255 }, SDL_Color{ 0, 0, 0, 0 });
	puntos->setDimension();
	puntos->setPosition(Vector2D<double>(posPuntX, posPuntY+puntos->getHeight()));
	uiManager->addInterfaz(puntos);

	GameObject *review = new GameObject(game);
	review->setTexture(string("Review asi medio larga"), string("paella"), SDL_Color { 255, 255, 255, 255 }, SDL_Color { 0, 0, 0, 0 });
	review->setDimension();
	review->setPosition(Vector2D<double>(sdlutils().width() - review->getWidth(), posPuntY));
	uiManager->addInterfaz(review);


	GameObject* periodico = new GameObject(game);
	periodico->setTexture("periodico");
	periodico->setDimension();
	periodico->setPosition(Vector2D<double>(sdlutils().width() - review->getWidth(), posPuntY+periodico->getHeight()-100));
	uiManager->addInterfaz(periodico);

}
