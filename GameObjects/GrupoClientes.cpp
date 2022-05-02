#include "GrupoClientes.h"

#include "../Control/Game.h"
#include "../Control/ObjectManager.h"

#include "../GameObjects/Cola.h"

#include "../../Data/Pedido.h"

#include "../GameObjects/Muebles/Mesa.h"

#include "../Scenes/Jornada.h"

GrupoClientes::GrupoClientes(Game* game) : ObjetoPortable(game), pedido(nullptr), cola(nullptr), estado_(CAMINANDO) , nPaellas(0)
{
	setDimension(DIMENSION, DIMENSION);

	setTexture("berenjena");


	texTolerancia = &sdlutils().images().at("barraTolerancia");
	tolerancia = 100;	
	lastTimeTol = 0;

	showPed = false;
	orderStart = true;
	itemNow = 0;
	lastTimePed = 0;

	comidoMitad = false;
	lastTimeComido = 0;

	setDepth(3);
}

GrupoClientes::~GrupoClientes()
{
	sdlutils().soundEffects().at("conversacion2").haltChannel(canalSonido);
}

void GrupoClientes::initGrupo(Cola* cola_, vector<Cliente*> clientes_)
{
	tolerancia = 100;

	posCola = cola_->getPos();
	cola = cola_;
	
	clientes = clientes_;
	for (auto i : clientes)
		i->setVel(vel);

	estado_ = CAMINANDO;
}

void GrupoClientes::update()
{
	if (estado_ == CAMINANDO) {

		for (auto i : clientes) {
			i->update();
		}		
	}

	else if (estado_ == ENCOLA) {

		int n = clientes.size() - 1;

		SDL_Rect col;
		if (clientes[0]->getVel().getX() < 0)
			col = clientes[0]->getCollider();
		else col = clientes[n]->getCollider();

		SDL_Rect rect = { col.x - col.w / 2, col.y - col.h, col.w * (n + 2), col.h * (n + 2) };

		

		for (auto i : game->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->getCollisions(rect)) 
			i->colisionClientes();
		
		bajaTolerancia();
	}

	else if (estado_ == PIDIENDO) {

		if (!showPed)
			orderStart = true;

		bajaTolerancia();
	}

	else if (estado_ == COMIENDO) {

		if (!comidoMitad && sdlutils().virtualTimer().currTime() - lastTimeComido >= TIEMPO_COMIDA / 2) {
			mesa->comerPaellas();
			comidoMitad = true;
		}
		else if (sdlutils().virtualTimer().currTime() - lastTimeComido >= TIEMPO_COMIDA) {
			mesa->comerPaellas();
			estado_ = CUENTA;
			sdlutils().soundEffects().at("cuenta").play();
		}
	}

	else {
		bajaTolerancia();
	}
}

void GrupoClientes::render(SDL_Rect* cameraRect)
{
	if (isPicked()) {
		for (int i = 0; i < numIntegrantes(); i++) {
			SDL_Rect rect = pickedClientsPos[i];
			rect.x = rect.x + getPosition().getX() - getWidth() / 2;
			rect.y = rect.y + getPosition().getY() - getWidth() / 2;
			clientes[i]->drawPickedClient(cameraRect, rect);
		}
	}	

	int bocadilloX = 80;
	int bocadilloY = 80;

	if (showPed && estado_ == PIDIENDO) {
		if (sdlutils().virtualTimer().currTime() - lastTimePed >= PED_DELAY) {
			lastTimePed = sdlutils().virtualTimer().currTime();

			itemNow = (itemNow + 1) % texPedido.size();
		}

		int itemDim = 45;
		

		drawRender(cameraRect, { (int)getX() - bocadilloX / 2, (int)getY() - bocadilloY, bocadilloX, bocadilloY }, texTolerancia);
		drawRender(cameraRect, { (int)getX() - itemDim / 2, (int)getY() - 70, itemDim, itemDim }, &sdlutils().images().at(texPedido[itemNow]));

		showPed = false;
	}

	else if (!isPicked()) {
		if (ratonEncima(cameraRect)) {

			SDL_Rect bocadillo = {};
			SDL_Rect emoticono = {};

			int emoticonoX = 35;
			int emoticonoY = 30;

			if (estado_ < PIDIENDO) {
				bocadillo = { mitadGrupo() - bocadilloX / 2,
					(int)clientes[0]->getOverlap().y - bocadilloY, bocadilloX, bocadilloY };
				emoticono = { mitadGrupo() - emoticonoX / 2,
					(int)clientes[0]->getOverlap().y - bocadilloY + emoticonoY / 2, emoticonoX, emoticonoY };
			}
			else {
				bocadillo = { (int)mesa->getCenterMesa().getX() - bocadilloX / 2, (int)mesa->getCenterMesa().getY() - bocadilloY, bocadilloX, bocadilloY };
				emoticono = { (int)mesa->getCenterMesa().getX() - emoticonoX / 2, (int)mesa->getCenterMesa().getY() - bocadilloY + emoticonoY / 2, emoticonoX, emoticonoY };
			}

			drawRender(cameraRect, bocadillo, texTolerancia);		
			drawRender(cameraRect, emoticono, &sdlutils().images().at(texturaTolerancia[((int)tolerancia / 20)]));
		}

		else if (estado_ == CUENTA) {
			drawRender(cameraRect, { (int)mesa->getCenterMesa().getX() - bocadilloX / 2, (int)mesa->getCenterMesa().getY() - bocadilloY, bocadilloX, bocadilloY }, texTolerancia);
		}
	}
}

