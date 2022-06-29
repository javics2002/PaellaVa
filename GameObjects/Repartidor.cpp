#include "Repartidor.h"

#include "../Control/Game.h"
#include "../Control/ObjectManager.h"

#include "../GameObjects/ColaTakeaway.h"
#include "../GameObjects/CajaTakeaway.h"
#include "../Data/PedidoTakeaway.h"

#include "../../Data/Pedido.h"

#include "../GameObjects/Muebles/Mesa.h"

#include "../Scenes/Jornada.h"

Repartidor::Repartidor(Game* mGame) : Mueble(mGame, pos, DIMENSION, DIMENSION, "repartidor"), pedido(nullptr), cola(nullptr), estado_(repCAMINANDO)
{
	setDimension(DIMENSION, DIMENSION);

	funcionando = true;

	texTolerancia = &sdlutils().images().at("barraTolerancia");
	tolerancia = 100;
	lastTimeTol = 0;

	clip.x = 0;
	clip.y = 0;
	clip.w = 2048;
	clip.h = 2048;

	showPed = false;
	orderStart = true;
	itemNow = 0;
	lastTimePed = 0;

	setDepth(1);
}

Repartidor::~Repartidor()
{
	sdlutils().soundEffects().at("conversacion2").haltChannel(canalSonido);
}

void Repartidor::init(ColaTakeaway* cola_)
{
	tolerancia = 100;

	posCola = cola_->getPos();
	cola = cola_;

	hacerPedido();

	setVel(vel);

	estado_ = repCAMINANDO;
}

void Repartidor::update()
{
	switch (estado_)
	{
		case repCAMINANDO:
		case repSALIENDO:
			setPosition(pos + vel);
			if (cajaTakeaway != nullptr) {
				cajaTakeaway->setPosition(getRectCenter(getOverlap()));
			}
			break;

		case repESPERANDO:

			for (auto i : mGame->getObjectManager()->getPool<Repartidor>(_p_REPARTIDOR)->getCollisions(getCollider())) {
				if (i != this && i->getState() == repCAMINANDO)
					i->colisionCartel();
			}

			if (!showPed)
				orderStart = true;

			bajaTolerancia();
			break;

		default:
			bajaTolerancia();
			break;
	}
}

void Repartidor::render(SDL_Rect* cameraRect)
{
	/*SDL_Rect dest = { getX() - getWidth() / 2, getY() - getHeight() / 2, w, h };
	drawRender(cameraRect, dest, );*/

	SDL_Rect dest = { getX() - getWidth() / 2, getY() - getHeight() / 2, w, h };
	if (isActive()) drawRender(cameraRect, dest, texture, clip, flip);

	int bocadilloX = 80;
	int bocadilloY = 80;

	

	if (ratonEncima(cameraRect)) {

		SDL_Rect bocadillo = {};
		SDL_Rect emoticono = {};

		int emoticonoX = 35;
		int emoticonoY = 30;

		bocadillo = { (int)getOverlap().x - bocadilloX / 2, (int)getOverlap().y - bocadilloY / 3, bocadilloX, bocadilloY };
		emoticono = { (int)getOverlap().x - emoticonoX / 2, (int)getOverlap().y - bocadilloY / 3 + emoticonoY / 2, emoticonoX, emoticonoY };

		drawRender(cameraRect, bocadillo, texTolerancia);
		drawRender(cameraRect, emoticono, &sdlutils().images().at(texturaTolerancia[((int)tolerancia / 20)]));

		if (estado_ == repESPERANDO) {
			if (sdlutils().virtualTimer().currTime() - lastTimePed >= PED_DELAY) {
				lastTimePed = sdlutils().virtualTimer().currTime();

				itemNow = (itemNow + 1) % texPedido.size();
			}

			int itemDim = 45;


			drawRender(cameraRect, { (int)getX() - bocadilloX / 2, (int)getY() - bocadilloY, bocadilloX, bocadilloY }, texTolerancia);
			drawRender(cameraRect, { (int)getX() - itemDim / 2, (int)getY() - 70, itemDim, itemDim }, &sdlutils().images().at(texPedido[itemNow]));

			showPed = false;
		}
	}
}

bool Repartidor::colisionCartel()
{
	setState(repESPERANDO);

	return true;
}

bool Repartidor::ratonEncima(SDL_Rect* cameraRect)
{
	SDL_Rect rect = SDL_Rect();

	SDL_GetMouseState(&rect.x, &rect.y);
	rect = { rect.x + cameraRect->x,  rect.y + cameraRect->y, 5, 5 };

	return overlap(rect);
}

void Repartidor::bajaTolerancia()
{
	if (tolerancia > 0 && sdlutils().virtualTimer().currTime() - lastTimeTol >= DIMIN_TIME) {

		tolerancia -= DIMIN_TOLERANCIA;

		if (tolerancia < 0) tolerancia = 0;

		lastTimeTol = sdlutils().virtualTimer().currTime();
	}
}

void Repartidor::setState(EstadoRepartidor est)
{
	estado_ = est;


	switch (estado_)
	{
	case repESPERANDO:
		lastTimeTol = sdlutils().virtualTimer().currTime();
		animPara();
		break;
	case repCAMINANDO:
		animCamina();
		break;
	case repSALIENDO:
		if (getVel().getX() < 0) {
			// Flip, mirar a la izq
			flip = SDL_FLIP_HORIZONTAL;
		}
		cola->remove(posCola);
		animCamina();
		break;
	default:
		break;
	}
}

EstadoRepartidor Repartidor::getState()
{
	return estado_;
}

void Repartidor::onDeactivate()
{
	// mesa->clienteSeVa();
	mGame->getCurrentScene()->addPuntuaciones(pedido->puntuarPedido(cajaTakeaway) * ((tolerancia + 50) / 100));
	// setActive(false);
}

void Repartidor::hacerPedido()
{
	pedido = new PedidoTakeaway(mGame);

	texPedido = pedido->getPedidoTex();
}

void Repartidor::modificaPedido(vector<int> ingPedidos) {

	delete pedido; // borramos el anterior pedido
	pedido = new PedidoTakeaway(mGame, ingPedidos);

	texPedido = pedido->getPedidoTex();
}

void Repartidor::decirPedidio()
{
	if (estado_ == repESPERANDO) {
		if (orderStart) {
			lastTimePed = sdlutils().virtualTimer().currTime();
			itemNow = 0;
			orderStart = false;
		}
		showPed = true;
	}
}

bool Repartidor::receiveCajaTakeaway(CajaTakeaway* caja)
{
	if (caja == nullptr || !caja->isLlena())
		return false;
	else {
		cajaTakeaway = caja;
		setVel(Vector2D<double>(-vel.getX(), -vel.getY()));
		setState(repSALIENDO);
	}
}

void Repartidor::setOrientacion(bool vertical_)
{
	vertical = vertical_;
}

void Repartidor::animPara()
{
	// Sería para Anims
}

void Repartidor::animCamina()
{
	// Sería para Anims
}


