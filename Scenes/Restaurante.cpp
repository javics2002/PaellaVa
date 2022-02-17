#include "Restaurante.h"
#include "../GameObjects/GrupoClientes.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Paella.h"
#include "../GameObjects/Ingrediente.h"
#include "../GameObjects/Muebles/Mueble.h"
#include <iostream>
using namespace std;

Restaurante::Restaurante()
{
	host = new Player(Posicion{ 200, 200 });
}

Restaurante::~Restaurante()
{
}

void Restaurante::Render()
{
	host->draw();
}
