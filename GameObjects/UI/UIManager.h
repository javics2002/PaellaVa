#pragma once
#include "../UI/UiButton.h"
class Game;

using namespace std;

class UIManager
{
	vector<GameObject*> interfaz;//el resto de iconos 
	vector<GameObject*> comandas;//comandas de la lista de comandas
	vector<GameObject*> teclado;//iconos que se usan en la comanda qeu esta redactando
	//vector <GameObject*> pedido;//pedido que se está redactando lo lleva la comanda
	int mx;
	int my;
public:
	UIManager(Game* game);
	~UIManager();
	void uiEvent(int mx, int my);
	void update();
	void render();
	void creaComanda(Game* game);
	void creaTeclado();
};

