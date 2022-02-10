#pragma once
#include "GameObject.h"
#include <list>

class PoolObject : public GameObject
{
	bool active;
	list<PoolObject*>::const_iterator it;

public:
	PoolObject(Game* game) : GameObject(game), active(false) {};
	~PoolObject() = default;

	void activate(list<PoolObject*>::const_iterator iterator);
	void descativate();

	void setActive(bool b);
	bool isActive();

	list<PoolObject*>::const_iterator getIterator();
};

