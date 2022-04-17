// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <iostream>
#include <SDL.h>
#include <array>

#include "../utils/Singleton.h"
#include "../Control/Game.h"
#include "../Control/NetworkManager.h"


// Instead of a Singleton class, we could make it part of
// SDLUtils as well.

class InputHandler : public Singleton<InputHandler> {

	friend Singleton<InputHandler>;

	Game* game;
	SDL_GameController* mController;

	vector<bool> mKeyPressed;
	vector<bool> mLastKeyPressed;
	vector<bool> mMousePressed;
	vector<char> mCurrentKey;
	std::pair<Sint32, Sint32> mMousePos;
	const Uint8* mKeyboardState;
	const int CONTROLLER_DEAD_ZONE = 8000;
	float ejeX, ejeY;

	int mx = -1;
	int my = -1;

	bool isMouseButtonHeldDown_;

	vector<bool> otherKeyPressed; // Hecho para el otro personaje
	vector<bool> otherLastKeyPressed; // Hecho para el otro personaje
	float otherEjeX, otherEjeY; // Hecho para el otro personaje

	InputHandler() {
		mKeyboardState = SDL_GetKeyboardState(0);
		mKeyPressed = vector<bool>(botones::UNKNOWN, false);
		mLastKeyPressed = vector<bool>(botones::UNKNOWN, false);

		otherKeyPressed = vector<bool>(4, false);
		otherLastKeyPressed = vector<bool>(4, false);

		mMousePressed = vector<bool>(3, false);
		isMouseButtonHeldDown_ = false;

		mController = nullptr;
	}

	inline void onMouseMotion(const SDL_Event& event) {
		mMousePos.first = event.motion.x;
		mMousePos.second = event.motion.y;
	}

	inline void onMouseButtonChange(const SDL_Event& event, bool isDown) {
		isMouseButtonHeldDown_ = isDown;
		switch (event.button.button) {
		case SDL_BUTTON_LEFT:
			mMousePressed[LEFT] = isDown;
			SDL_GetMouseState(&mx, &my);
			break;
		case SDL_BUTTON_MIDDLE:
			mMousePressed[MOUSE_MIDDLE] = isDown;
			break;
		case SDL_BUTTON_RIGHT:
			mMousePressed[MOUSE_RIGHT] = isDown;
			break;
		default:
			break;
		}
	}

public:
	enum MOUSEBUTTON : uint8_t { MOUSE_LEFT, MOUSE_MIDDLE, MOUSE_RIGHT };

	//Unknown debe ser el �ltimo bot�n para marcar el n�mero de botones
	enum botones { LEFT, RIGHT, UP, DOWN, INTERACT, CANCEL, J, UNKNOWN };

	virtual ~InputHandler() {
	}

	inline void resetHeld() {
		isMouseButtonHeldDown_ = false;
	}

