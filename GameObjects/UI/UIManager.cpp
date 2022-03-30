#include "UIManager.h"
#include "../../Data/Comanda.h"
#include "../../GameObjects/UI/RedactaComandabutton.h"
#include "../../sdlutils/InputHandler.h"
#include "../UI/Ingredientebutton.h"
#include "../UI/Numerobutton.h"
#include "../UI/Tamanobutton.h"
#include "DescartaCommandaButton.h"
#include "EnviaComandaButton.h"
#include "BorrarButton.h"
#include "AceptaPaellaButton.h"
#include "EliminaComandaButton.h"
#include "../../Data/ListaComandas.h"

#include "../../Scenes/Restaurante.h"
#include "../../Scenes/Menu.h"

#include <iostream>

UIManager::UIManager(Game* game)
{
	this->game = game;
	anchobotones *= uiscale;

}

UIManager::~UIManager()
{
	for (auto i : interfaz)
	{
		delete i;
		i = nullptr;
	}

	for (auto i : pauseMenu) {
		delete i;
		i = nullptr;
	}

	for (auto i : pauseButtons) {
		delete i;
		i = nullptr;
	}
}

void UIManager::uiEvent(int mx, int my, bool& exit)
{
	for (int i = 0; i < interfaz.size(); ++i)
	{
		if (interfaz[i]->isActive())
		{
			if (interfaz[i]->onClick(mx, my, exit))
			{
				mx = -1;
				my = -1;
			}
		}
	}

	for (auto j : teclado)
	{
		if (j->isActive())
		{
			if (j->onClick(mx, my, exit))
			{
				mx = -1;
				my = -1;
			}
		}
	}
	for (auto j : uicomandas)
	{
		if (j->isActive())
		{
			if (j->onClick(mx, my, exit))
			{
				mx = -1;
				my = -1;
			}
		}
	}

	if (barra != nullptr)
	{
		for (auto c : barra->getlista())
		{

			if (c->getEliminabutton()->onClick(mx, my, exit))
			{
				mx = -1;
				my = -1;
			}
			if (c->onClick(mx, my, exit))
			{
				mx = -1;
				my = -1;
			}
		}
	}

	for (auto i : pauseButtons)
	{
		if (i->isActive())
		{
			if (i->onClick(mx, my, exit))
			{
				mx = -1;
				my = -1;
			}
		}
	}
}

void UIManager::handleInput(bool& exit)
{
	if (ih().getMouseButtonState(InputHandler::MOUSE_LEFT))
	{
		uiEvent(ih().getmx(), ih().getmy(), exit);
	}
}

void UIManager::update()
{
	for (auto i : interfaz)
	{
		if (i->isActive())
		{
			i->update();
		}
	}
}

void UIManager::render(SDL_Rect* rect = nullptr)
{
	if (barra != nullptr) {
		barra->render(rect);
		barra->renderComandas();
	}

	for (auto i : interfaz)
	{
		if (i->isActive())
		{
			i->render(rect);
		}
	}

	for (auto i : comandas)
	{
		if (i->isActive())
		{
			i->render(rect);
			i->dibujaPedido();
		}
	}

	for (auto i : teclado)
	{
		if (i->isActive())
		{
			i->render(rect);
		}
	}

	for (auto i : uicomandas)
	{
		if (i->isActive())
			i->render(rect);
	}

	for (auto i : pauseMenu) {
		if (i->isActive())
			i->render(rect);
	}

	for (auto i : pauseButtons) {
		if (i->isActive())
			i->render(rect);
	}

	for (auto i = activeTweens.begin(); i != activeTweens.end(); i++) {
		i->step(20);
	}
}

