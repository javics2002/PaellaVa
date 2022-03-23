#include "Comanda.h"
#include "../GameObjects/UI/Ingredientebutton.h"
#include "../GameObjects/UI/Numerobutton.h"
#include "../GameObjects/UI/Tamanobutton.h"
#include  "../GameObjects/UI/EliminaComandaButton.h"

#include "../Control/Game.h"
#include "../Control/ObjectManager.h"
#include "../GameObjects/UI/UIManager.h"
#include "ListaComandas.h"

Comanda::Comanda(Game* game, uint nmesa, UIManager* uim) :GameObject(game)
{
	setTexture("cuadernillo");
	Vector2D<double> p;
	x = 100;
	y = 110;
	p.setX(x);
	p.setY(y);
	setPosition(p);
	setDimension(ancho, alto);
	altoini = alto;
	iniy = p.getY();
	uiManager = uim;
	//   objectmanager = gamet->getObjectManager();
	double ix = p.getX() / 2 + margenbotones + anchobotones / 2;
	double iy = p.getY() / 2 + 2 * anchobotones;
	margenizquierdo = ix;
	margensuperior = iy;
	escritoX = ix;
	escritoY = iy - anchobotones - margenbotones;
	//teclado inicial igualq eu lso magenes y eso para resetear la comanda bien
	 //creamos las psiciones de los botones del teclado
}
Comanda::Comanda(Comanda& c) : GameObject(c.game)
{
	setTexture("cuadernillo");
	setPosition(c.x, c.y);
	paellas = c.copyPaellas();
	game = c.game;
	setDimension(c.ancho, c.alto);
	numeroPaellas = c.numeroPaellas;
	uiManager = c.uiManager;
	for (int i = 0; i < c.numeroPaellas; i++)
	{
		paellas.push_back(c.paellas[i]);
	}
	if (c.numeromesa != nullptr)
	{
		UiButton* nm = new UiButton(game, c.numeromesa->getTextura(), c.numeromesa->getPosition().getX(), c.numeromesa->getPosition().getY(), c.numeromesa->getWidth(), c.numeromesa->getHeight());
		numeromesa = nm;

	}
}
Comanda::~Comanda()
{
	//si hay leaks esq aqui tengo qeu borrar algun puntero de botone spero creo que el uimanager ya los borra todos
	if (eliminarboton != nullptr)
		delete eliminarboton; eliminarboton = nullptr;
}
void Comanda::añadiraPedido(string i)
{

	UiButton* a = new UiButton(game, i, escritoX, escritoY, anchobotones / 2, anchobotones / 2);
	escritoX += anchobotones / 2 + margenbotones;
	//gamet->getObjectManager()->creaTeclado(a);
	Pedido.push_back(a);
	if (Pedido.size() % 4 == 0)
	{
		escritoY += anchobotones / 2 + margenbotones;
		escritoX = getPosition().getX() / 2 + margenbotones + anchobotones / 2;
		alto = alto + anchobotones / 2 + 2 * margenbotones;
		setDimension(ancho, alto);
		setPosition(getPosition().getX(), getPosition().getY() + 2 * margenbotones);
		vector<Point2D<double>> sangria = uiManager->getPosTeclado();
		for (int i = 0; i < sangria.size(); i++)
		{
			int ny = sangria[i].getY() + anchobotones * 0.7f;
			sangria[i].setY(ny);
			//en algun lugar vuelven a tener el valor default lo tengo que mirar
			//bajar teclado
			//lo bajará en uim?
		}
		uiManager->setPosTeclado(sangria);
	}
	randomizaIconos();

}
void Comanda::anadirNumeromesa(string n)
{
	numeromesa = new UiButton(game, n, x + 1.5 * anchobotones, anchobotones, anchobotones / 2, anchobotones / 2);
	toggleTecladonum(false);
	toggleTecaldotam(true);
}
UiButton* Comanda::getNumeromesa()
{
	return numeromesa;
}
void Comanda::randomizaIconos()
{
	vector<Point2D<double>> posdis = uiManager->getPosTeclado();
	int j = rand() % posdis.size();
	for (auto i : teclado)
	{
		i->setPosition(posdis[j]);
		posdis.erase(posdis.begin() + j);
		if (posdis.size() > 0)
			j = rand() % posdis.size();
	}

}
void Comanda::dibujaPedido()
{
	if (numeromesa != nullptr)
	{
		numeromesa->render(nullptr);
	}
	for (auto i : Pedido)
	{
		i->render(nullptr);
	}
	renderizaPaellas();
}
void Comanda::borraPedido()
{//FALTA SUBIR EL TECLADO DE BOTONES AL BORRAR
	bool saltolinea = Pedido.size() % 4 == 0;//por si acaso el metodo que escribe ya hace saltos de liena se pueden anular en algunos casos
	if (Pedido.size() > 0)
	{

		Pedido.erase(Pedido.begin() + Pedido.size() - 1, Pedido.begin() + Pedido.size());
		escritoX -= anchobotones / 2 + margenbotones;
		if (escritoX < margenizquierdo)
		{
			if (saltolinea)
			{
				escritoY -= anchobotones / 2 + margenbotones;
				escritoX = margenizquierdo + 3 * (anchobotones / 2 + margenbotones);
				vector<Point2D<double>> sangria = uiManager->getPosTeclado();
				for (int i = 0; i < sangria.size(); i++)
				{
					int ny = sangria[i].getY() - anchobotones * 0.7f;
					sangria[i].setY(ny);
					//en algun lugar vuelven a tener el valor default lo tengo que mirar
					//bajar teclado
					//lo bajará en uim?
				}
				uiManager->setPosTeclado(sangria);
				uiManager->randomizaTeclado();
			}
			else escritoX = margenbotones;
			//siendo 4 el numero maximo de iconos por linea
		}
	}
}
void Comanda::cancelaPedido()
{
	Pedido.clear();
	escritoX = margenizquierdo;
	escritoY = margensuperior;
	alto = altoini;
	setDimension(ancho, altoini);
	setPosition(getPosition().getX(), iniy);
	uiManager->setPosTeclado(postecladoini);
	uiManager->randomizaTeclado();
	clearPaellas();
	numeroPaellas = 0;
	delete numeromesa; numeromesa = nullptr;
	toggleTecaldotam(false);
	toggleTeclado(false);
	if (isActive())
	{
		toggleTecladonum(true);
	}
}

