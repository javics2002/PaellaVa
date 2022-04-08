#pragma once

class PoolObject
{
	bool alive;

public:
	PoolObject() : alive(false) {};
	~PoolObject() = default;

	void activate();
	void deactivate();

	bool isAlive();

protected:
	virtual void onActivate() {}
	virtual void onDeactivate() {}
};

