#include "GrupoClientes.h"

GrupoClientes::GrupoClientes() {
	
}

void GrupoClientes::ini(int numClientes)
{
	//x = limite inferior, y = limite superior

	//paciencia = x + rand() % (j+1 - x);

	numeroClientes = numClientes;

	Cliente* cliente = nullptr;

	for (int i = 0; i < numeroClientes; i++) {
		cliente = new Cliente();
		clientes.push_back(cliente);
	}
}

void GrupoClientes::Pedir()
{

}

void GrupoClientes::Comer()
{

}

float GrupoClientes::Puntuacion()
{
	return 0.0f;
}

void GrupoClientes::NuevoGrupo()
{
	integrantes = 1 + rand() % (8 + 1 - 1);
}
