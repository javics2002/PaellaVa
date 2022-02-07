#include "Input.h"

Input::Input()
{
	joystick_ = nullptr;
	x_move = 0;
	y_move = 0;
}

Input::~Input()
{
}

bool Input::GetKey(Uint8 key)
{
	return SDL_GetKeyboardState(NULL)[key];
}

void Input::UpdateAxis() 
{
	x_move = SDL_JoystickGetAxis(joystick_, 0); // Eje horizontal
	y_move = SDL_JoystickGetAxis(joystick_, 1); // Eje vertical
}
