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

void PoolObject::descativate()
{
	active = false;

	onDesactivate();
}

list<PoolObject*>::const_iterator PoolObject::getIterator()
{
	return it;
}
