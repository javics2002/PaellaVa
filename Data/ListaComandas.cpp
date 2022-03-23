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
	cX = p.getX()/5;
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
	Comanda* c = new Comanda(*comanda);
	limit = inicx + maxvisibles * 1.5 * c->getWidth();
	if (numcomandas < maxvisibles)
	{
		
		int x = c->getX();
		if (lista.empty())
		{
			cX = inicx;//añadimos siempre al principio y empujamos las demas
		}
		else
		{
			auto ic = lista.begin();
			
			Comanda* d=*ic;
			cX = d->getPosition().getX() - 1.5 * c->getWidth();
		}
		//comprobar si la posicion 0 está libre
		/*if (numcomandas > 0)
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
		}*/
		//cX = inicx + (numcomandas * 1.5 * c->getWidth());//al llenar y vaciar de locos pero añadir si ya hay 3 , en la posicion 0 ....
		desplazamineto = cX - x;
		
		c->desplazacomandas(desplazamineto);//esta la paella anterior en el mismo  vector 
		c->setPosition(cX, cY);
		c->getPosition().setX(cX);
		//c->setSitio();
		c->setSitio(lista.insert(lista.begin(),c));
		//c->setSitio(inicx/(1.5 * c->getWidth())-1); AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
		//c->setSitio(numcomandas);
		//UIManager* u,Comanda* c, Game* game, string texturename, int x, int y, int w, int h
		EliminaComandaButton* e = new EliminaComandaButton(uimt, c, game, "cancela", cX, cY + c->getHeight()/2, 25, 25);
		//uimt->addInterfaz(e);
		c->setEliminabutton(e);
		numcomandas++;
		//cX += 1.5 * c->getWidth();
		auto ec = lista.end();
		ec--;
		Comanda* ed = *ec;
		if (ed->getPosition().getX() < limit)
		{
			for (auto c : lista)//al agregar otra comanda se desplazan topdas y la ultima se sale :(
			{



				int nx = c->getPosition().getX() + 1.5 * c->getWidth();
				int dsp = nx - c->getPosition().getX();
				c->desplazacomandas(dsp);//esta la paella anterior en el mismo  vector 
				c->setPosition(nx, cY);
				c->getEliminabutton()->setPosition(nx, cY + c->getHeight() / 2);

			}
		}

	}
	else 
	{
	//se guardan en el buffer de comandas.
		c->desplazacomandas(-500);//esta la paella anterior en el mismo  vector 
		c->setPosition(-500, cY);
		EliminaComandaButton* e = new EliminaComandaButton(uimt, c, game, "cancela", -500, cY + c->getHeight() / 2, 25, 25);
	
		c->setEliminabutton(e);
		listanovisibles.push(c);
	}
}
void ListaComandas::renderComandas()
{
	for (auto i: lista)
	{
		
			i->render(nullptr);//se pasan bien pero los uibutton de la lista de paelas deciden morir aqui xd
			i->dibujaPedido();//XD son punteros y la comanda hace paella clear y los borra lol
		
	}
}
void ListaComandas::finalizacomanda(Comanda* comanda)
{
	/*int desplazables = maxvisibles- comanda->getSitio();
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
	}*/
	if (comanda == selected)
		selected = nullptr;
	for (auto c : lista)
	{

		if (c->getPosition().getX()<comanda->getPosition().getX())
		{
			int nx = c->getPosition().getX() + 1.5 * c->getWidth();
			int dsp = nx - c->getPosition().getX();
			c->desplazacomandas(dsp);//esta la paella anterior en el mismo  vector 
			c->setPosition(nx, cY);
			c->getEliminabutton()->setPosition(nx, cY + c->getHeight() / 2);
		}
	}
	lista.erase( comanda->getSitio());
	numcomandas--;
	if (!listanovisibles.empty())
	{

	
	
	
		//traer comanda del buffer 
		Comanda* c = listanovisibles.back();
		//AñadeComanda(c);
		int x = c->getX();
		
		
			
		
		
		
			auto ic = lista.begin();

			Comanda* d = *ic;
			cX = d->getPosition().getX() - 1.5 * c->getWidth();
		
		
		
		desplazamineto = cX - x;

		c->desplazacomandas(desplazamineto -  c->getWidth());//esta la paella anterior en el mismo  vector 
		c->setPosition(cX, cY);
		c->getPosition().setX(cX);
		
		c->setSitio(lista.insert(lista.begin(), c));
		c->getEliminabutton()->setPosition(cX, cY + c->getHeight() / 2);
		
		numcomandas++;
			
		listanovisibles.pop();

	}

}
void ListaComandas::update()
{
}
void ListaComandas::seleccionaComanda(Comanda* comanda)
{
	if (selected != nullptr)
	{
		selected->deseleccionaComanda();
		selected = comanda;
	}
	else
		selected = comanda;
}
Comanda* ListaComandas::getComandaSeleccionada()
{
	if (selected == nullptr)
	{
	}
	else
	return selected;
}