#include "UIManager.h"
#include "../../Data/Comanda.h"
#include "../../GameObjects/UI/RedactaComandabutton.h"
#include "../../sdlutils/InputHandler.h"
#include "../UI/Ingredientebutton.h"
#include <iostream>
using namespace std;
UIManager::UIManager(Game* game)
{
	gamet = game;
}
UIManager::~UIManager()
{
	for (auto i : interfaz)
	{

		delete i;
		i = nullptr;
	}
	
}
void UIManager::uiEvent(int mx, int my)
{
	for (auto i : interfaz)
	{
		if (i->OnClick(mx, my))
		{
			mx = -1;
			my = -1;
		}
	}
	for (auto j: teclado)
	{
		if (j->OnClick(mx, my))
		{
			mx = -1;
			my = -1;
		}
	}
}
void UIManager::update()
{
/*	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_MOUSEBUTTONDOWN && event.button.clicks == 1)
		{
			SDL_GetMouseState(&mx, &my);

			uiEvent(mx, my);
		}

	}*/
	//muy lento
	if(ih().getMouseButtonState(ih().LEFT))
	{
		cout << "UIM DETECTED CLICK ";
		uiEvent(ih().Getmx(), ih().Getmy());
		ih().clearState();
	}
	
}
//void UIManager::render()
//{
//	for (auto i : interfaz)
//	{
//		i->render();
//	}
//	for (auto i : comandas)
//	{
//		i->render();
//		i->dibujaPedido();
//	}
//	for (auto i : teclado)
//	{
//		i->render();
//	}
//}

void UIManager::render(SDL_Rect* rect=nullptr)
{
	for (auto i : interfaz)
	{
		i->render(rect);
	}
	for (auto i : comandas)
	{
		i->render(rect);
		i->dibujaPedido();
	}
	for (auto i : teclado)
	{
		i->render(rect);
	}
}

void UIManager::creaComanda(Game* game)
{
	actual = new Comanda(game, 2,this);
	comandas.push_back(actual);
	creaTeclado();
}
Comanda* UIManager::getComanda()
{
	return actual;
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
		Ingredientebutton* a = new Ingredientebutton(this, gamet, i, (int)posicionesBotones[j].getX(), (int)posicionesBotones[j].getY(), anchobotones, anchobotones);
		teclado.push_back(a);
		//  objectmanager->creaTeclado(a);

		j++;
	}
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