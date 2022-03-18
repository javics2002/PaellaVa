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
	Comanda c = *comanda;
	int x = c.getX();
	desplazamineto = cX - x;
	c.desplazacomandas(desplazamineto);
	c.setPosition(cX, cY);
	lista.push_back(c);
	cX += 1.5*c.getWidth();
}
void ListaComandas::renderComandas()
{
	for (int i = 0; i < lista.size(); i++)
	{
		lista[i].render(nullptr);
		lista[i].dibujaPedido();
	}
}