#pragma once
#include <list>
#include "ObjetoPortable.h"

using namespace std;

class Game;

class PoolObject : public ObjetoPortable
{
	bool active;
	list<PoolObject*>::const_iterator it;

public:
	PoolObject(Game* game) : ObjetoPortable(game), active(false) {};
	~PoolObject() = default;

	void activate(list<PoolObject*>::const_iterator iterator);
	void desactivate();

	bool isActive();

	list<PoolObject*>::const_iterator getIterator();

protected:
	virtual void onActivate() {}
	virtual void onDesactivate() {}
};

