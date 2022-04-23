#include "Comanda.h"
#include "../GameObjects/UI/Ingredientebutton.h"
#include "../GameObjects/UI/Numerobutton.h"
#include "../GameObjects/UI/Tamanobutton.h"
#include  "../GameObjects/UI/EliminaComandaButton.h"
#include "../Scenes/Tutorial.h"
#include "../Control/Game.h"
#include "../Control/ObjectManager.h"
#include "../GameObjects/UI/UIManager.h"
#include "ListaComandas.h"
#include "../sdlutils/InputHandler.h"
template <class T >
bool comparaX(T u1, T u2)
{
	return u1.getPosition().getX() < u2.getPosition().getX();
}
template <class T >
bool comparaY(T u1,T u2)
{
	return  u1.getPosition().getY() < u2.getPosition().getY();
}

Comanda::Comanda(Game* game, uint escala, UIManager* uim) :GameObject(game)
{
	setTexture("cuadernillo");
	escale = escala;
	Vector2D<double> p;
	ancho *= escale;
	alto *= escale;
	margenbotones *= escale;
	anchobotones *= escale;
	margenbotones *= escale;
	margenizquierdo *= escale;
	margensuperior *= escale;
	x = 100;
	y = 110;
//	x += ancho /2*escale;
	//y += alto / 4*escale;
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
	margensuperior = iy-anchobotones/2;
	escritoX = ix;
	//margenizquierdo= getPosition().getX() / 2 + anchobotones / 1*escale;
	//escritoX = getPosition().getX() / 2 + anchobotones / 1*escale;
	escritoY = margensuperior;
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
	
	if (Pedido.size() < maxingrendientes+1&&paellas.size()<maxpaellas)

	{
		UiButton* a = new UiButton(game, i, escritoX, escritoY, anchobotones / 2, anchobotones / 2);
		escritoX += anchobotones / 2 + margenbotones;
		//gamet->getObjectManager()->creaTeclado(a);
		Pedido.push_back(a);
		if (Pedido.size() % 4 == 0)
		{
			escritoY += anchobotones / 2 + margenbotones;
			escritoX = getPosition().getX() / 2 + anchobotones / 6;
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
			for (int i = 0; i < tecladotam.size(); i++)
			{
				//b->getPosition().setY(b->getPosition().getY()+anchobotones*0.7);
				int ny = tecladotam[i]->getY() + anchobotones * 0.5f;

				Point2D<double> np = tecladotam[i]->getPosition();
				np.setY(ny);
				tecladotam[i]->setPosition(np);
			}
			uiManager->setPosTeclado(sangria);
		}
		randomizaIconos();
	}
}
void Comanda::anadirNumeromesa(string n)
{
	numeromesa = new UiButton(game, n, x + anchobotones, anchobotones, anchobotones / 2, anchobotones / 2);
	toggleTecladonum(false);
	toggleTecaldotam(true);
	activeTeclado = tecladotam;
	chageActiveTeclado();
	
}
UiButton* Comanda::getNumeromesa()
{
	return numeromesa;
}
bool Comanda::comparaX(UiButton u1, UiButton u2)
{
	return (u1.getPosition().getX() < u2.getPosition().getX());
}
bool Comanda::comparaY(UiButton u1, UiButton u2)
{
	return  (u1.getPosition().getY() < u2.getPosition().getY());
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
	//pero lo qeuiro ordenado para el foco
	sort(teclado.begin(), teclado.end(), [](UiButton* u1, UiButton* u2) {return  u1->getPosition().getX() < u2->getPosition().getX(); });
	sort(teclado.begin(), teclado.end(), [](UiButton* u1, UiButton* u2) {return  u1->getPosition().getY() < u2->getPosition().getY(); });
		
	//sort(teclado.begin(), teclado.end(), &comparaY);
	//sort(teclado.begin(), teclado.end(), &comparaX);

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
	for (int i = 0; i < tecladotam.size(); i++)
	{
		//b->getPosition().setY(b->getPosition().getY()+anchobotones*0.7);
		

		tecladotam[i]->setPosition(postecladoini[i]);
	}
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
	randomizaIconos();
	//pero al inicial le falta la primera sangria y queda por encima de la primera linea de pedido D:
   // de momento voy a forzar una sangria aqui s tnego tiempo mirare una manera mejor xd
}

void Comanda::guardaTecladonum(vector<UiButton*> n)
{
	tecladonum = n;
}

void Comanda::guardaTecladotam(vector<UiButton*> t)
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
	if (!tecladonum[0]->isActive()&&paellas.size()<maxpaellas&&Pedido.size()>0)
	{
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
		for (int i = 0; i < tecladotam.size(); i++)
		{
			//b->getPosition().setY(b->getPosition().getY()+anchobotones*0.7);
			int ny = tecladotam[i]->getY() + anchobotones * 0.7f;

			Point2D<double> np = tecladotam[i]->getPosition();
			np.setY(ny);
			tecladotam[i]->setPosition(np);
		}
		uiManager->setPosTeclado(sangria);
		toggleTeclado(false);
		toggleTecaldotam(true);
	}
}