void UIManager::creaComanda(Game* game)
{
	actual = new Comanda(game, uiscale, this);
	comandas.push_back(actual);

	creaTeclado();
	actual->guardaTeclado();
	actual->toggleTeclado(false);
	actual->toggleTecaldotam(false);

	AceptaPaellaButton* aceptaPaellaButton = new AceptaPaellaButton(game, actual, "acepta", actual->getPosition().getX() + actual->getWidth() / 2 + anchobotones / 4, actual->getPosition().getY() + actual->getHeight() / 4, uiscale * anchobotones, uiscale * anchobotones);
	interfaz.push_back(aceptaPaellaButton);
	actual->guardaBoton(aceptaPaellaButton);
	DescartaCommandaButton* descartaComandaButton = new DescartaCommandaButton(actual, game, "cancela", actual->getPosition().getX() + actual->getWidth() / 2 + anchobotones / 4, actual->getPosition().getY() + actual->getHeight() / 4 - anchobotones, uiscale * anchobotones, uiscale * anchobotones);
	interfaz.push_back(descartaComandaButton);
	actual->guardaBoton(descartaComandaButton);
	BorrarButton* borraComandaButton = new BorrarButton(game, actual, "borra", actual->getPosition().getX() + actual->getWidth() / 2 + anchobotones / 4, actual->getPosition().getY() + actual->getHeight() / 4 - 2 * anchobotones, uiscale * anchobotones, uiscale * anchobotones);
	interfaz.push_back(borraComandaButton);
	actual->guardaBoton(borraComandaButton);
	EnviaComandaButton* enviaComandaButton = new EnviaComandaButton(game, "envia", actual->getPosition().getX() + actual->getWidth() / 2 + anchobotones / 4, actual->getPosition().getY() + actual->getHeight() / 4 + anchobotones, uiscale * anchobotones, uiscale * anchobotones);
	interfaz.push_back(enviaComandaButton);
	actual->guardaBoton(enviaComandaButton);
}

Comanda* UIManager::getComanda()
{
	return actual;
}

ListaComandas* UIManager::getBarra()
{
	return barra;
}

void UIManager::setBarra(ListaComandas* b)
{
	barra = b;
}

void UIManager::creaTeclado()
{
	int margenbotones = 5;
	double ix = actual->getPosition().getX() / 2 + margenbotones + anchobotones / 2;
	double iy = actual->getPosition().getY() / 2 + 2 * anchobotones;
	for (int i = 0; i < 9; ++i)
	{
		posicionesBotones.push_back(Point2D<double>(ix, iy));
		ix += anchobotones + margenbotones;
		if (ix >= actual->getPosition().getX() + actual->getWidth() / 2 - anchobotones / 2)
		{
			ix = actual->getPosition().getX() / 2 + margenbotones + anchobotones / 2;
			iy += anchobotones + margenbotones;
		}
	}
	int j = 0;
	for (auto i : texturasIngredienes)
	{
		//Comanda comanda,Game* game, TextureName texturename, int x, int y, int w, int h
		IngredienteButton* a = new IngredienteButton(this, game, i, (int)posicionesBotones[j].getX(), (int)posicionesBotones[j].getY(), uiscale * anchobotones, uiscale * anchobotones);
		teclado.push_back(a);
		//  objectmanager->creaTeclado(a);

		j++;
	}
	j = 0;
	vector <NumeroButton*> tecladonum;

	for (auto i : texturasNumeros)
	{
		//Comanda comanda,Game* game, TextureName texturename, int x, int y, int w, int h
		NumeroButton* a = new NumeroButton(this, game, i, (int)posicionesBotones[j].getX(), (int)posicionesBotones[j].getY(), uiscale * anchobotones, uiscale * anchobotones);
		tecladonum.push_back(a);
		uicomandas.push_back(a);
		j++;
	}

	actual->guardaTecladonum(tecladonum);

	j = 0;
	vector <TamanoButton*> tecladotam;
	for (auto i : texturasTamanos)
	{
		TamanoButton* a = new TamanoButton(this, game, i, (int)posicionesBotones[j].getX(), (int)posicionesBotones[j].getY(), uiscale * anchobotones, uiscale * anchobotones);

		tecladotam.push_back(a);
		uicomandas.push_back(a);
		j++;
	}
	actual->guardaTecladotam(tecladotam);

}

void UIManager::randomizaTeclado()
{
	vector<Point2D<double>> posdis = posicionesBotones;
	int j = rand() % posdis.size();
	for (auto i : teclado)
	{
		i->setPosition(posdis[j]);
		posdis.erase(posdis.begin() + j);
		if (posdis.size() > 0)
			j = rand() % posdis.size();
	}
}

