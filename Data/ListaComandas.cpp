#include "ListaComandas.h"
#include "Comanda.h"
ListaComandas::ListaComandas(Game* game) :GameObject(game)
{

	setTexture("barra");
	Vector2D<double> p;
	p.setX(500);
	p.setY(40);
	setPosition(p);
	cX = p.getX();
	cY = p.getY();
	setDimension(ancho, alto);
}
ListaComandas::~ListaComandas()
{
}
void ListaComandas::AñadeComanda(Comanda* comanda)
{
	Comanda* c = new Comanda( *comanda);
	int x = c->getX();
	desplazamineto = cX - x;
	c->desplazacomandas(desplazamineto);//esta la paella anterior en el mismo  vector 
	c->setPosition(cX, cY);
	c->getPosition().setX(cX);
	lista.push_back(c);
	cX += 1.5*c->getWidth();
	
}
void ListaComandas::renderComandas()
{
	for (int i = 0; i < lista.size(); i++)
	{
		lista[i]->render(nullptr);//se pasan bien pero los uibutton de la lista de paelas deciden morir aqui xd
		lista[i]->dibujaPedido();//XD son punteros y la comanda hace paella clear y los borra lol
	}
}