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

	void setIsPicked(bool b);
	bool getIsPicked();

protected:
	virtual void onObjectPicked() = 0;
	virtual void onObjectDropped() = 0;

};

