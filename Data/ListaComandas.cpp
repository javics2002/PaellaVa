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
	inicx = cX;
}
ListaComandas::~ListaComandas()
{
}
void ListaComandas::AñadeComanda(Comanda* comanda)
{
	if (numcomandas < maxvisibles)
	{
		Comanda* c = new Comanda(*comanda);
		int x = c->getX();
		c->setSitio(numcomandas);
		//comprobar si la posicion 0 está libre
		if (numcomandas > 0)
		{


			if (lista[0]->getPosition().getX() != inicx)
			{
				//priemraposicion de la lista libre
				cX = inicx;
				//shift derecho visual y en el vector
				for (auto c : lista)
				{

					
						//c->setSitio(c->getSitio()+ 1);
					
				}
				
			}
			else 
				cX = inicx + (numcomandas * 1.5 * c->getWidth());
		}
		//cX = inicx + (numcomandas * 1.5 * c->getWidth());//al llenar y vaciar de locos pero añadir si ya hay 3 , en la posicion 0 ....
		desplazamineto = cX - x;
		
		c->desplazacomandas(desplazamineto);//esta la paella anterior en el mismo  vector 
		c->setPosition(cX, cY);
		c->getPosition().setX(cX);
		lista.push_back(c);
		//c->setSitio(inicx/(1.5 * c->getWidth())-1); AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
		//c->setSitio(numcomandas);
		//UIManager* u,Comanda* c, Game* game, string texturename, int x, int y, int w, int h
		EliminaComandaButton* e = new EliminaComandaButton(uimt, c, game, "cancela", cX, cY + c->getHeight()/2, 25, 25);
		//uimt->addInterfaz(e);
		c->setEliminabutton(e);
		numcomandas++;
		//cX += 1.5 * c->getWidth();

	}
	else 
	{
	//se guardan en el buffer de comandas.
	
	}
}
void ListaComandas::renderComandas()
{
	for (int i = 0; i < lista.size(); i++)
	{
		if (lista[i] != nullptr)
		{
			lista[i]->render(nullptr);//se pasan bien pero los uibutton de la lista de paelas deciden morir aqui xd
			lista[i]->dibujaPedido();//XD son punteros y la comanda hace paella clear y los borra lol
		}
		}
}
void ListaComandas::finalizacomanda(Comanda* comanda)
{
	int desplazables = maxvisibles- comanda->getSitio();
	int aborrar = comanda->getSitio();

	//delete lista[comanda->getSitio()];
	//lista[comanda->getSitio()] = nullptr;

	//desplazamiento estetico a la derecha
	for (auto c : lista)
	{

		if (c->getSitio() < aborrar)
		{
			int nx=c->getPosition().getX()+ 1.5 * c->getWidth();
			int dsp = nx - c->getPosition().getX();
			c->desplazacomandas(dsp);//esta la paella anterior en el mismo  vector 
			c->setPosition(nx, cY);
			c->getEliminabutton()->setPosition(nx, cY+c->getHeight()/2);
		}
	}

	//shift del vector a la izquierda
	for (auto c : lista)
	{

		if (c->getSitio() > aborrar)
		{
			c->setSitio(c->getSitio() - 1);

		}
	}
	lista.erase(lista.begin() + comanda->getSitio());
	if(listanovisibles.empty())
	numcomandas--;
	else
	{
		//traer comanda del buffer 
	}

}
void ListaComandas::update()
{
}