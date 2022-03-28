// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <iostream>
#include <SDL.h>
#include <array>

#include "../utils/Singleton.h"


// Instead of a Singleton class, we could make it part of
// SDLUtils as well.

class InputHandler : public Singleton<InputHandler> {

	friend Singleton<InputHandler>;

	SDL_Joystick* joystick_;

	vector<bool> keyPressed;
	vector<bool> lastKeyPressed;
	vector<bool> mousePressed;

	bool leftLibre = true;
	bool rightLibre = true;
	bool upLibre = true;
	bool downLibre = true;

	std::pair<Sint32, Sint32> mousePos_;
	const Uint8* kbState_;
	const int CONTROLLER_DEAD_ZONE = 8000;
	float ejeX, ejeY;
	int mx = -1;
	int my = -1;

	InputHandler() {
		kbState_ = SDL_GetKeyboardState(0);
		initJoystick();
		keyPressed = vector<bool>(botones::UNKNOWN, false);
		lastKeyPressed = vector<bool>(botones::UNKNOWN, false);
		mousePressed = vector<bool>(botones::UNKNOWN, false);
	}

	inline void onMouseMotion(const SDL_Event& event) {
		mousePos_.first = event.motion.x;
		mousePos_.second = event.motion.y;
	}

	inline void onMouseButtonChange(const SDL_Event& event, bool isDown) {
		switch (event.button.button) {
		case SDL_BUTTON_LEFT:
			mousePressed[LEFT] = isDown;
			SDL_GetMouseState(&mx, &my);
			break;
		case SDL_BUTTON_MIDDLE:
			mousePressed[MOUSE_MIDDLE] = isDown;
			break;
		case SDL_BUTTON_RIGHT:
			mousePressed[MOUSE_RIGHT] = isDown;
			break;
		default:
			break;
		}
	}

public:
	enum MOUSEBUTTON : uint8_t { MOUSE_LEFT, MOUSE_MIDDLE, MOUSE_RIGHT };

	//Unknown debe ser el último botón para marcar el número de botones
	enum botones { LEFT, RIGHT, UP, DOWN, INTERACT, CANCEL, UNKNOWN };

	virtual ~InputHandler() {
	}

