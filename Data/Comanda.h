#pragma once
#include <vector>
#include "../Utils/Texture.h"
#include "../GameObjects/GameObject.h"
#include "../GameObjects/UI/UiButton.h"
#include <string>
#include <list>
#define NK_IMPLEMENTATION
class Paella;
class Ingrediente;
class Game;
class IngredienteButton;
class NumeroButton;
class TamanoButton;
class EliminaComandaButton;
class UIManager;
using namespace std;
using uint = uint32_t;
using Iconos = char;
class Comanda :public GameObject
{
    //al añadir paella se alarga un poco la comanda
    //hay 8 ingredientes distintos
public:
    enum Tamaño { Pequeña, Mediana, Grande };
    Comanda(Game* game, uint nmesa, UIManager* uim);//crear comanda
    Comanda( Comanda& c);//NECESARIO PARA QUE LA LIST DE COMANDAS FUNCIONE

    ~Comanda();//No se puede definir todavia y no compila NO LO BORREIS/
    void añadiraPedido(string i);
    void anadirNumeromesa(string n);
    UiButton* getNumeromesa();
    void randomizaIconos();
    //void abrirComanda(int x,int y);
    void dibujaPedido();
    void borraPedido();
    void cancelaPedido();
    void aceptaPaella();
    void enviaComanda();

    void guardaTeclado();
    void guardaBoton(UiButton* b);
    void guardaTecladonum(vector <NumeroButton*> n);
    void guardaTecladotam(vector <TamanoButton*> t);
    void toggleTeclado(bool b);
    void toggleTecladonum(bool b);
    void toggleTecaldotam(bool b);


    void renderizaPaellas();
    void desplazacomandas(int d);
    void clearPaellas();
    vector<vector<UiButton*>> copyPaellas();

    void setSitio(list<Comanda*>::iterator  s);
    list<Comanda*>::iterator  getSitio();

    void setEliminabutton(EliminaComandaButton* e) { eliminarboton = e; };
    EliminaComandaButton* getEliminabutton() { return eliminarboton; };
    void toggleactive();

    bool  OnClick(int mx, int my);
        Comanda *seleccionaComanda();
    void deseleccionaComanda();

private:
    vector<Point2D<double> >posicionesBotones;//vas seleccionando y vas borrando las ya usadas
    int x;
    int iniy;
    int y;
    int escritoX;
    int escritoY;
    int w;
    int h;
    int margenbotones = 5;
    int anchobotones = 25;
    double margenizquierdo;
    double margensuperior;
    int ancho = 110;
    int alto = 220;
    int altoini;
    uint numeroMesa;
    uint numeroPaellas = 0;
    list<Comanda*>::iterator sitiolista;

    EliminaComandaButton* eliminarboton;
    float puntuacion;
    UIManager* uiManager;
    vector<UiButton*> Pedido;//lo que mostramos en la ui  de lo que vas añadiendo a la comanda
    vector<IngredienteButton*> teclado;
    vector <NumeroButton*> tecladonum;
    vector <TamanoButton*> tecladotam;
    vector<UiButton*> botones;
    UiButton* numeromesa;
    vector<Point2D<double>> postecladoini;
    vector<vector <UiButton* >> paellas; // esto es lo que se envia a la lista de comandas y vamos haciendo paellas[i,j.pushback(]pedido[i])

    //float calculaPuntuacion(Paella paella) ; definicion pendiente de la creacion e paella
    void update() override {};
};