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
	bool focusedcontrols = false;

	bool isMouseButtonHeldDown_;

	vector<bool> otherKeyPressed; // Hecho para el otro personaje
	vector<bool> otherLastKeyPressed; // Hecho para el otro personaje
	float otherEjeX, otherEjeY; // Hecho para el otro personaje

	InputHandler() {
		mKeyboardState = SDL_GetKeyboardState(0);
		mKeyPressed = vector<bool>(UNKNOWN, false);
		mLastKeyPressed = vector<bool>(UNKNOWN, false);

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

	/* Estos botones son de mando de XBOX/teclado
	A/E, SPACE, ENTER es Interactuar
	B/ESC es Cancelar, Volver
	X/Left Shift es Abrir o cerrar la libretita de las comandas
	Y/Q es avanzar el texto
	LB, RB, LT, RT ajustan el volumen en el menu de configuracion
	Pause/ESC pausa el juego
	Las flechas de direccion se pueden controlar tanto con WASD, las flechas del teclado,
	el dpad del mando y el joystick izquierdo
	Unknown debe ser el �ltimo bot�n para marcar el n�mero de botones*/
	enum botones { LEFT, RIGHT, UP, DOWN, A, B, X, Y, LB, RB, LT, RT, PAUSE, TAB,UNKNOWN };

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
				onControllerDown(event);
				break;
			case SDL_CONTROLLERBUTTONUP:
				onControllerUp(event);
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

	void onControllerDown(SDL_Event& event)
	{
		switch (event.cbutton.button)
		{
		case SDL_CONTROLLER_BUTTON_A:
			keyJustDown(A);
			break;
		case SDL_CONTROLLER_BUTTON_B:
			keyJustDown(B);
			break;
		case SDL_CONTROLLER_BUTTON_X:
			keyJustDown(X);
			break;
		case SDL_CONTROLLER_BUTTON_Y:
			keyJustDown(Y);
			break;

		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
			keyJustDown(LB);
			break;
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
			keyJustDown(RB);
			break;

		case SDL_CONTROLLER_BUTTON_START:
			keyJustDown(PAUSE);
			break;

		case SDL_CONTROLLER_BUTTON_DPAD_UP:
			keyJustDown(UP);
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
			keyJustDown(DOWN);
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			keyJustDown(LEFT);
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			keyJustDown(RIGHT);
			break;
		}
	}

	void onControllerUp(SDL_Event& event)
	{
		switch (event.cbutton.button)
		{
		case SDL_CONTROLLER_BUTTON_A:
			mKeyPressed[A] = false;
			break;
		case SDL_CONTROLLER_BUTTON_B:
			mKeyPressed[B] = false;
			break;
		case SDL_CONTROLLER_BUTTON_X:
			mKeyPressed[X] = false;
			break;
		case SDL_CONTROLLER_BUTTON_Y:
			mKeyPressed[Y] = false;
			break;

		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
			mKeyPressed[LB] = false;
			break;
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
			mKeyPressed[RB] = false;
			break;

		case SDL_CONTROLLER_BUTTON_START:
			mKeyPressed[PAUSE] = false;
			break;

		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			mKeyPressed[LEFT] = false;
			ejeX = mKeyPressed[RIGHT] ? 1 : 0;
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			mKeyPressed[RIGHT] = false;
			ejeX = mKeyPressed[LEFT] ? -1 : 0;
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_UP:
			mKeyPressed[UP] = false;
			ejeY = mKeyPressed[DOWN] ? 1 : 0;
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
			mKeyPressed[DOWN] = false;
			ejeY = mKeyPressed[UP] ? -1 : 0;
			break;
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

	void onKeyboardDown(SDL_Scancode key) {
		switch (key) {
		case SDL_SCANCODE_A:
		case SDL_SCANCODE_LEFT:
			keyJustDown(LEFT);
		break;
		case SDL_SCANCODE_D:
		case SDL_SCANCODE_RIGHT:
			keyJustDown(RIGHT);
		break;
		case SDL_SCANCODE_W:
		case SDL_SCANCODE_UP:
			keyJustDown(UP);
		break;
		case SDL_SCANCODE_S:
		case SDL_SCANCODE_DOWN:
			keyJustDown(DOWN);
		break;

		case SDL_SCANCODE_E:
		case SDL_SCANCODE_SPACE:
		case SDL_SCANCODE_RETURN: //Enter
			keyJustDown(A);
			break;
		case SDL_SCANCODE_ESCAPE:
			keyJustDown(B);
			keyJustDown(PAUSE);
			break;
		case SDL_SCANCODE_Q:
			keyJustDown(X);
			break;
		case SDL_SCANCODE_LSHIFT:
			keyJustDown(Y);
			break;
		case SDL_SCANCODE_TAB:
			keyJustDown(TAB);
			break;
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
			case LEFT:
				ejeX = -1; // valor entre -1 y 1
				break;
			case RIGHT:
				ejeX = 1;
				break;
			case UP:
				ejeY = -1; // valor entre -1 y 1
				break;
			case DOWN:
				ejeY = 1;
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
			mKeyPressed[LEFT] = false;
			ejeX = mKeyPressed[RIGHT] ? 1 : 0;
			break;
		case SDL_SCANCODE_D:
		case SDL_SCANCODE_RIGHT:
			mKeyPressed[RIGHT] = false;
			ejeX = mKeyPressed[LEFT] ? -1 : 0;
			break;
		case SDL_SCANCODE_W:
		case SDL_SCANCODE_UP:
			mKeyPressed[UP] = false;
			ejeY = mKeyPressed[DOWN] ? 1 : 0;
			break;
		case SDL_SCANCODE_S:
		case SDL_SCANCODE_DOWN:
			mKeyPressed[DOWN] = false;
			ejeY = mKeyPressed[UP] ? -1 : 0;
			break;

		case SDL_SCANCODE_E:
		case SDL_SCANCODE_SPACE:
		case SDL_SCANCODE_RETURN:
			mKeyPressed[A] = false;
			break;
		case SDL_SCANCODE_ESCAPE:
			mKeyPressed[B] = false;
			mKeyPressed[PAUSE] = false;
			break;
		case SDL_SCANCODE_Q:
			mKeyPressed[X] = false;
			break;
		case SDL_SCANCODE_LSHIFT:
			mKeyPressed[Y] = false;
			break;
		case SDL_SCANCODE_TAB:
			mKeyPressed[TAB] = false;
			break;
		default:
			break;
		}

		game->getNetworkManager()->sendButtonsBuffer(mKeyPressed);
	}

	inline void updateOtherAxis() {
		// sincronizar
		// otherLastKeyPressed = otherKeyPressed;

		if (otherKeyPressed[RIGHT]) {
			otherEjeX = 1;
		}
		else if (otherKeyPressed[LEFT]) {
			otherEjeX = -1;
		}
		else
			otherEjeX = 0;

		if (otherKeyPressed[UP]) {
			otherEjeY = -1;
		}
		else if (otherKeyPressed[DOWN]) {
			otherEjeY = 1;
		}
		else
			otherEjeY = 0;
	}

	inline void onJoystickMotion(const SDL_Event& e) {
		switch (e.caxis.axis) {
		case SDL_CONTROLLER_AXIS_LEFTX:
			if (e.caxis.value < -CONTROLLER_DEAD_ZONE || e.caxis.value > CONTROLLER_DEAD_ZONE) {
				ejeX = e.caxis.value;
				mKeyPressed[RIGHT] = true;
			}
			else {
				ejeX = 0;
				mKeyPressed[LEFT] = mKeyPressed[RIGHT] = false;
			}
			break;
		case SDL_CONTROLLER_AXIS_LEFTY:
			if (e.caxis.value < -CONTROLLER_DEAD_ZONE) {
				ejeY = e.caxis.value;
				mKeyPressed[UP] = true;
			}
			else if (e.caxis.value > CONTROLLER_DEAD_ZONE) {
				ejeY = e.caxis.value;
				mKeyPressed[DOWN] = true;
			}
			else {
				ejeY = 0;
				mKeyPressed[DOWN] = mKeyPressed[UP] = false;
			}
			break;
		case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
			if(e.caxis.value > CONTROLLER_DEAD_ZONE)
				keyJustDown(LT);
			else
				mKeyPressed[LT] = false;
			break;
		case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
			if (e.caxis.value > CONTROLLER_DEAD_ZONE)
				keyJustDown(RT);
			else
				mKeyPressed[RT] = false;
			break;
		}
	}

	inline Vector2D<double> getAxis() {
		Vector2D<double> axis(ejeX, ejeY);
		if(axis.magnitude() > 1)
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
	void setFocused(bool b) { focusedcontrols = b; };
	void setKey(bool b, botones boton) { mKeyPressed[boton] = b; };
	bool getFocus() { return focusedcontrols; };
};

// This macro defines a compact way for using the singleton InputHandler, instead of
// writing InputHandler::instance()->method() we write ih().method()
//
inline InputHandler& ih() {
	return *InputHandler::instance();
}

