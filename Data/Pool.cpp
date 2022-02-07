#include "Pool.h"

template<class T>
inline Pool<T>::Pool(int numElementos)
{
	for (int i = 0; i < numElementos; i++)
		v.push_back(T());
}

template<class T>
Pool<T>::~Pool()
{
	for (auto i : v) {
		delete i;
		i = nullptr;
	}
		
}
