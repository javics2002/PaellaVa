#pragma once

#include "../Control/ObjectManager.h"

class Scene {
	Game* game;
	ObjectManager* objectManager;

public:
	Scene() {};
	virtual ~Scene() {};

	virtual void handleInput() {};
	virtual void update() {};
	virtual void render() {};
};