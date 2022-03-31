#include "PoolObject.h"

bool PoolObject::isActive()
{
	return active;
}

void PoolObject::activate(list<PoolObject*>::const_iterator iterator)
{
	it = iterator;

	active = true;

	onActivate();
}

void PoolObject::deactivate()
{
	active = false;

	onDeactivate();
}

list<PoolObject*>::const_iterator PoolObject::getIterator()
{
	return it;
}
