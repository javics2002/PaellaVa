#pragma once
#include "../GameObject.h"

constexpr auto TILE_SIZE = 64;

class Game;
class Paella;
class Ingrediente;
class GrupoClientes;
class Player;
class Arroz;
class Mesa;
class Silla;
class PoolObject;

class Mueble : public GameObject
{
protected:
    double rellenoTimer = 0.0;

    int timerDimension = 50;

    Texture* timerTexture;

public:
	Mueble(Game* game, Vector2D<double> position, int width, int height, string claveTextura);
	~Mueble() = default;

    virtual bool receiveIngrediente(Ingrediente* ingr) { return false; }
    virtual bool receiveGrupoClientes(GrupoClientes* gc) { return false; }
    virtual bool receivePaella(Paella* pa) { return false; }
    virtual bool receiveArroz(Arroz* arr) { return false;}

    virtual bool returnObject(Player* p) { return false; }

    virtual Silla* initMesa(Mesa* mesa) { return nullptr;}
    virtual void decirPedido() {};

    SDL_Rect getCollider() override;
    SDL_Rect getOverlap() override;   
    virtual void setOverlap(SDL_Rect r) {};
    virtual void setCollider(SDL_Rect r) {};
};



