#pragma once
#include <SDL.h>

class Input
{
private:
	Sint16 x_move, y_move;
	SDL_Joystick* joystick_;
	
public:
	Input();
	~Input();
	bool GetKey(Uint8 key);
	void UpdateAxis();

	Sint16 getAxisX() { return x_move; }
	Sint16 getAxisY() { return y_move; }
};