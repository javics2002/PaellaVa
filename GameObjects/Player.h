#pragma once
#include "GameObject.h"
#include "ObjetoPortable.h"
#include "../sdlutils/InputHandler.h"

class Ingrediente;
class Cliente;
class Paella;
class Arroz;
class Mueble;

enum orientation { N, S, E, O };
enum objectType { INGREDIENTE, CLIENTES, PAELLA, ARROZ};

class Player : public GameObject
{
	ObjetoPortable* pickedObject_;
	objectType objectType_;

	bool chef_;		// False is waiter

	orientation orientation_;
	
	float aceleracion, deceleracion, maxVel;

	Vector2D<double> overlapPos;
	Vector2D<int> overlapDim;

	bool nearestObject(ObjetoPortable* go);
	Mueble* nearestObject(Mueble* m1, Mueble* m2);

	void setAnimResources();
	void animUpdate(Vector2D<double> axis);

	vector<Texture*> anims;
	SDL_RendererFlip flip;
	SDL_Rect clip;
	int frameCounter;
	float lastFrameTime;
	float frameRate;
	int currAnim;

public:
	Player(Game* game, bool chef);
	~Player();

	void handleInput(Vector2D<double> axis);

	void update() override;
	void renderDebug(SDL_Rect* cameraRect) override;
	void render(SDL_Rect* cameraRect) override;

	void setPickedObject(ObjetoPortable* op, objectType ot);

	//Vector2D<double> getAxis() { return ih().getAxis(); }

	void setVel(double x, double y);
	void setVel(Vector2D<double> vel2);
	Vector2D<double> getVel();
	Vector2D<double> getOrientation();

	SDL_Rect getCollider() override;
	SDL_Rect getOverlap() override;
};