	// update the state with a new event
	inline void update(SDL_Event& event, bool& exit) {
		// sincronizar
		mLastKeyPressed = mKeyPressed;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				onKeyboardDown(event.key.keysym.scancode);
				typeKey(event.key.keysym.sym);
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

			case SDL_CONTROLLERAXISMOTION:
				onJoystickMotion(event);
				break;
			case SDL_CONTROLLERBUTTONDOWN:
				switch (event.cbutton.button)
				{
				case SDL_CONTROLLER_BUTTON_A:
					keyJustDown(botones::INTERACT);
					break;
				case SDL_CONTROLLER_BUTTON_B:
				case SDL_CONTROLLER_BUTTON_START:
					keyJustDown(botones::CANCEL);
					break;
				case SDL_CONTROLLER_BUTTON_Y:
					keyJustDown(botones::J);
					break;
				}
				break;
			case SDL_CONTROLLERBUTTONUP:
				switch (event.cbutton.button)
				{
				case SDL_CONTROLLER_BUTTON_A:
					mKeyPressed[botones::INTERACT] = false;
					break;
				case SDL_CONTROLLER_BUTTON_B:
				case SDL_CONTROLLER_BUTTON_START:
					mKeyPressed[botones::CANCEL] = false;
					break;
				case SDL_CONTROLLER_BUTTON_Y:
					mKeyPressed[botones::J] = false;
					break;
				}
				break;

			case SDL_CONTROLLERDEVICEADDED:
				onControllerAdded();
				cout << event.cdevice.which << endl;
				break;
			case SDL_CONTROLLERDEVICEREMOVED:
				onControllerRemoved(event);
				cout << event.cdevice.which << endl;
				break;

			case SDL_WINDOWEVENT:
				switch (event.window.event) {
				case SDL_WINDOWEVENT_MOVED:
					cout << "Has movido la ventana" << endl;
					break;
				}
				break;

			case SDL_QUIT:
				exit = true;
				break;
			default:
				break;
			}
		}
	}

	void onControllerAdded() {
		//Si conectamos un nuevo mando lo cogemos si no tenemos ninguno
		if (mController == nullptr) {
			mController = SDL_GameControllerOpen(0);

			cout << "Mando conectado" << endl;

			if (!SDL_IsGameController(0)) {
				SDL_GameControllerClose(mController);

				cout << "El mando no me vale" << endl;
			}
		}
	}

	void onControllerRemoved(SDL_Event& event) {
		//Si se desconecta el mando usado, lo cerramos
		if (mController != nullptr) {
			SDL_GameControllerClose(mController);
			mController = nullptr;

			cout << "Mando desconectado" << endl;
		}
	}

	inline void onKeyboardDown(SDL_Scancode key) {
		switch (key) {
		case SDL_SCANCODE_A:
		case SDL_SCANCODE_LEFT:
			keyJustDown(botones::LEFT);
		break;
		case SDL_SCANCODE_D:
		case SDL_SCANCODE_RIGHT:
			keyJustDown(botones::RIGHT);
		break;
		case SDL_SCANCODE_W:
		case SDL_SCANCODE_UP:
			keyJustDown(botones::UP);
		break;
		case SDL_SCANCODE_S:
		case SDL_SCANCODE_DOWN:
			keyJustDown(botones::DOWN);
		break;
		case SDL_SCANCODE_E:
		case SDL_SCANCODE_SPACE:
		case SDL_SCANCODE_EXECUTE:
			keyJustDown(botones::INTERACT);
			break;
		case SDL_SCANCODE_ESCAPE:
			keyJustDown(botones::CANCEL);
			break;
		case SDL_SCANCODE_J:
			keyJustDown(botones::J);
		default:
			break;
		}
	}

	//Se encarga de revisar de que una tecla se acaba de pulsar y actualiza el estado en ese caso
	inline void keyJustDown(botones boton) {
		if (!mKeyPressed[boton]) {
			//KeyDown!
			mKeyPressed[boton] = true;

			switch (boton) {
			case botones::LEFT:
				ejeX = -1; // valor entre -1 y 1
				break;
			case botones::RIGHT:
				ejeX = 1;
				break;
			case botones::UP:
				ejeY = -1; // valor entre -1 y 1
				break;
			case botones::DOWN:
				ejeY = 1;
				break;
			case botones::INTERACT:
				break;
			case botones::CANCEL:
				break;
			case botones::J:
				break;
			default:
				break;
			}

			game->getNetworkManager()->sendButtonsBuffer(mKeyPressed);
		}
	}

	inline void onKeyboardUp(SDL_Scancode key) {
		switch (key) {
		case SDL_SCANCODE_A:
		case SDL_SCANCODE_LEFT:
			mKeyPressed[botones::LEFT] = false;
			ejeX = mKeyPressed[botones::RIGHT] ? 1 : 0;
			break;
		case SDL_SCANCODE_D:
		case SDL_SCANCODE_RIGHT:
			mKeyPressed[botones::RIGHT] = false;
			ejeX = mKeyPressed[botones::LEFT] ? -1 : 0;
			break;
		case SDL_SCANCODE_W:
		case SDL_SCANCODE_UP:
			mKeyPressed[botones::UP] = false;
			ejeY = mKeyPressed[botones::DOWN] ? 1 : 0;
			break;
		case SDL_SCANCODE_S:
		case SDL_SCANCODE_DOWN:
			mKeyPressed[botones::DOWN] = false;
			ejeY = mKeyPressed[botones::UP] ? -1 : 0;
			break;
		case SDL_SCANCODE_E:
		case SDL_SCANCODE_SPACE:
		case SDL_SCANCODE_EXECUTE:
			mKeyPressed[botones::INTERACT] = false;
			break;
		case SDL_SCANCODE_ESCAPE:
			mKeyPressed[botones::CANCEL] = false;
			break;
		case SDL_SCANCODE_J:
			mKeyPressed[botones::J] = false;
			break;
		default:
			break;
		}

		game->getNetworkManager()->sendButtonsBuffer(mKeyPressed);
	}

	inline void updateOtherAxis() {
		// sincronizar
		// otherLastKeyPressed = otherKeyPressed;

		if (otherKeyPressed[botones::RIGHT]) {
			otherEjeX = 1;
		}
		else if (otherKeyPressed[botones::LEFT]) {
			otherEjeX = -1;
		}
		else
			otherEjeX = 0;

		if (otherKeyPressed[botones::UP]) {
			otherEjeY = -1;
		}
		else if (otherKeyPressed[botones::DOWN]) {
			otherEjeY = 1;
		}
		else
			otherEjeY = 0;
	}

	inline void onJoystickMotion(const SDL_Event& e) {
		switch (e.caxis.axis) {
		case SDL_CONTROLLER_AXIS_LEFTX:
			if (e.caxis.value < -CONTROLLER_DEAD_ZONE)
				ejeX = e.caxis.value; // valor entre -1 y 1
			else if (e.caxis.value > CONTROLLER_DEAD_ZONE)
				ejeX = e.caxis.value;
			else
				ejeX = 0;
			break;
		case SDL_CONTROLLER_AXIS_LEFTY:
			if (e.caxis.value < -CONTROLLER_DEAD_ZONE)
				ejeY = e.caxis.value;
			else if (e.caxis.value > CONTROLLER_DEAD_ZONE)
				ejeY = e.caxis.value;
			else
				ejeY = 0;
			break;
		}
	}

	inline Vector2D<double> getAxis() {
		Vector2D<double> axis(ejeX, ejeY);
		axis.normalize();
		return axis;
	}

	inline const std::pair<Sint32, Sint32>& getMousePos() {
		return mMousePos;
	}

	inline int getMouseButtonState(MOUSEBUTTON b) {
		if (mMousePressed[b]) {
			mMousePressed[b] = false;
			return true;
		}
		else return false;
	}

	bool getKey(Uint8 key)
	{
		cout << SDL_GetKeyboardState(NULL)[key];
		return SDL_GetKeyboardState(NULL)[key];

	}

	bool getKey(botones boton)
	{
		return mKeyPressed[boton] && !mLastKeyPressed[boton];
	}

	void typeKey(SDL_Keycode key)
	{
		mCurrentKey.push_back((char)key);
		//return key;
	}

	char getTypedKey()
	{
		if (!mCurrentKey.empty())
		{
			char c = mCurrentKey[0];
			mCurrentKey.pop_back();
			return c;
		}
		else
			return' ';
	}

	void clearInputBuffer() {
		mCurrentKey.clear();
	}

	bool getMouseButtonHeld() {
		SDL_GetMouseState(&mx, &my);
		return isMouseButtonHeldDown_;
	}



	// TODO add support for Joystick, see Chapter 4 of
	// the book 'SDL Game Development'
	int getmx() { return mx; };
	int getmy() { return my; };

	inline void setOtherKeyPressed(vector<bool> otherKeyPressed_) {
		otherKeyPressed = otherKeyPressed_;
	}

	inline vector<bool> getOtherKeyPressed() {
		return otherKeyPressed;
	}

	inline void setOtherAxisX(float ejeX)
	{
		otherEjeX = ejeX;
	}

	inline void setOtherAxisY(float ejeY)
	{
		otherEjeY = ejeY;
	}

	inline Vector2D<double> getOtherAxis() {
		Vector2D<double> otherAxis(otherEjeX, otherEjeY);
		otherAxis.normalize();
		return otherAxis;
	}

	inline int getOtherAxisX()
	{
		return otherEjeX;
	}

	inline int getOtherAxisY()
	{
		return otherEjeY;
	}

	inline int getAxisX()
	{
		return ejeX;
	}

	inline int getAxisY()
	{
		return ejeY;
	}

	void setGame(Game* game_) { game = game_; }
};

// This macro defines a compact way for using the singleton InputHandler, instead of
// writing InputHandler::instance()->method() we write ih().method()
//
inline InputHandler& ih() {
	return *InputHandler::instance();
}

