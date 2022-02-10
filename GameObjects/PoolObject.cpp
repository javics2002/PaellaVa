#include "PoolObject.h"

void PoolObject::setActive(bool b)
{
	active = b;
}

bool PoolObject::isActive()
{
	return active;
}

void PoolObject::activate(list<PoolObject*>::const_iterator iterator)
{
	it = iterator;

	active = true;
}

void PoolObject::descativate()
{
	active = false;
}

list<PoolObject*>::const_iterator PoolObject::getIterator()
{
	return it;
}
