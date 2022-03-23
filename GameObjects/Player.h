#pragma once
#include "GameObject.h"
#include "ObjetoPortable.h"
#include "../Control/Input.h"
#include "../sdlutils/InputHandler.h"

class Ingrediente;
class Cliente;
class Paella;

class Mueble;

enum orientation { N, S, E, O };
enum objectType { INGREDIENTE, CLIENTES, PAELLA};

class Player : public GameObject
{
	float lastTime_;

	ObjetoPortable* pickedObject_;
	objectType objectType_;

	Input* input_;
	orientation orientation_;
	
	float aceleracion, deceleracion, maxVel;

	Vector2D<double> overlapPos;
	Vector2D<int> overlapDim;

	bool nearestObject(ObjetoPortable* go);
	Mueble* nearestObject(Mueble* m1, Mueble* m2);
	void setAnimResources();
	void animUpdate();

	int id_;

public:
	Player(Game* game);
	~Player();

	void handleInput();
	void handleInput(Vector2D<double> axis);

	void update() override;
	void renderDebug(SDL_Rect* cameraRect) override;

	SDL_Rect getOverlapCollider();	

	void setPickedObject(ObjetoPortable* op, objectType ot);

	void setId(int id) { id_ = id; }
	int getId() { return id_; }

	Vector2D<double> getAxis() { return ih().getAxis(); }
};

