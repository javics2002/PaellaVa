#pragma once
#include <vector>
#include "../Utils/Texture.h"
#include "../GameObjects/GameObject.h"
#include "../GameObjects/UI/UiButton.h"
#define NK_IMPLEMENTATION
class Paella;
class Ingrediente;
class Game;
class Ingredientebutton;
using namespace std;
using uint = uint32_t;
using Iconos = char;
class Comanda :public GameObject
{
    //al añadir paella se alarga un poco la comanda
    //hay 8 ingredientes distintos
public:
    enum Tamaño { Pequeña, Mediana, Grande };
    Comanda(Game* game, uint nmesa);//crear comanda

    ~Comanda();//No se puede definir todavia y no compila NO LO BORREIS/
    void añadiraPedido(string i);
    void randomizaIconos();
    //void abrirComanda(int x,int y);
    void dibujaPedido();
    /*void cierraComanda();*/
    //void Ecribeinfoenjson();
    //Compara las paellas recibidas con el pedido y le pone la puntuación
    //float Puntuacion(vector<Paella> paellas);
private:
    vector<Point2D<double> >posicionesBotones;//vas seleccionando y vas borrando las ya usadas
    vector<string> texturasIngredienes = { "alcachofa", "berenjena", "calamar", "cangrejo", "gamba",
        "langosta", "mejillon", "pimientoRojo", "pimientoVerde", "pollo"};
    int x;
    int y;
    int escritoX;
    int escritoY;
    int w;
    int h;
    int margenbotones=5;
    int anchobotones=25;
    int ancho = 110;
    int alto = 220;
    uint numeroMesa;
    uint numeroPaellas;
    float puntuacion;
    Game* gamet;
   // ObjectManager* objectmanager;
    //Texture textura;
    //Por cada paella, un tamaño y un vector de ingredientes
    //vector<Tamaño> tamaños;
    //vector<vector<Ingrediente>> ingredientes;
    //vector<Paella> paellas;//datos de la paella recogida en la comanda con la que se calcula la puntuacion
    vector<UiButton*> Pedido;//lo que mostramos en la ui  de lo que vas añadiendo a la comanda
    vector<Ingredientebutton*> teclado;

    //float calculaPuntuacion(Paella paella) ; definicion pendiente de la creacion e paella
    void update() override{};
};

