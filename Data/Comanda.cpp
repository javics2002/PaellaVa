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

Comanda::Comanda(Game* game, uint escala, UIManager* uim,bool enVentanilla_) :GameObject(game)
{
	setTexture("cuadernillo");

	clip.x = 0;
	clip.y = 75;
	clip.w = sdlutils().images().at("cuadernillo").width() / 8;
	clip.h = sdlutils().images().at("cuadernillo").height() ;

	frameCounter = 0;
	lastFrameTime = sdlutils().currRealTime();
	frameRate = 1000 / 24;


	escale = escala;
	Vector2D<double> p;
	ancho *= escale;
	alto *= escale;
	w = 187;
	h = 291;
	margenbotones *= escale;
	anchobotones *= escale;
	margenbotones *= escale;
	margenizquierdo *= escale;
	margensuperior *= escale;
	x = 100;//30
	y = 110;//0
	tx = 10;

	p.setX(x);
	p.setY(y);
	setPosition(p);
	
	setDimension(ancho, alto);
	altoini = alto;
	iniy = p.getY();
	uiManager = uim;
	
	double ix = p.getX() / 2 + margenbotones + anchobotones / 2;
	double iy = p.getY() / 2 + 2 * anchobotones;
	margenizquierdo = ix;
	margensuperior = iy-anchobotones/2;
	escritoX = ix;
	
	escritoY = margensuperior;
	
}
Comanda::Comanda(Comanda& c) : GameObject(c.game)
{
	clip.x = 0;
	clip.y = 0;
	clip.w = sdlutils().images().at("cuadernilloseleted").width();
	clip.h = sdlutils().images().at("cuadernilloseleted").height();
	y = c.y;
	x = c.x;
	h = c.h;
	w = c.w*0.7;
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
Comanda::Comanda(Game* game, int numMesa, vector<int> tamPaellas, vector<int> ingPedidos) : GameObject(game)
{
	//crear una comanda normal y rellenar el pedido con los numeritos
	setTexture("cuadernillo");
	clip.x = 0;
	clip.y = 75;
	clip.w = sdlutils().images().at("cuadernillo").width() / 8;
	clip.h = sdlutils().images().at("cuadernillo").height();
	frameCounter = 0;
	lastFrameTime = sdlutils().currRealTime();
	frameRate = 1000 / 24;
	escale = 1;
	Vector2D<double> p;
	ancho *= escale;
	alto *= escale;
	w = 187;
	h = 291;
	margenbotones *= escale;
	anchobotones *= escale;
	margenbotones *= escale;
	margenizquierdo *= escale;
	margensuperior *= escale;
	x = 100;//30
	y = 110;//0
	tx = 10;
	p.setX(x);
	p.setY(y);
	setPosition(p);
	setDimension(ancho, alto);
	altoini = alto;
	iniy = p.getY();
	uiManager = game->getUIManager();
	double ix = p.getX() / 2 + margenbotones + anchobotones / 2;
	double iy = p.getY() / 2 + 2 * anchobotones;
	margenizquierdo = ix;
	margensuperior = iy - anchobotones / 2;
	escritoX = ix;
	escritoY = margensuperior;
	//int aux = 0;
	int j = 0;
	for (int i = 0; i < tamPaellas.size(); i++) 
	{
		añadiraPedido(uiManager->getTamanosTrxtures()[tamPaellas[i]], 0);
		
		for (j = maxingrendientes*i; j <  maxingrendientes*(i+1); j++) {
			if (ingPedidos[j] != 9)
			{
				añadiraPedido(uiManager->getIngredientesTextures()[ingPedidos[j]], 0);
			}
		}
		aceptaPaella();
		escritoY += anchobotones / 2 + margenbotones;
		escritoX = getPosition().getX() / 2 + margenbotones + anchobotones / 2;
		alto = alto + anchobotones / 2 + 2 * margenbotones;
		h += anchobotones / 2 + 2 * margenbotones;
		setDimension(ancho, alto);
		setPosition(getPosition().getX(), getPosition().getY() + 2 * margenbotones);
	}
	if(numMesa>=0&&numMesa<=8)
	anadirNumeromesa(uiManager->getNumerosTextures()[numMesa],0);
	

}

Comanda::~Comanda()
{

	if (eliminarboton != nullptr)
		delete eliminarboton; eliminarboton = nullptr;
}
void Comanda::añadiraPedido(string i, int j)
{
	
	if (Pedido.size() < maxingrendientes+1 && paellas.size()<maxpaellas)
	{
		if (!tecladotam.empty() && !teclado.empty()) {
			if (tecladotam[0]->isActive())//se añade un tamaño
			{
				tamanosweb.push_back(j);
			}
			else if (teclado[0]->isActive())//añadimos un ingrediente
			{
				ingredientesweb.push_back(j);
			}
		}
		

		UiButton* a = new UiButton(game, i, escritoX, escritoY, anchobotones*0.75, anchobotones*0.75);
		escritoX += anchobotones / 2 + margenbotones;
		
		Pedido.push_back(a);
		if (Pedido.size() % 4 == 0)
		{
			escritoY += anchobotones / 2 + margenbotones;
			escritoX = getPosition().getX() / 2 + anchobotones / 6;
			alto = alto + anchobotones / 2 + 2 * margenbotones;
			h+= +anchobotones / 2 + 2 * margenbotones;
			setDimension(ancho, alto);
			setPosition(getPosition().getX(), getPosition().getY() + 2 * margenbotones);
		
			vector<Point2D<double>> sangria = uiManager->getPosTeclado();
			for (int i = 0; i < sangria.size(); i++)
			{
				int ny = sangria[i].getY() + anchobotones * 0.7f;
				sangria[i].setY(ny);
				
			}
			for (int i = 0; i < tecladotam.size(); i++)
			{
				
				int ny = tecladotam[i]->getY() + anchobotones * 0.5f;

				Point2D<double> np = tecladotam[i]->getPosition();
				np.setY(ny);
				tecladotam[i]->setPosition(np);
			}
			uiManager->setPosTeclado(sangria);
		}
		if(uiManager->getPosTeclado().size() > 0)
			randomizaIconos();
	}
}
void Comanda::anadirNumeromesa(string n,int j)
{
	ih().setKey(false, InputHandler::A);
	numeromesa = new UiButton(game, n, x + anchobotones, anchobotones, anchobotones / 2, anchobotones / 2);
	toggleTecladonum(false);
	toggleTecaldotam(true);
	activeTeclado = tecladotam;
	changeActiveTeclado();
	numeromesaweb = j;
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


	setActiveTeclado(teclado);


	
		


}
void Comanda::dibujaPedido()
{
	if (numeromesa != nullptr)
		numeromesa->render(nullptr);

	for (auto i : Pedido)
		i->render(nullptr);

	renderizaPaellas();
}
void Comanda::borraPedido()
{
	bool saltolinea = Pedido.size() % 4 == 0;//por si acaso el metodo que escribe ya hace saltos de liena se pueden anular en algunos casos
	//pedido guarda la paella actual si size es 1 estoy borrando un tamanio y no un ingrediente
	if (Pedido.size() > 0)
	{
		if (Pedido.size() == 1)//borro tamanio
		{
			tamanosweb.pop_back();
		}
		else//borro ingrediente
		{
			ingredientesweb.pop_back();
		}

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
	h= sdlutils().images().at("cuadernillo").height();
	setDimension(ancho, altoini);
	setPosition(getPosition().getX(), iniy);
	uiManager->setPosTeclado(postecladoini);
	uiManager->randomizaTeclado();
	for (int i = 0; i < tecladotam.size(); i++)
	{
		
		

		tecladotam[i]->setPosition(postecladoini[i]);
	}
	clearPaellas();
	numeroPaellas = 0;
	delete numeromesa; numeromesa = nullptr;
	toggleTecaldotam(false);
	toggleTeclado(false);
	numeromesaweb = -1;
	tamanosweb.clear();
	ingredientesweb.clear();
	if (isActive())
	{
		toggleTecladonum(true);
		uiManager->getRedactabutton()->resetPosition();
	}
	ih().setKey(false, InputHandler::A);
}

void Comanda::guardaTeclado()
{
	vector<Point2D<double>> sangria = uiManager->getPosTeclado();
	for (int i = 0; i < sangria.size(); i++)
	{
		int ny = sangria[i].getY() + anchobotones * 0.7f;
		sangria[i].setY(ny);
		
	}
	
	postecladoini = sangria;
	teclado = uiManager->getTeclado();
	
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
	//se renderice todo guay
	//todos los pedidos con 3 ingredientes y si no pues se rellena con las (un 9)
	
	if (ingredientesweb.size() == maxingrendientes * tamanosweb.size())
	{
		//paellas llenas y sin huecos
	}
	else
	{
		//calculamos cuantos huecos hay y los rellenamos 
		int huecos = maxingrendientes * tamanosweb.size() - ingredientesweb.size();
		for (int i = 0; i < huecos; i++)
		{
			ingredientesweb.push_back(9);
		}
	}

	
		
	if (!Pedido.empty() && paellas.size() < maxpaellas)
	{
		uiManager->getRedactabutton()->goDown();
		paellas.push_back(vector<UiButton*>());
		for (int j = 0; j < Pedido.size(); j++)
		{
			paellas[numeroPaellas].push_back(Pedido[j]);
			
		}
		Pedido.erase(Pedido.begin(), Pedido.begin() + Pedido.size());
		Pedido.clear();
		numeroPaellas++;

		if (tecladonum.size() > 0) {

			//sangriado
			if (!tecladonum[0]->isActive())
			{
				escritoY += anchobotones / 2 + margenbotones;
				escritoX = getPosition().getX() / 2 + margenbotones + anchobotones / 2;
				alto = alto + anchobotones / 2 + 2 * margenbotones;
				h += anchobotones / 2 + 2 * margenbotones;
				setDimension(ancho, alto);
				setPosition(getPosition().getX(), getPosition().getY() + 2 * margenbotones);
				
				vector<Point2D<double>> sangria = uiManager->getPosTeclado();
				for (int i = 0; i < sangria.size(); i++)
				{
					int ny = sangria[i].getY() + anchobotones * 0.7f;
					sangria[i].setY(ny);
					
				}
				for (int i = 0; i < tecladotam.size(); i++)
				{
					
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
	
	aceptaPaella();
	uiManager->getBarra()->AñadeComanda(this);
	game->getNetworkManager()->syncComanda(getNumeroMesaWeb(), getTamanosWeb(), getIngredientesWeb());
	cancelaPedido();
	toggleTecaldotam(false);
	toggleTeclado(false);
	toggleTecladonum(true);
	toggleactive();
	


}
void Comanda::renderizaPaellas()
{
	if (numeroPaellas > 0)
	{
		for (int i = 0; i < numeroPaellas; i++)
		{

			for (int j = 0; j < paellas[i].size(); j++)
			{
				paellas[i][j]->render(nullptr);
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
	uiManager->getRedactabutton()->Swichposition();
	
	if(!isActive()) {
		setActive(true);
		for (auto b : botones)
		{
			b->setActive(false);
		}

		toggleTecaldotam(false);
		toggleTeclado(false);
		toggleTecladonum(false);
		uiManager->addTween(-56.0f, -200.0f, 500.0f, true).via(easing::quadraticOut).onStep(
			[this](tweeny::tween<float>& t, float) mutable
			{
				y = t.peek();
				if (t.progress() == 1.0f)
				{  
					sdlutils().soundEffects().at("guardarComandas").play(0, game->UI);

					setActive(false);
					cancelaPedido();
					if (focusedbutton != nullptr)
						focusedbutton->setunfocused();
					focusedbutton = nullptr;
					focusedzone = -1;
					indexfocus = -1;
					return true;
				}
				return false;
			});;
	}
	else //activando comanda
	{
		uiManager->addTween(-200.0f, -56.0f, 500.0f, true).via(easing::quadraticOut).onStep(
			[ this](tweeny::tween<float>& t, float) mutable {
				
					y=t.peek();

				if (t.progress() == 1.0f && isActive()) {
					
					sdlutils().soundEffects().at("sacarComandas").play(0, game->UI);
					for (auto b : botones)
					{
						b->setActive(true);
					}

					toggleTecladonum(true);
					cancelaPedido();
					focusedzone = 0;
					indexfocus = 0;
					
					activeTeclado = tecladonum;
					focusedbutton = activeTeclado[indexfocus];
					activeTeclado[indexfocus]->setfocused();

					uiManager->getBarra()->setBarraActive(true);
					uiManager->getBarra()->toggleBarra();
					return true;
				}
				return false;
			});;
		
	}
	
	
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
	clip.x = 0;
	clip.y = 0;
	clip.w = sdlutils().images().at("cuadernilloseleted").width();
	clip.h = sdlutils().images().at("cuadernilloseleted").height();
	setTexture("cuadernilloseleted");
	eliminarboton->setActive(true);
	return this;
}

void Comanda::deseleccionaComanda()
{
	clip.x =36;
	clip.y = 75;
	clip.w = sdlutils().images().at("cuadernillo").width() /8 - clip.x*1.3;
	setTexture("cuadernillo");
	eliminarboton->setActive(false);
}
void Comanda::pressSelectedButton()
{
	if (focusedbutton != nullptr)
	{
		bool b= false;
		
		
		focusedbutton->execute(b);

	}
}
void Comanda::cambiazonafoco()
{
	if (focusedzone == 0)//cambio del teclado a la ui comandas
	{
		activeTeclado = botones;
		changeActiveTeclado();
		focusedzone = 1;
	}
	else
	{
		//ver que teclado esta activo  y focusearlo
		if (teclado[0]->isActive())
		{
			activeTeclado = teclado;
			changeActiveTeclado();
		}
		else if (tecladonum[0]->isActive())
		{
			activeTeclado = tecladonum;
			changeActiveTeclado();

		}
		else if (tecladotam[0]->isActive())
		{
			activeTeclado = tecladotam;
			changeActiveTeclado();

		}
		focusedzone = 0;
	}
}
void Comanda::siguientebotonfocus(int dir)
{
	if (!activeTeclado.empty())
	{
		if (indexfocus < activeTeclado.size()) 
			activeTeclado[indexfocus]->setunfocused();
		
		if (indexfocus == activeTeclado.size() - 1)
		{
			if (dir > 0)
				indexfocus = 0;
			else indexfocus--;
		}
		else if (indexfocus == 0){
			if (dir < 0)
				indexfocus = activeTeclado.size() - 1;
			else indexfocus++;
		}
		else indexfocus += dir;

		if (indexfocus <= activeTeclado.size()) {
			focusedbutton = activeTeclado[indexfocus];
			activeTeclado[indexfocus]->setfocused();
		}
		else {
			indexfocus = 0;
			focusedbutton = activeTeclado[indexfocus];
			activeTeclado[indexfocus]->setfocused();
		}
	}

}
void Comanda::update()
{
	if (isActive())
	{
		if (ih().getKey(ih().FOCUSRIGHT))
		{
		
			siguientebotonfocus(1);
		}
		if (ih().getKey(ih().FOCUSLEFT))
		{
			siguientebotonfocus(-1);
		}
		if (ih().getKey(ih().X))
		{
			ih().setKey(false, ih().X);
			pressSelectedButton();
			
		}
		if (ih().getKey(ih().LB) || ih().getKey(ih().TAB))
		{
			cambiazonafoco();
		}

		if (sdlutils().currRealTime() - lastFrameTime > frameRate && isActive())
			animUpdate();

	}
}

void Comanda::render(SDL_Rect* cameraRect)
{
	SDL_Rect dest = { tx , y , w, h};
	drawRender(cameraRect, dest, texture, clip);
}

void Comanda::animUpdate()
{
	lastFrameTime = sdlutils().currRealTime();

	if (animPlay) {
		clip.x = frameCounter * clip.w;
		frameCounter++;
	}

	if (frameCounter * clip.w > texture->width() - 10) {
		frameCounter = 0;
		animPlay = false;
	}

}

void Comanda::changeActiveTeclado()
{
	if(focusedbutton!=nullptr)focusedbutton->setunfocused();
	if (activeTeclado.size() > 0) {
		activeTeclado[0]->setfocused();
		focusedbutton = activeTeclado[0];
	}
	else toggleactive();
	indexfocus = 0;
}
void Comanda::setActiveTeclado(vector<UiButton*> a)
{
	activeTeclado = a;
	changeActiveTeclado();
}
void Comanda::setTexturecoords(int nx, int ny)
{
	tx = nx-w/2;
	y = ny-h/2;
}
int Comanda::getNumeroMesaWeb()
{
	return numeromesaweb;
}
vector<int> Comanda::getTamanosWeb()
{
	
	
	return tamanosweb;
}
vector<int> Comanda::getIngredientesWeb()
{
	
	
	vector<int> ing;
		int i = 0;
		int huecos = maxingrendientes * maxpaellas - ingredientesweb.size();//aqui last es 9
		for (int j = 0; j < huecos; j++)
		{
			ingredientesweb.push_back(9);
		}
		
	return ingredientesweb;
}