void Comanda::enviaComanda()
{
	if (dynamic_cast<Tutorial*>(game->getCurrentScene()) && game->getCurrentScene()->getState()==apuntaPedido) {

		if (numeroPaellas == 1 && paellas[0].size()==4 && paellas[0][0]->getTextura() == "small") {
			if (paellas[0][1]->getTextura() == "alcachofa") {
				if (paellas[0][2]->getTextura() == "calamar") {
					if (paellas[0][3]->getTextura() == "cangrejo") {
						eC();
						game->getCurrentScene()->changeState(States::pausaPedido);
					}
				}
				else if (paellas[0][2]->getTextura() == "cangrejo") {
					if (paellas[0][3]->getTextura() == "calamar") {
						eC();
						game->getCurrentScene()->changeState(States::pausaPedido);
					}
				}
			}
			else if (paellas[0][1]->getTextura() == "calamar") {
				if (paellas[0][2]->getTextura() == "alcachofa") {
					if (paellas[0][3]->getTextura() == "cangrejo") {
						eC();
						game->getCurrentScene()->changeState(States::pausaPedido);
					}
				}
				else if (paellas[0][2]->getTextura() == "cangrejo") {
					if (paellas[0][3]->getTextura() == "alcachofa") {
						eC();
						game->getCurrentScene()->changeState(States::pausaPedido);
					}
				}
			}
			else if (paellas[0][1]->getTextura() == "cangrejo") {
				if (paellas[0][2]->getTextura() == "calamar") {
					if (paellas[0][3]->getTextura() == "alcachofa") {
						eC();
						game->getCurrentScene()->changeState(States::pausaPedido);
					}
				}
				else if (paellas[0][2]->getTextura() == "alcachofa") {
					if (paellas[0][3]->getTextura() == "calamar") {
						eC();
						game->getCurrentScene()->changeState(States::pausaPedido);
					}
				}
			}
			else game->getCurrentScene()->changeState(States::pausaComandaEquivocada);
		}
		else game->getCurrentScene()->changeState(States::pausaComandaEquivocada);
	}
	else {
		eC();
	}
}
void Comanda::eC()
{
	uiManager->getBarra()->AñadeComanda(this);
	cancelaPedido();
	toggleTecaldotam(false);
	toggleTeclado(false);
	toggleTecladonum(true);
	toggleactive();
	//uiManager->getBarra()->setBarraActive(false);
	//uiManager->getBarra()->toggleBarra();

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
		focusedzone = 0;
		indexfocus = 0;
		//tecladonum[indexfocus]->setfocused();
		activeTeclado = tecladonum;
		focusedbutton = activeTeclado[indexfocus];
		activeTeclado[indexfocus]->setfocused();
		
		uiManager->getBarra()->setBarraActive(true);
			uiManager->getBarra()->toggleBarra();

	



	}
	else//desactivando comanda
	{
		toggleTecaldotam(isActive());
		toggleTeclado(isActive());
		toggleTecladonum(isActive());
		if(focusedbutton!=nullptr)
		focusedbutton->setunfocused();
		focusedbutton = nullptr;
		focusedzone = -1;
		indexfocus = -1;
	}

	cancelaPedido();
	//cout << "togleando active"; 
}
bool Comanda::onClick(int mx, int my, bool& exit)
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
	eliminarboton->setActive(true);
	return this;
}

void Comanda::deseleccionaComanda()
{
	setTexture("cuadernillo");
	eliminarboton->setActive(false);
}
void Comanda::pressSelectedButton()
{
	if (focusedbutton != nullptr)
	{
		bool b= false;
		
		setDimension(ancho, alto);
		focusedbutton->execute(b);
	}
}
void Comanda::cambiazonafoco()
{
	if (focusedzone == 0)//cambio del teclado a la ui comandas
	{
		activeTeclado = botones;
		chageActiveTeclado();
		focusedzone = 1;
	}
	else
	{
		//ver que teclado esta activo  y focusearlo
		if (teclado[0]->isActive())
		{
			activeTeclado = teclado;
			chageActiveTeclado();
		}
		else if (tecladonum[0]->isActive())
		{
			activeTeclado = tecladonum;
			chageActiveTeclado();

		}
		else if (tecladotam[0]->isActive())
		{
			activeTeclado = tecladotam;
			chageActiveTeclado();

		}
		focusedzone = 0;
	}
}
void Comanda::siguientebotonfocus(int dir)
{
	if (!activeTeclado.empty())
	{
		activeTeclado[indexfocus]->setunfocused();
		if (indexfocus == activeTeclado.size() - 1)
		{
			if (dir > 0)
			{
				indexfocus = 0;
			}
			else indexfocus--;
		}
		else if (indexfocus == 0)
		{
			if (dir < 0)
			{
				indexfocus = activeTeclado.size() - 1;
			}
			else
			{
				indexfocus++;
			}
		}
		else
		{
			indexfocus += dir;
		}
		focusedbutton = activeTeclado[indexfocus];
		activeTeclado[indexfocus]->setfocused();
	}
/*	if (!activeTeclado.empty())
	{
		activeTeclado[indexfocus]->setunfocused();
		indexfocus += dir;
		if (indexfocus < activeTeclado.size()&&indexfocus>=0)
		{
			//indice valido todo guay
		}
		 else if (indexfocus < 0)
		{
			indexfocus = activeTeclado.size() - 1;
		}
		else indexfocus = 0;
		focusedbutton = activeTeclado[indexfocus];
		activeTeclado[indexfocus]->setfocused();
	}*/
}
void Comanda::update()
{
	if (isActive())
	{
		if (ih().getKey(ih().RIGHT))
		{
		
			siguientebotonfocus(1);
		}
		if (ih().getKey(ih().LEFT))
		{
			siguientebotonfocus(-1);
		}
		if (ih().getKey(ih().A))
		{
			pressSelectedButton();
			
		}
		if (ih().getKey(ih().LB))
		{
			cambiazonafoco();
		}

	}
}
void Comanda::chageActiveTeclado()
{
	focusedbutton->setunfocused();
	activeTeclado[0]->setfocused();
	focusedbutton = activeTeclado[0];
}
void Comanda::setActiveTeclado(vector<UiButton*> a)
{
	activeTeclado = a;
	chageActiveTeclado();
}
