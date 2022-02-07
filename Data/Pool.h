#pragma once
#include <vector>
#include "../GameObjects/Ingredientes/Ingrediente.h"

using namespace std;

template<class T>
class Pool
{
	vector<T> v;
public:
	Pool(int numElementos);
	Pool() {};
	~Pool();
};

