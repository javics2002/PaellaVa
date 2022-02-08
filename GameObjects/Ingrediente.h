#pragma once
class Ingrediente
{
	bool active;
public:
	Ingrediente() : active(false) {};
	~Ingrediente() {};

	void activate();
	void descativate();
	bool isActive();
};

