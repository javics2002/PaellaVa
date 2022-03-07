#pragma once
#include <vector>
#include <iostream>
#include <List>

#include "../GameObjects/Ingrediente.h"
#include "../GameObjects/PoolObject.h"
#include "../GameObjects/Collider.h"


using namespace std;

class Game;

template<typename T>
class Pool
{
	Game* game;

	//lista con solo los objetos de la pool que están activos
	list<PoolObject*> activeObjects;

	//vector con toda la pool
	vector<T*> v;
	//número de objetos en la Pool
	int numElems;
	//siguiente objeto a añadir al juego (o último en haberse añadido)
	int nextElem;

	void findNextElem() {
		nextElem = (nextElem + 1) % numElems;
		int cont = 0;

		//el bucle no itera ninguna vez en la mayoría de casos, sobre todo en los ingredientes
		//(es posible que en el caso de los clientes o paellas tarde más en encontrar al siguiente)
		while (cont < numElems && v[nextElem]->isActive())
		{
			cont++;
			nextElem = (nextElem + 1) % numElems;
		}
		//si no encuentra ninguno, se duplica el tamaño de la pool
		if (cont == numElems) {
			for (int i = 0; i < numElems; i++)
				v.push_back(new T(game));

			numElems *= 2;
		}
	}

public:
	Pool(Game* game, int n) : game(game), numElems(n), nextElem(-1) {
		for (int i = 0; i < n; i++)
			v.push_back(new T(game));
	}

	~Pool() {
		for (auto i : v) {
			delete i;
			i = nullptr;
		}
	};

	//método que busca el siguiente objeto y lo activa
	T* add(Vector2D<double> pos) {
		findNextElem();

		auto& elem = v[nextElem];
		activeObjects.push_front(elem);
		elem->activate(activeObjects.begin());
		elem->setPosition(pos);		

		return elem;
	}

	T* add() {
		findNextElem();

		auto& elem = v[nextElem];
		activeObjects.push_front(elem);
		elem->activate(activeObjects.begin());

		return elem;
	}

	//borra el objeto de la lista de activos
	void remove(list<PoolObject*>::const_iterator it) {
		activeObjects.erase(it);
	}

	vector<Collider*> getColliders() {
		vector<Collider*> c;

		for (auto it : activeObjects) {
			c.push_back(it);
		}

		return c;
	}

	vector<Collider*> getCollisions(SDL_Rect GOcollider) {
		vector<Collider*> c;

		for (auto it : activeObjects) {
			if (it->collide(GOcollider))
				c.push_back(it);
		}

		return c;
	}

	void render() {	
		for (auto it : activeObjects)
			it->render();
	}

	void render(SDL_Rect& rect) {
		for (auto it : activeObjects)
			it->render(rect);
	}

	void debug() {
		for (auto it : activeObjects) {
			it->drawDebug();
		}
	}

	void update() {
		for (auto it : activeObjects) {
			it->update();
		}
	}
};



