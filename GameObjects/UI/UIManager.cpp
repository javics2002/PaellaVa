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
#include "../../Data/ListaComandas.h"
#include <iostream>

using namespace std;

UIManager::UIManager(Game* game)
{
	this->game = game;
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
	for (int i = 0; i < interfaz.size(); ++i)
	{
		if (interfaz[i]->isActive())
		{
			if (interfaz[i]->OnClick(mx, my))
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
			if (j->OnClick(mx, my))
			{
				mx = -1;
				my = -1;
			}
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
	if (ih().getMouseButtonState(InputHandler::MOUSE_LEFT))
	{
		cout << "UIM DETECTED CLICK ";
		uiEvent(ih().getmx(), ih().getmy());
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
		if(i->isActive())
		i->render(rect);
	}

}

void UIManager::creaComanda(Game* game)
{
	actual = new Comanda(game, 2, this);
	comandas.push_back(actual);

	creaTeclado();
	actual->guardaTeclado();
	AceptaPaellaButton* e = new AceptaPaellaButton(game, actual, "acepta", actual->getPosition().getX() + actual->getWidth() / 2 + anchobotones / 4, actual->getPosition().getY() + actual->getHeight() / 4, anchobotones, anchobotones);
	interfaz.push_back(e);
	actual->guardaBoton(e);
	DescartaCommandaButton* b = new DescartaCommandaButton(actual, game, "cancela", actual->getPosition().getX() + actual->getWidth() / 2 + anchobotones / 4, actual->getPosition().getY() + actual->getHeight() / 4 - anchobotones, anchobotones, anchobotones);
	interfaz.push_back(b);
	actual->guardaBoton(b);
	BorrarButton* bo = new BorrarButton(game, actual, "borra", actual->getPosition().getX() + actual->getWidth() / 2 + anchobotones / 4, actual->getPosition().getY() + actual->getHeight() / 4 - 2 * anchobotones, anchobotones, anchobotones);
	interfaz.push_back(bo);
	actual->guardaBoton(bo);
	EnviaComandaButton* en = new EnviaComandaButton(game, "envia", actual->getPosition().getX() + actual->getWidth() / 2 + anchobotones / 4, actual->getPosition().getY() + actual->getHeight() / 4 + anchobotones, anchobotones, anchobotones);
	interfaz.push_back(en);
	actual->guardaBoton(en);

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
		Ingredientebutton* a = new Ingredientebutton(this, game, i, (int)posicionesBotones[j].getX(), (int)posicionesBotones[j].getY(), anchobotones, anchobotones);
		teclado.push_back(a);
		//  objectmanager->creaTeclado(a);

		j++;
		
	}
	j = 0;
	for (auto i :texturasNumeros)
	{
	
		vector <Numerobutton*> tecladonum;
	
		//Comanda comanda,Game* game, TextureName texturename, int x, int y, int w, int h
		Numerobutton* a = new Numerobutton(this, gamet, i, (int)posicionesBotones[j].getX(), (int)posicionesBotones[j].getY(), anchobotones, anchobotones);
		teclado.push_back(a);
		//  objectmanager->creaTeclado(a);

		j++;
	}
	j = 0;
	for (auto i : texturasTamanos)
	{
	
	/*	vector <Tamanobutton*> tecladotam;
		//Comanda comanda,Game* game, TextureName texturename, int x, int y, int w, int h
		Tamanobutton* a = new Tamanobutton(this, gamet, i, (int)posicionesBotones[j].getX(), (int)posicionesBotones[j].getY(), anchobotones, anchobotones);
		teclado.push_back(a);
		//  objectmanager->creaTeclado(a);

		j++;*/
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