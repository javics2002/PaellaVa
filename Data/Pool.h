#pragma once
#include <vector>
#include <iostream>
#include "../GameObjects/Ingrediente.h"


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
	Pool(Game* game, int n) : numElems(n), nextElem(0) {
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

	//método que activa el siguiente objeto ("mete el objeto en la escena")
	void add() {
		v[nextElem]->activate();
	}

	void render() {
		for (auto i : v) {
			if (i->isActive());
				//i->
		}
			
	}
};