void Comanda::guardaTeclado()
{
	vector<Point2D<double>> sangria = uiManager->getPosTeclado();
	for (int i = 0; i < sangria.size(); i++)
	{
		int ny = sangria[i].getY() + anchobotones * 0.7f;
		sangria[i].setY(ny);
		//en algun lugar vuelven a tener el valor default lo tengo que mirar
		//bajar teclado
		//lo bajará en uim?
	}
	//uimt->setPosTeclado(sangria);
	postecladoini = sangria;
	teclado = uiManager->getTeclado();
	//pero al inicial le falta la primera sangria y queda por encima de la primera linea de pedido D:
   // de momento voy a forzar una sangria aqui s tnego tiempo mirare una manera mejor xd
}

void Comanda::guardaTecladonum(vector<NumeroButton*> n)
{
	tecladonum = n;
}

void Comanda::guardaTecladotam(vector<TamanoButton*> t)
{
	tecladotam = t;
}

void Comanda::guardaBoton(UiButton* b)
{
	botones.push_back(b);
}

void Comanda::toggleTeclado(bool b)
{
	for (auto t : teclado)
	{
		t->setActive(b);
	}
}

void Comanda::toggleTecladonum(bool b)
{
	for (auto t : tecladonum)
	{
		t->setActive(b);
	}
}

void Comanda::toggleTecaldotam(bool b)
{
	for (auto t : tecladotam)
	{
		t->setActive(b);
	}
}

