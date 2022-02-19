#include "Comanda.h"
#include "../Control/Game.h"
#include "../Ingredientebutton.h"
Comanda::Comanda(Game* game,uint nmesa):GameObject(game)
{
	textureName = cuadernillo;
	Vector2D<double> p;
	p.setX(100);
	p.setY(100);
	setPosition(p);
	setDimension(100, 200);
	gamet = game;
	double ix = p.getX() / 2 + margenbotones+anchobotones/2;
	double iy = p.getY() / 2 + 2*anchobotones;
	escritoX = ix;
	escritoY = iy - anchobotones-margenbotones;
	//creamos las psiciones de los botones del teclado
	for (int i = 0; i <9; i++)
	{
		posicionesBotones.push_back(Point2D<double>(ix, iy));
		ix += anchobotones + margenbotones;
		if (ix >=p.getX() + getWidth()/2-anchobotones/2)
		{
			ix = p.getX() / 2 + margenbotones + anchobotones / 2;
			iy += anchobotones + margenbotones;
		}


	}
	//ahora que tenemos las posiciones creamos el teclado;
	int j = 0;
	for (auto i : texturasIngredienes)
	{
		//Comanda* comanda,Game* game, TextureName texturename, int x, int y, int w, int h
		Ingredientebutton* a = new Ingredientebutton(this,gamet, i,(int) posicionesBotones[j].getX(),(int) posicionesBotones[j].getY(), anchobotones, anchobotones);
		teclado.push_back(a);
		gamet->getObjectManager()->creaTeclado(a);
		
		j++;
	}
}
void Comanda::añadiraPedido(TextureName i)
{
	UiButton *a = new UiButton(gamet,i,escritoX,escritoY,anchobotones/2,anchobotones/2);
	escritoX += anchobotones / 2 +margenbotones;
	gamet->getObjectManager()->creaTeclado(a);
	
	randomizaIconos();
}
void Comanda::randomizaIconos()
{
	vector<Point2D<double>> posdis = posicionesBotones;
	int j = rand() % posdis.size();
	for (auto i : teclado)
	{
		i->setPosition(posdis[j]);
		posdis.erase(posdis.begin() + j);
		if(posdis.size()>0)
		 j = rand() % posdis.size();
	}

}
void Comanda::dibujaPedido()
{
}