void UIManager::addInterfaz(GameObject* elementoInterfaz)
{
	interfaz.push_back(elementoInterfaz);
}

vector<Point2D<double>> UIManager::getPosTeclado()
{
	return posicionesBotones;
}

void UIManager::setPosTeclado(vector<Point2D<double>> t)
{
	posicionesBotones = t;
}

void UIManager::creaMenuPausa() {
	// crear men�
	Imagen* bg = new Imagen(game, sdlutils().width() / 2, sdlutils().height() / 2, sdlutils().width(), sdlutils().height(), "pause2");
	bg->setActive(false);
	pauseMenu.push_back(bg);

	Imagen* pauseImage = new Imagen(game, sdlutils().width() / 2, sdlutils().height() / 2, 300, 300, "pause1");
	pauseImage->setActive(false);
	pauseMenu.push_back(pauseImage);

	UiButton* resumeButton = new UiButton(game, "resumeBoton", sdlutils().width() / 2, sdlutils().height() / 2 - 25, 200, 100);
	resumeButton->setInitialDimension(200, 100);
	resumeButton->setActive(false);

	resumeButton->setAction([this, resumeButton](Game* game, bool& exit) {
		// Pausa
		Restaurante* currentScene = dynamic_cast<Restaurante*>(game->getCurrentScene());

		addTween(0.9f, 1.0f, 600.0f).via(easing::exponentialOut).onStep([resumeButton, currentScene](tweeny::tween<float>& t, float) mutable {
			resumeButton->setDimension(t.peek() * resumeButton->getInitialWidth(), t.peek() * resumeButton->getInitialHeight());

			if (t.progress() > .2f) { //Busco una respuesta mas rapida
				// Resume
				currentScene->togglePause();
				return true;
			}
			return false;
			});
		});

	pauseButtons.push_back(resumeButton);

	UiButton* exitButton = new UiButton(game, "exitBoton", sdlutils().width() / 2, sdlutils().height() / 2 + 30, 200, 100);
	exitButton->setInitialDimension(200, 100);
	exitButton->setActive(false);

	exitButton->setAction([this, exitButton](Game* game, bool& exit) mutable {
		sdlutils().soundEffects().at("select").play(0, game->UI);

		addTween(0.9f, 1.0f, 600.0f).via(easing::exponentialOut).onStep([game, exitButton](tweeny::tween<float>& t, float) mutable {
			exitButton->setDimension(t.peek() * exitButton->getInitialWidth(), t.peek() * exitButton->getInitialHeight());

			if (t.progress() == 1.0f) {
				// Exit
				game->changeScene(new Menu(game));
				return true;
			}
			return false;
			});
		});

	pauseButtons.push_back(exitButton);
}

void UIManager::togglePause() {
	for (auto i : pauseMenu) {
		i->setActive(!i->isActive());
	}

	//Solo una imagen tiene el tween
	auto pauseImage = pauseMenu[1];
	pauseImage->setDimension(0, 0);

	if (pauseImage->isActive()) {
		addTween(0.8f, 1.0f, 600.0f).via(easing::exponentialOut).onStep([pauseImage](tweeny::tween<float>& t, float) mutable {
			pauseImage->setDimension(t.peek() * pauseImage->getInitialWidth(), t.peek() * pauseImage->getInitialHeight());

			if (t.progress() == 1.0f) {
				return true;
			}
			return false;
			});
	}

	for (auto i : pauseButtons) {
		i->setDimension(0, 0);
		i->setActive(!i->isActive());

		if (i->isActive()) {
			addTween(0.8f, 1.0f, 600.0f).via(easing::exponentialOut).onStep([i](tweeny::tween<float>& t, float) mutable {
				i->setDimension(t.peek() * i->getInitialWidth(), t.peek() * i->getInitialHeight());

				if (t.progress() == 1.0f) {
					return true;
				}
				return false;
				});
		}
	}
}

tweeny::tween<float>& UIManager::addTween(float from, float to, float during) {
	tweeny::tween<float> tween = tweeny::tween<float>::from(from).to(to).during(during);
	activeTweens.push_front(tween);

	return activeTweens.front();
}