	// update the state with a new event
	inline void update(SDL_Event& event, bool& exit) {
		// sincronizar
		lastKeyPressed = keyPressed;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				onKeyboardDown(event.key.keysym.scancode);
				break;
			case SDL_KEYUP:
				onKeyboardUp(event.key.keysym.scancode);
				break;
			case SDL_MOUSEMOTION:
				onMouseMotion(event);
				break;
			case SDL_MOUSEBUTTONDOWN:
				onMouseButtonChange(event, true);
				break;
			case SDL_MOUSEBUTTONUP:
				onMouseButtonChange(event, false);
				break;
			case SDL_JOYAXISMOTION:
				onJoystickMotion(event);
				break;
			case SDL_QUIT:
				exit = true;
				break;
			default:
				break;
			}
		}
		
		
	}

	inline void initJoystick()
	{
		//Check for joysticks
		if (SDL_NumJoysticks() < 1)
		{
			printf("Warning: No joysticks connected!\n");
		}
		else
		{
			//Load joystick
			joystick_ = SDL_JoystickOpen(0);
			if (joystick_ == NULL)
			{
				printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
			}
		}
	}

	inline int getAxisX()
	{
		return ejeX;
	}

	inline int getAxisY()
	{
		return ejeY;
	}

	inline void onKeyboardDown(SDL_Scancode key) {
		switch (key) {
		case SDL_SCANCODE_A:
		case SDL_SCANCODE_LEFT:
			{
				keyJustDown(botones::LEFT);
			}
			break;
		case SDL_SCANCODE_D:
		case SDL_SCANCODE_RIGHT:
			{
				keyJustDown(botones::RIGHT);
			}
			break;
		case SDL_SCANCODE_W:
		case SDL_SCANCODE_UP:
			{
				keyJustDown(botones::UP);
			}
			break;
		case SDL_SCANCODE_S:
		case SDL_SCANCODE_DOWN:
			{
				keyJustDown(botones::DOWN);
			}
			break;
		case SDL_SCANCODE_E:
		case SDL_SCANCODE_SPACE:
		case SDL_SCANCODE_EXECUTE:
			keyJustDown(botones::INTERACT);
			break;
		case SDL_SCANCODE_ESCAPE:
			keyJustDown(botones::CANCEL);
			break;
		default:
			break;
		}
	}

	//Se encarga de revisar de que una tecla se acaba de pulsar y actualiza el estado en ese caso
	inline void keyJustDown(botones boton) {
		if (!keyPressed[boton]) {
			//KeyDown!
			keyPressed[boton] = true;

			switch (boton) {
			case botones::LEFT:
				if (rightLibre)
				{
					ejeX = -1; // valor entre -1 y 1
					leftLibre = false;
				}
				break;
			case botones::RIGHT:
				if (leftLibre)
				{
					ejeX = 1;
					rightLibre = false;
				}
				break;
			case botones::UP:
				if (downLibre)
				{
					ejeY = -1; // valor entre -1 y 1
					upLibre = false;
				}
				break;
			case botones::DOWN:
				if (upLibre)
				{
					ejeY = 1;
					downLibre = false;
				}
				break;
			case botones::INTERACT:
				break;
			case botones::CANCEL:
				// pausa
				// lastKeyPressed


				break;
			default:
				break;
			}
		}
	}

	inline void onKeyboardUp(SDL_Scancode key) {
		switch (key) {
		case SDL_SCANCODE_A:
		case SDL_SCANCODE_LEFT:
			//if (rightLibre)
			{
				keyPressed[botones::LEFT] = false;
				ejeX = keyPressed[botones::RIGHT] ? 1 : 0;
			}
			leftLibre = true;
			break;
		case SDL_SCANCODE_D:
		case SDL_SCANCODE_RIGHT:
			//if (leftLibre)
			{
				keyPressed[botones::RIGHT] = false;
				ejeX = keyPressed[botones::LEFT] ? -1 : 0;
			}
			rightLibre = true;
			break;
		case SDL_SCANCODE_W:
		case SDL_SCANCODE_UP:
			//if (downLibre)
			{
				keyPressed[botones::UP] = false;
				ejeY = keyPressed[botones::DOWN] ? 1 : 0;
			}
			upLibre = true;
			break;
		case SDL_SCANCODE_S:
		case SDL_SCANCODE_DOWN:
			//if (upLibre)
			{
				keyPressed[botones::DOWN] = false;
				ejeY = keyPressed[botones::UP] ? -1 : 0;
			}
			downLibre = true;
			break;
		case SDL_SCANCODE_E:
		case SDL_SCANCODE_SPACE:
		case SDL_SCANCODE_EXECUTE:
			keyPressed[botones::INTERACT] = false;
			break;
		case SDL_SCANCODE_ESCAPE:
			keyPressed[botones::CANCEL] = false;
			break;
		default:
			break;
		}
	}

	inline void onJoystickMotion(const SDL_Event& e) {
		if (e.jaxis.which == 0)//controller 0
		{
			if (e.jaxis.axis == 0)// x axis
			{
				if (e.jaxis.value < -CONTROLLER_DEAD_ZONE)
					ejeX = e.jaxis.value; // valor entre -1 y 1
				else if (e.jaxis.value > CONTROLLER_DEAD_ZONE)
					ejeX = e.jaxis.value;
				else
					ejeX = 0;
			}

			else if (e.jaxis.axis == 1)//y axis
			{
				if (e.jaxis.value < -CONTROLLER_DEAD_ZONE)
					ejeY = e.jaxis.value;
				else if (e.jaxis.value > CONTROLLER_DEAD_ZONE)
					ejeY = e.jaxis.value;
				else
					ejeY = 0;
			}
		}
	}

	inline Vector2D<double> getAxis() {
		Vector2D<double> axis(ejeX, ejeY);
		axis.normalize();
		return axis;
	}

	inline const std::pair<Sint32, Sint32>& getMousePos() {
		return mousePos_;
	}

	inline int getMouseButtonState(MOUSEBUTTON b) {
		if (mousePressed[b]) {
			mousePressed[b] = false;
			return true;
		}
		else return false;
	}

	bool getKey(Uint8 key)
	{
		return SDL_GetKeyboardState(NULL)[key];
	}

	bool getKey(botones boton)
	{
		return keyPressed[boton] && !lastKeyPressed[boton];
	}

	// TODO add support for Joystick, see Chapter 4 of
	// the book 'SDL Game Development'
	int getmx() { return mx; };
	int getmy() { return my; };
};

// This macro defines a compact way for using the singleton InputHandler, instead of
// writing InputHandler::instance()->method() we write ih().method()
//
inline InputHandler& ih() {
	return *InputHandler::instance();
}