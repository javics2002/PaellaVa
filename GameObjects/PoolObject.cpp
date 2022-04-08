#include "PoolObject.h"

bool PoolObject::isAlive()
{
	return alive;
}

void PoolObject::activate()
{
	alive = true;

	onActivate();
}

void PoolObject::deactivate()
{
	alive = false;

	onDeactivate();
}

