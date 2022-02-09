#pragma once
#include <vector>
#include <iostream>
#include "../GameObjects/Ingrediente.h"
#include "../GameObjects/Collider.h"


using namespace std;

class Game;

template<typename T>
class Pool
{
	vector<T*> v;
	//numero de objetos en la Pool
	int numElems;
	//siguiente objeto en añadirse al juego
	int nextElem;

public:
	Pool(Game* game, int n) : numElems(n), nextElem(-1) {
		for (int i = 0; i < n; i++) {
			cout << "Objeto creado" << endl;
			v.push_back(new T(game));
		}			
	}

	~Pool() {
		for (auto i : v) {
			delete i;
			i = nullptr;
		}
	};

	//método que busca el siguiente objeto y lo activa
	void add(Vector2D<double> pos) {
		nextElem = (nextElem + 1) % numElems;
		int cont = 0;

		//el bucle no itera ninguna vez en la mayoría de casos (sobre todo en los ingredientes)
		//(es posible que en el caso de los clientes tarde más en encontrar al siguiente)
		while (cont < numElems && v[nextElem]->isActive())
		{
			cont++;
			nextElem = (nextElem + 1) % numElems;
		}
				
		if (cont < numElems) {
			v[nextElem]->activate();
			v[nextElem]->setPosition(pos);
		}
		//si no encuentra ninguno, parará la ejecución, indicando que hay que aumentar de antemano los objetos de la pool
		else
			throw string("ERROR: La pool no es lo suficientemente grande");
	}

	vector<Collider*> getColliders() {
		vector<Collider*> c;

		for (auto i : v) {
			if (i->isActive())
				c.push_back(i);
		}

		return c;
	}

	vector<Collider*> getCollisions(SDL_Rect GOcollider) {
		vector<Collider*> c;

		for (auto i : v) {
			if (i->isActive() && i->collide(GOcollider))
				c.push_back(i);
		}

		return c;
	}

	void render() {
		for (auto i : v) {
			if (i->isActive())
				i->draw();
		}			
	}

	void debug() {
		for (auto i : v) {
			if (i->isActive())
				i->drawDebug();
		}
	}

	void update() {
		for (auto i : v) {
			if (i->isActive())
				i->update();
		}
	}
};