void Comanda::aceptaPaella()
{
	//aqui esta lo dificil el vector de la paella que envias ya no lo podras editar pero deberia seguir siendo visible 
	//, tendra que mover margenes y vaciar el vector de pedido y que haya un render paellas , lo dificil va  a ser que 
	//se renderice otdo guay
	if (!Pedido.empty())
	{
		paellas.push_back(vector<UiButton*>());
		for (int j = 0; j < Pedido.size(); j++)
		{
			paellas[numeroPaellas].push_back(Pedido[j]);
			//  string s = Pedido[j]->getTextura();

			  //paellas[numeroPaellas][j].push_back(*s.c_str()); //el vector qeuire chars raros por algun motivo
			  ///paellas[numeroPaellas].push_back(s);
			  //esta explotando ahi por algun motivo//el motivo : no se leer
		}
		Pedido.erase(Pedido.begin(), Pedido.begin() + Pedido.size());
		Pedido.clear();
		numeroPaellas++;
	}
	//sangriado

	escritoY += anchobotones / 2 + margenbotones;
	escritoX = getPosition().getX() / 2 + margenbotones + anchobotones / 2;
	alto = alto + anchobotones / 2 + 2 * margenbotones;
	setDimension(ancho, alto);
	setPosition(getPosition().getX(), getPosition().getY() + 2 * margenbotones);
	vector<Point2D<double>> sangria = uiManager->getPosTeclado();
	for (int i = 0; i < sangria.size(); i++)
	{
		int ny = sangria[i].getY() + anchobotones * 0.7f;
		sangria[i].setY(ny);
		//en algun lugar vuelven a tener el valor default lo tengo que mirar
		//bajar teclado
		//lo bajará en uim?
	}
	uiManager->setPosTeclado(sangria);
	toggleTeclado(false);
	toggleTecaldotam(true);
}

void Comanda::enviaComanda()
{
	uiManager->getBarra()->AñadeComanda(this);
	cancelaPedido();
	toggleTecaldotam(false);
	toggleTeclado(false);
	toggleTecladonum(true);

}
void Comanda::renderizaPaellas()
{

	if (numeroPaellas > 0)
	{
		for (int i = 0; i < numeroPaellas; i++)
		{

			for (int j = 0; j < paellas[i].size(); j++)
			{
				paellas[i][j]->render(nullptr);//los strings de las texturas no llegan y se muere
			}
		}
	}

	if (eliminarboton != nullptr)
	{
		eliminarboton->render(nullptr);
	}
}
void Comanda::desplazacomandas(int d)
{

	if (numeroPaellas > 0)
	{
		for (int i = 0; i < numeroPaellas; i++)
		{

			for (int j = 0; j < paellas[i].size(); j++)
			{
				paellas[i][j]->setPosition(paellas[i][j]->getPosition().getX() + d, paellas[i][j]->getPosition().getY());
			}
		}



	}
	if (numeromesa != nullptr)
	{
		numeromesa->setPosition(numeromesa->getPosition().getX() + d, numeromesa->getPosition().getY());
	}


}
void Comanda::clearPaellas()
{
	if (numeroPaellas > 0)
	{
		for (int i = 0; i < numeroPaellas; i++)
		{

			for (int j = 0; j < paellas[i].size(); j++)
			{
				delete paellas[i][j]; paellas[i][j] = nullptr;
			}
			paellas[i].clear();
		}

		paellas.clear();

	}
	numeroPaellas = 0;
}
vector<vector<UiButton*>> Comanda::copyPaellas()
{
	vector<vector <UiButton*>> paellascopiadas;
	for (int i = 0; i < paellas.size(); i++)
	{
		paellascopiadas.push_back(vector<UiButton*>());

		for (int j = 0; j < paellas[i].size(); j++)
		{
			UiButton u = *paellas[i][j];
			UiButton* up = new UiButton(game, u.getTextura(), u.getPosition().getX(), u.getPosition().getY(), u.getWidth(), u.getHeight());
			paellascopiadas[i].push_back(up);
		}
	}

	return paellascopiadas;
}

void Comanda::setSitio(list<Comanda*>::iterator  s)
{
	sitiolista = s;
}

list<Comanda*>::iterator  Comanda::getSitio()
{
	return sitiolista;
}

void Comanda::toggleactive()
{
	setActive(!isActive());

	for (auto b : botones)
	{
		b->setActive(!b->isActive());
	}
	if (isActive())//activando comanda
	{
		toggleTecladonum(isActive());
	}
	else//desactivando comanda
	{
		toggleTecaldotam(isActive());
		toggleTeclado(isActive());
		toggleTecladonum(isActive());
	}

	cancelaPedido();
}

bool Comanda::OnClick(int mx, int my)
{
	SDL_Rect z = getCollider();
	SDL_Rect d = { mx,my,1,1 };
	if (SDL_HasIntersection(&z, &d))
	{
		uiManager->getBarra()->seleccionaComanda(seleccionaComanda());
		return true;
	}
	else
		return false;
}

Comanda* Comanda::seleccionaComanda()
{
	setTexture("cuadernilloseleted");
	return this;
}

void Comanda::deseleccionaComanda()
{
	setTexture("cuadernillo");
}