#include "UIManager.h"
#include "../../Data/Comanda.h"
#include "../../GameObjects/UI/RedactaComandabutton.h"
#include "../../sdlutils/InputHandler.h"
#include <iostream>
using namespace std;
UIManager::UIManager(Game* game)
{
	interfaz.push_back(new RedactaComandabutton(game, "redactaboton", 10, 10, 30, 30));

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
void UIManager::render()
{
	for (auto i : interfaz)
	{
		i->render();
	}
	for (auto i : comandas)
	{
		i->render();
	}
}
void UIManager::creaComanda(Game* game)
{
	comandas.push_back(new Comanda(game, 2));
}
void UIManager::creaTeclado()
{
	
}



