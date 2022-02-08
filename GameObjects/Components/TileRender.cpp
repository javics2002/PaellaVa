#include "TileRender.h"
#include "../game/Game.h"
#include "../ecs/Camera.h"
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
TileRender::TileRender(SDL_Rect source, SDL_Rect dest, Texture* tx)
{
	src = source;
	dst = dest;
	text = tx;
}

void TileRender::init() {
	cam = entity_->getMngr()->getHandler<Camera__>()->getComponent<Camera>()->getCam();
}

void TileRender::render() {
	cam = entity_->getMngr()->getHandler<Camera__>()->getComponent<Camera>()->getCam();

	Vector2D renderPos = Vector2D(dst.x - cam.x, dst.y - cam.y);
	SDL_Rect dstFijo = build_sdlrect(renderPos, dst.w, dst.h);

	text->render(src, dstFijo);
}