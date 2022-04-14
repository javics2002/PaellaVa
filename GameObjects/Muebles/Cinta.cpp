#include "Cinta.h"

Cinta::Cinta(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE,  TILE_SIZE, "cinta")
{
	//Este sonido es m�s bajo
	sdlutils().soundEffects().at("cinta").setVolume(2);
	canalSonido = sdlutils().soundEffects().at("cinta").play(-1);

	clip.x = 0;
	clip.y = 0;
	clip.w = 256;
	clip.h = 256;

	frameCounter = 0;
	lastFrameTime = sdlutils().currRealTime();
	frameRate = 1000 / 24;

	setTexture("cinta");

}

Cinta::~Cinta()
{
	sdlutils().soundEffects().at("cinta").haltChannel(canalSonido);
}

SDL_Rect Cinta::getCollider()
{
	if(isActive())return cintaCollider;
	return { 0,0,0,0 };
}

void Cinta::setCollider(SDL_Rect r)
{
	cintaCollider = r;
}


SDL_Rect Cinta::getOverlap()
{
	return SDL_Rect();
}

void Cinta::update()
{
	if (sdlutils().currRealTime() - lastFrameTime > frameRate && isActive())
		animUpdate();
}

void Cinta::render(SDL_Rect* cameraRect)
{
	if (isActive()) {
		SDL_Rect dest = { getX() - getWidth() / 2, getY() - getHeight() / 2, w, h };
		drawRender(cameraRect, dest, texture, clip);
	}
}

void Cinta::animUpdate()
{
	lastFrameTime = sdlutils().currRealTime();

	clip.x = frameCounter * clip.w;
	frameCounter++;

	if (frameCounter * clip.w > texture->width() - 10)
		frameCounter = 0;
}
