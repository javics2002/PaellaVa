#pragma once
#include "GameObject.h"

class Game;

class ObjetoPortable : public GameObject
{
	bool isPicked_;
	
public:
	ObjetoPortable(Game* game);
	~ObjetoPortable() {};

	void pickObject();
	void dropObject();

	void setPicked(bool b);
	bool isPicked();

	virtual bool canPick() { return true; };
	virtual bool canDrop() { return true; };

protected:
	virtual void onObjectPicked() = 0;
	virtual void onObjectDropped() = 0;
};

