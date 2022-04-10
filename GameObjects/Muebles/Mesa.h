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

	pair<Vector2D<double>, bool> tiles[2][2];


	void setTiles();
	Vector2D<double> getProxPos(Vector2D<double> pos);
	Vector2D<double> getCenterMesa();

public:
	Mesa(Game* game, Vector2D<double> pos, Vector2D<int> dim, Vector2D<int> tiles, string texture);
	~Mesa() = default;

	void init(ObjectManager* objectManager) override;

	bool receiveGrupoClientes(GrupoClientes* gc) override;
	bool receivePaella(Paella* paella) override;
	bool returnObject(Player* p) override;


	void clienteSeVa();

	vector<Paella*> getPaellasEntregadas();

	void comerPaellas();

	SDL_Rect getOverlap() override;

	void decirPedido() override;
};

