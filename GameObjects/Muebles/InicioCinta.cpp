#include "InicioCinta.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"
#include "../../Scenes/Tutorial.h"
#include "../../Control/NetworkManager.h"
#include "../../Utils/ParticleExample.h"
#include "../Herramienta.h"

InicioCinta::InicioCinta(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, TILE_SIZE, "inicioCinta")
{
	initTime = 0;
	funcionando = true;
}

void InicioCinta::update()
{
	if (!funcionando)
	{
		humo->setStyle(ParticleExample::SMOKE);
	}
	else 
	{
		humo->setStyle(ParticleExample::NONE);
	}

	humo->setPosition(getX(), getY());
	humo->update();

	if (!game->getNetworkManager()->isHost())
		return;

	int i = rand() % 1000;
	if (sdlutils().virtualTimer().currTime() - initTime >= SPAWN_DELAY && isActive() && funcionando)
	{
		if (i < porcentajeLetal && !dynamic_cast<Tutorial*>(game->getCurrentScene()))
		{
			IngredienteLetal* ingLetal = game->getObjectManager()->getPool<IngredienteLetal>(_p_INGREDIENTELETAL)->add(getPosition());
			ingLetal->setVel(vel);
			initTime = sdlutils().virtualTimer().currTime();

			game->getNetworkManager()->sendCreateIngredienteLetal(ingLetal->getTipo(), getPosition(), vel);
		}
		else {
			Ingrediente* ing = game->getObjectManager()->getPool<Ingrediente>(_p_INGREDIENTE)->add(getPosition());
			ing->setVel(vel);
			initTime = sdlutils().virtualTimer().currTime();

			ing->setId(idCont);
			idCont++;

			game->getNetworkManager()->sendCreateIngrediente(ing->getTipo(), ing->getId(), getPosition(), vel);
		}
	}

	if(!dynamic_cast<Tutorial*>(game->getCurrentScene())){
		if (funcionando && couldBreak <= 0)
		{
			testMueble();
			if (funcionando)
			{
				//se reduce cuando se podría romper
				couldBreak = MAX_BREAK_TIME - REDUCE_BREAK_TIME;
			}
			else
			{
				//se resetea cuando se podría romper
				couldBreak = MAX_BREAK_TIME;
				humo->setStyle(ParticleExample::EXPLOSION);
			}
		}
		else if (funcionando && couldBreak > 0)
		{
			couldBreak -= seg;
		}
	}

	
}

void InicioCinta::render(SDL_Rect* cameraRect)
{
	SDL_Rect dest = { getX() - getWidth() / 2, getY() - getHeight() / 2, getWidth(),
	   getHeight() };
	if (isActive()) {

		//if (funcionando)drawRender(cameraRect, dest, &sdlutils().images().at("inicioCinta"));
		//else drawRender(cameraRect, dest, &sdlutils().images().at("berenjena"));
		drawRender(cameraRect, dest, &sdlutils().images().at("inicioCinta"));

		humo->draw(cameraRect);
	}
}

SDL_Rect InicioCinta::getCollider()
{
	if(isActive())return getTexBox();
	return { 0,0,0,0 };
}

bool InicioCinta::resetCounter()
{
	couldBreak = MAX_BREAK_TIME;
	return true;
}

bool InicioCinta::receiveHerramienta(Herramienta* h)
{
	if (!funcionando)
	{
		if (dynamic_cast<Tutorial*>(game->getCurrentScene()) && game->getCurrentScene()->getState() == arreglarCinta)
			game->getCurrentScene()->changeState(pausaArreglarCinta);

		funcionando = true;
		h->setActive(false);
		return true;
	}
	return false;
}

SDL_Rect InicioCinta::getOverlap()
{
	return { int(getX() -getWidth()),
		int(getY() - getHeight()),
		(getWidth() *2),
		(getHeight() * 2) };
}