bool GrupoClientes::overlap(SDL_Rect rect)
{
	for (auto i : clientes) {
		if (i->overlap(rect)) {
			return true;
		}
	}

	return false;
}

bool GrupoClientes::collide(SDL_Rect rect)
{
	for (auto i : clientes) {
		if (i->collide(rect)) {
			return true;
		}
	}

	return false;
}

bool GrupoClientes::colisionClientes()
{
	if (estado_ == CAMINANDO) 
		setState(ENCOLA);

	return true;
}

bool GrupoClientes::ratonEncima(SDL_Rect* cameraRect)
{
	SDL_Rect rect = SDL_Rect();

	SDL_GetMouseState(&rect.x, &rect.y);

	rect = { rect.x + cameraRect->x,  rect.y + cameraRect->y, 5, 5 };

	return overlap(rect);
}

void GrupoClientes::bajaTolerancia()
{
	if (tolerancia > 0 && sdlutils().virtualTimer().currTime() - lastTimeTol >= DIMIN_TIME) {

		tolerancia -= DIMIN_TOLERANCIA;

		if (tolerancia < 0) tolerancia = 0;

		lastTimeTol = sdlutils().virtualTimer().currTime();
	}
}

void GrupoClientes::setState(EstadoClientes est)
{
	estado_ = est;

	lastTimeTol = sdlutils().virtualTimer().currTime();

	switch (estado_)
	{
	case ENCOLA:
		for (auto i : clientes) {
			i->clientePara();
		}
		break;
	case CAMINANDO:
		for (auto i : clientes) {
			i->clienteCamina();
		}
		break;
	default:
		break;
	}
}

EstadoClientes GrupoClientes::getState()
{
	return estado_;
}

int GrupoClientes::numIntegrantes()
{
	return clientes.size();
}

vector<Cliente*> GrupoClientes::getIntegrantes()
{
	return clientes;
}

void GrupoClientes::onObjectPicked()
{
	sdlutils().soundEffects().at("conversacion2").haltChannel(canalSonido);	

	if (estado_ == ENCOLA) {
		for (int i = 0; i < numIntegrantes(); i++) {
			int rX = 50;
			int rY = 30;
			pickedClientsPos[i] = { sdlutils().rand().nextInt() % rX - rX / 2, sdlutils().rand().nextInt() % rY - rY / 2,
					getWidth(), getHeight() };
		}
	}
	
	setGoshtGroup();
}

void GrupoClientes::onObjectDropped()
{
	if (estado_ == ENCOLA) {

		setState(PIDIENDO);

		setGoshtGroup();

		cola->remove(posCola, clientes.size());

		if (true /*&& clientes.size() >= 2*/) {
			string conversacion = "conversacion4";

			//Elegimos la conversacion en funcion del grupo de clientes
			if (clientes.size() >= 4)
				conversacion = clientes.size() >= 7 ? "conversacion7" : "conversacion4";

			canalSonido = sdlutils().soundEffects().at(conversacion).play(-1);
		}		
	}
}

bool GrupoClientes::canPick()
{
	return estado_ == ENCOLA || estado_==CUENTA;
}

bool GrupoClientes::canDrop() {

	return  estado_== ENCOLA;
}

void GrupoClientes::onDeactivate()
{
	for (auto i : clientes) {
		i->setActive(false);
	}
		
	if (estado_ == CUENTA) {
		mesa->clienteSeVa();
		game->getCurrentScene()->addPuntuaciones(pedido->puntuarPedido(mesa->getPaellasEntregadas()));
	}	
}

int GrupoClientes::mitadGrupo()
{
	int mitad = 0;

	for (auto i : clientes) {
		mitad += i->getX();
	}

	return mitad / clientes.size();
}

void GrupoClientes::hacerPedido(int tamMesa, Mesa* m)
{
	mesa = m;

	if (estado_ != CUENTA) {
		pedido = new Pedido(game, clientes.size(), tamMesa);

		texPedido = pedido->getPedidoTex();
	}
}

void GrupoClientes::modificaPedido(int numPaellas, vector<int> tamPaellas, vector<int> ingPedidos) {
	delete pedido; // borramos el anterior pedido

	pedido = new Pedido(game, numPaellas, tamPaellas, ingPedidos);

	texPedido = pedido->getPedidoTex();
}

void GrupoClientes::decirPedidio()
{
	if (estado_ == PIDIENDO) {
		if (orderStart) {
			lastTimePed = sdlutils().virtualTimer().currTime();
			itemNow = 0;
			orderStart = false;		
		}
		showPed = true;
	}		
}

void GrupoClientes::setOrientacion(bool vertical_)
{
	vertical = vertical_;
}

bool GrupoClientes::paellasPedidas() {
	if (estado_ == ESPERANDO || estado_ == PIDIENDO) {

		if (nPaellas + 1 <= pedido->getPedido().size()) {
			nPaellas++;
			if (nPaellas == pedido->getPedido().size()) {
				estado_ = COMIENDO;
				sdlutils().soundEffects().at("clienteCome").play();
				lastTimeComido = sdlutils().virtualTimer().currTime();
			}
			else estado_ = ESPERANDO;

			return true;
		}
	}

	return false;
}

void GrupoClientes::setGoshtGroup()
{
	for (auto i : clientes)
		i->setGoshtClient(isPicked());
}


