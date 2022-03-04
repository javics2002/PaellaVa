#pragma once

class Scene {
public:
	virtual void handleInput() {};
	virtual void update() {};
	virtual void render() {};
};