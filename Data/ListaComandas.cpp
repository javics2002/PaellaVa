#include "ListaComandas.h"
#include "Comanda.h"
#include "../GameObjects/UI/EliminaComandaButton.h"
#include "../Control/Game.h"
#include "../GameObjects/UI/UIManager.h"
ListaComandas::ListaComandas(Game* game,UIManager* m) :GameObject(game)
{

	setTexture("barra");
	Vector2D<double> p;
	p.setX(500);
	p.setY(40);
	setPosition(p);
	cX = p.getX()/2;
	cY = p.getY();
	setDimension(ancho, alto);
	uimt = m;
}
ListaComandas::~ListaComandas()
{
}
void ListaComandas::AñadeComanda(Comanda* comanda)
{
	if (numcomandas < 4)
	{
		Comanda* c = new Comanda(*comanda);
		int x = c->getX();
		desplazamineto = cX - x;
		c->desplazacomandas(desplazamineto);//esta la paella anterior en el mismo  vector 
		c->setPosition(cX, cY);
		c->getPosition().setX(cX);
		lista.push_back(c);
		
		c->setSitio(numcomandas);
		//UIManager* u,Comanda* c, Game* game, string texturename, int x, int y, int w, int h
		EliminaComandaButton* e = new EliminaComandaButton(uimt, c, game, "cancela", cX, cY + c->getHeight()/2, 25, 25);
		//uimt->addInterfaz(e);
		c->setEliminabutton(e);
		numcomandas++;
		cX += 1.5 * c->getWidth();
	}
}
void ListaComandas::renderComandas()
{
	for (int i = 0; i < lista.size(); i++)
	{
		lista[i]->render(nullptr);//se pasan bien pero los uibutton de la lista de paelas deciden morir aqui xd
		lista[i]->dibujaPedido();//XD son punteros y la comanda hace paella clear y los borra lol
	}
}
void ListaComandas::finalizacomanda(Comanda* comanda)
{
	lista.erase(lista.begin() + comanda->getSitio());
	numcomandas--;

}
void ListaComandas::update()
{
}