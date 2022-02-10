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
	//vector con toda la pool
	vector<T*> v;
	//número de objetos en la Pool
	int numElems;
	//siguiente objeto a añadir al juego (o último en haberse añadido)
	int nextElem;

	//lista con solo los objetos de la pool que están activos
	list<PoolObject*> activeObjects;

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

		//el bucle no itera ninguna vez en la mayoría de casos, sobre todo en los ingredientes
		//(es posible que en el caso de los clientes o paellas tarde más en encontrar al siguiente)
		while (cont < numElems && v[nextElem]->isActive())
		{
			cont++;
			nextElem = (nextElem + 1) % numElems;
		}
				
		if (cont < numElems) {
			auto& elem = v[nextElem];
			activeObjects.push_front(elem);
			elem->activate(activeObjects.begin());
			elem->setPosition(pos);
		}
		//si no encuentra ninguno, parará la ejecución, indicando que hay que aumentar de antemano los objetos de la pool
		else
			throw string("ERROR: La pool no es lo suficientemente grande");
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
			it->draw();
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

	//borra el objeto de la lista de activos
	void erase(list<PoolObject*>::const_iterator it) {
		activeObjects.erase(it);
	}	
};



