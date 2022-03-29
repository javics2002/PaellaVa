#pragma once
#include "Mueble.h"
#include "Silla.h"
#include "../Paella.h"
#include <deque>

#include "../../GameObjects/GrupoClientes.h"
#include "../../GameObjects/Player.h"

class Game;

class Mesa : public Mueble
{
	int mWidth, mHeight;

	GrupoClientes* mGrupo;

	std::vector<Silla*> sillas;
	deque<Paella*> paellas;
	int nSillas;

public:
	Mesa(Game* game, Vector2D<double> pos, Vector2D<int> dim, string texture);
	~Mesa() = default;

	void init(ObjectManager* objectManager) override;

	bool receiveGrupoClientes(GrupoClientes* gc) override;
	bool receivePaella(Paella* paella) override;
	bool returnObject(Player* p) override;

	bool colisionPlayer(Player* p) override;

	void clienteSeVa();

	void cambiaTexturaPaellas(string clave, Contenido contenido);

};

