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
class Herramienta;
class ParticleExample;
class Tutorial;

class Mueble : public GameObject
{
protected:
    double rellenoTimer = 0.0;

    int timerDimension = 50;

    Texture* timerTexture;

    int id;
    bool funcionando;
    int roto = 20;
    int randomOptions = 100;
    ParticleExample* humo;


public:
	Mueble(Game* mGame, Vector2D<double> position, int width, int height, string claveTextura);
    ~Mueble() { delete humo; };


    virtual bool receiveIngrediente(Ingrediente* ingr) { return false; }
    virtual bool receiveGrupoClientes(GrupoClientes* gc) { return false; }
    virtual bool receivePaella(Paella* pa) { return false; }
    virtual bool receiveArroz(Arroz* arr) { return false;}
    virtual bool receiveHerramienta(Herramienta* h);
    virtual bool testMueble();
    virtual bool resetCounter() { return false; }

    virtual bool returnObject(Player* p) { return false; }

    virtual Silla* initMesa(Mesa* mesa) { return nullptr;}
    virtual void decirPedido() {};

    SDL_Rect getCollider() override;
    SDL_Rect getOverlap() override;   
    virtual void setOverlap(SDL_Rect r) {};
    virtual void setCollider(SDL_Rect r) {};

    virtual void setId(int newId) { id = newId; }
    virtual int getId() { return id; }

    void romperMueble();
};



