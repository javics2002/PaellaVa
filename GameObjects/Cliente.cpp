#include "Cliente.h"
#include "../../Control/game.h"

Cliente::Cliente(Game* game) : GameObject(game)
{
	setDimension(DIMENSION, DIMENSION);

	setDepth(1);

	clip.x = 0;
	clip.y = 0;
	clip.w = 256;
	clip.h = 256;

	frameCounter = sdlutils().rand().nextInt(0, 15);
	lastFrameTime = sdlutils().currRealTime();
	frameRate = 1000 / 50;

	currAnim = 0;
	setAnimResources(sdlutils().rand().nextInt(0, 3));
}

void Cliente::update()
{
	setPosition(pos+vel);

	animUpdate(Vector2D<double>(1, 0));
}

void Cliente::render(SDL_Rect* cameraRect)
{
	SDL_Rect dest = { getX() - getWidth() / 2, getY() - getHeight() / 2, w, h };
	drawRender(cameraRect, dest, anims[currAnim], clip, flip);
}

void Cliente::cambiaTextura(string textureN)
{
	setTexture(textureN);
}

void Cliente::setAnimResources(int clientType)
{
	if (clientType == 0) {

		anims.push_back(&sdlutils().images().at("cliente1WalkSide"));

		anims.push_back(&sdlutils().images().at("cliente1IdleDown"));
		anims.push_back(&sdlutils().images().at("cliente1IdleSide"));

		anims.push_back(&sdlutils().images().at("cliente1SittingDown"));
		anims.push_back(&sdlutils().images().at("cliente1SittingSide"));

	}
	else if (clientType == 1) {

		anims.push_back(&sdlutils().images().at("cliente2WalkSide"));

		anims.push_back(&sdlutils().images().at("cliente2IdleDown"));
		anims.push_back(&sdlutils().images().at("cliente2IdleSide"));

		anims.push_back(&sdlutils().images().at("cliente2SittingDown"));
		anims.push_back(&sdlutils().images().at("cliente2SittingSide"));

	}
	else if (clientType == 2) {

		anims.push_back(&sdlutils().images().at("cliente3WalkSide"));

		anims.push_back(&sdlutils().images().at("cliente3IdleDown"));
		anims.push_back(&sdlutils().images().at("cliente3IdleSide"));

		anims.push_back(&sdlutils().images().at("cliente3SittingDown"));
		anims.push_back(&sdlutils().images().at("cliente3SittingSide"));

	}
}

void Cliente::animUpdate(Vector2D<double> axis)
{
	// 0 Andando
	// 1 Idle Front
	// 3 Sitting Front

	if (sdlutils().currRealTime() - lastFrameTime > frameRate) {
	
		//bool para saber si hay que cambiar el flip
	bool flipH = false;
	if (flip == SDL_FLIP_HORIZONTAL) flipH = true;

	lastFrameTime = sdlutils().currRealTime();

	clip.x = frameCounter * clip.w;
	frameCounter++;

	if (frameCounter * clip.w > anims[currAnim]->width() - 10)
		frameCounter = 0;

	}


}

void Cliente::setAnim(int animNum)
{
	// 0 Andando
	// 1 Idle Front
	// 3 Sitting Front

	currAnim = animNum;
}

Texture* Cliente::getAnim()
{
	return anims[currAnim];
}

SDL_Rect Cliente::getClip()
{
	return clip;
}

SDL_RendererFlip Cliente::getFlip()
{
	return flip;
}

void Cliente::drawPickedClient(SDL_Rect* cameraRect, SDL_Rect rect)
{
	drawRender(cameraRect, rect, anims[anims.size() - 1], clip, flip);
}

