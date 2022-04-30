#pragma once
#include <vector>
#include "../Utils/Texture.h"
#include "../GameObjects/GameObject.h"
#include "../GameObjects/UI/UiButton.h"
#include <string>
#include <list>
#define NK_IMPLEMENTATION
class Paella;
class Ventanilla;
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
    Comanda(Game* game, uint escala, UIManager* uim,bool enVentanilla_);//crear comanda
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

    void eC();

    void guardaTeclado();
    void guardaBoton(UiButton* b);
    void guardaTecladonum(vector <UiButton*> n);
    void guardaTecladotam(vector <UiButton*> t);
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
    virtual bool onClick(int mx, int my, bool& exit) override;
    bool  OnClick(int mx, int my);
        Comanda *seleccionaComanda();
    void deseleccionaComanda();
    void pressSelectedButton();
    void cambiazonafoco();
    void siguientebotonfocus(int dir);
    void update() override;
    void render(SDL_Rect* cameraRect) override;
    void animUpdate();
    void playAnim() { animPlay = true; };
    void setActiveTeclado(vector<UiButton*> a);
    vector<UiButton*> getTecladoing() { return teclado; };
    void setTexturecoords(int nx, int ny);

private:
    vector<Point2D<double> >posicionesBotones;//vas seleccionando y vas borrando las ya usadas
    int escale = 1;//esta mas difisil de lo que pensaba mmh
    int x;
    int tx;//coordenada para la textura, necesario para spritesheets
    int iniy;
    int y;
    int escritoX;
    int escritoY;
    int w;
    int h;
    int margenbotones = 5;
    int anchobotones = 36;
    double margenizquierdo;
    double margensuperior;
    int ancho = 120;
    int alto = 230;
    int altoini;
    int maxingrendientes=3;
    int maxpaellas = 4;
    bool enVentanilla = false;
    uint numeroMesa;
    uint numeroPaellas = 0;
    list<Comanda*>::iterator sitiolista;

    SDL_Rect clip;
    int frameCounter;
    float lastFrameTime;
    float frameRate;
    bool animPlay;

    EliminaComandaButton* eliminarboton;
    float puntuacion;
    UIManager* uiManager;
    vector<UiButton*> Pedido;//lo que mostramos en la ui  de lo que vas añadiendo a la comanda
    vector<UiButton*> teclado;
    vector <UiButton*> tecladonum;
    vector <UiButton*> tecladotam;
    vector<UiButton*> botones;
    UiButton* numeromesa;
    vector<Point2D<double>> postecladoini;
    vector<vector <UiButton* >> paellas; // esto es lo que se envia a la lista de comandas y vamos haciendo paellas[i,j.pushback(]pedido[i])
    UiButton* focusedbutton = nullptr;
    vector<UiButton*> activeTeclado = {};
    int indexfocus=-1;
    int focusedzone = 1; // 0 teclado 1 uicomanda
    void changeActiveTeclado();
    bool comparaX(UiButton u1, UiButton u2);
    bool comparaY(UiButton u1, UiButton u2);
    //float calculaPuntuacion(Paella paella) ; definicion pendiente de la creacion e paella
   
};
