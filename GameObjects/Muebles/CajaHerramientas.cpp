#include "CajaHerramientas.h"
#include "../Ingrediente.h"
#include "../Player.h"
#include "../Paella.h"
#include "../../Control/Game.h"
#include "../Herramienta.h"

CajaHerramientas::CajaHerramientas(Game* game, Vector2D<double> pos) : Mueble(game, pos, 1 * TILE_SIZE, 2 * TILE_SIZE, "encimera")
{
	//Siempre tiene que funcionar
	funcionando = true;

	tool_ = new Herramienta(game);
}

bool CajaHerramientas::receiveHerramienta(Herramienta* h)
{
	//Si ya tiene objeto, no recoge objeto
	if (tool_ == nullptr)
	{
		tool_ = h;

		tool_->setPosition(getRectCenter(getOverlap()));

		return true;
	}
	else
		return false;
}

bool CajaHerramientas::returnObject(Player* p)
{
	if (tool_ != nullptr)
	{
		//TOCHECK: Podríamos hacer un return del objeto y que el player se lo guarde a sí mismo
		p->setPickedObject(tool_, HERRAMIENTA);

		tool_ = nullptr;

		return true;
	}
	else
		return false;
}

