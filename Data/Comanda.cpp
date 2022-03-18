#include "Comanda.h"
#include "../GameObjects/UI/Ingredientebutton.h"
#include "../Control/Game.h"
#include "../Control/ObjectManager.h"
#include "../GameObjects/UI/UIManager.h"
#include "ListaComandas.h"
Comanda::Comanda(Game* game, uint nmesa, UIManager* uim) :GameObject(game)
{

    setTexture("cuadernillo");
    Vector2D<double> p;
    x = 100;
    y = 110;
    p.setX(x);
    p.setY(y);
    setPosition(p);
    setDimension(ancho, alto);
    altoini = alto;
    iniy = p.getY();
    gamet = game;
    uimt = uim;
    //   objectmanager = gamet->getObjectManager();
    double ix = p.getX() / 2 + margenbotones + anchobotones / 2;
    double iy = p.getY() / 2 + 2 * anchobotones;
    margenizquierdo = ix;
    margensuperior = iy;
    escritoX = ix;
    escritoY = iy - anchobotones - margenbotones;
    //teclado inicial igualq eu lso magenes y eso para resetear la comanda bien
     //creamos las psiciones de los botones del teclado
    /* for (int i = 0; i < 9; i++)
     {
         posicionesBotones.push_back(Point2D<double>(ix, iy));
         ix += anchobotones + margenbotones;
         if (ix >= p.getX() + getWidth() / 2 - anchobotones / 2)
         {
             ix = p.getX() / 2 + margenbotones + anchobotones / 2;
             iy += anchobotones + margenbotones;
         }


     }
     //ahora que tenemos las posiciones creamos el teclado;
     int j = 0;
     for (auto i : texturasIngredienes)
     {
         //Comanda comanda,Game* game, TextureName texturename, int x, int y, int w, int h
         Ingredientebutton* a = new Ingredientebutton(this, gamet, i, (int)posicionesBotones[j].getX(), (int)posicionesBotones[j].getY(), anchobotones, anchobotones);
         teclado.push_back(a);
      //  objectmanager->creaTeclado(a);

         j++;
     }*/
}
Comanda::Comanda( Comanda& c) :GameObject(c.gamet)
{
    setTexture("cuadernillo");
    setPosition(c.x, c.y);
     paellas = c.copyPaellas();
    gamet = c.gamet;
    setDimension(c.ancho, c.alto);
    numeroPaellas = c.numeroPaellas;
    for (int i = 0; i < c.numeroPaellas;i++)
    {
        paellas.push_back(c.paellas[i]);
      /*  for (int j = 0; j < paellas[i].size(); j++)
        {
            paellas[i].push_back(c.paellas[i][j]);
        }*/
    }
}
Comanda::~Comanda()
{
}
void Comanda::añadiraPedido(string i)
{

    UiButton* a = new UiButton(gamet, i, escritoX, escritoY, anchobotones / 2, anchobotones / 2);
    escritoX += anchobotones / 2 + margenbotones;
    //gamet->getObjectManager()->creaTeclado(a);
    Pedido.push_back(a);
    if (Pedido.size() % 4 == 0)
    {
        escritoY += anchobotones / 2 + margenbotones;
        escritoX = getPosition().getX() / 2 + margenbotones + anchobotones / 2;
        alto = alto + anchobotones / 2 + 2 * margenbotones;
        setDimension(ancho, alto);
        setPosition(getPosition().getX(), getPosition().getY() + 2 * margenbotones);
        vector<Point2D<double>> sangria = uimt->getPosTeclado();
        for (int i = 0; i < sangria.size(); i++)
        {
            int ny = sangria[i].getY() + anchobotones * 0.7f;
            sangria[i].setY(ny);
            //en algun lugar vuelven a tener el valor default lo tengo que mirar
            //bajar teclado
            //lo bajará en uim?
        }
        uimt->setPosTeclado(sangria);
    }
    randomizaIconos();

}
void Comanda::randomizaIconos()
{
    vector<Point2D<double>> posdis = uimt->getPosTeclado();
    int j = rand() % posdis.size();
    for (auto i : teclado)
    {
        i->setPosition(posdis[j]);
        posdis.erase(posdis.begin() + j);
        if (posdis.size() > 0)
            j = rand() % posdis.size();
    }

}
void Comanda::dibujaPedido()
{
    for (auto i : Pedido)
    {
        i->render(nullptr);
    }
    renderizaPaellas();
}
void Comanda::borraPedido()
{//FALTA SUBIR EL TECLADO DE BOTONES AL BORRAR
    bool saltolinea = Pedido.size() % 4 == 0;//por si acaso el metodo que escribe ya hace saltos de liena se pueden anular en algunos casos
    if (Pedido.size() > 0)
    {

        Pedido.erase(Pedido.begin() + Pedido.size() - 1, Pedido.begin() + Pedido.size());
        escritoX -= anchobotones / 2 + margenbotones;
        if (escritoX < margenizquierdo)
        {
            if (saltolinea)
            {
                escritoY -= anchobotones / 2 + margenbotones;
                escritoX = margenizquierdo + 3 * (anchobotones / 2 + margenbotones);
                vector<Point2D<double>> sangria = uimt->getPosTeclado();
                for (int i = 0; i < sangria.size(); i++)
                {
                    int ny = sangria[i].getY() - anchobotones * 0.7f;
                    sangria[i].setY(ny);
                    //en algun lugar vuelven a tener el valor default lo tengo que mirar
                    //bajar teclado
                    //lo bajará en uim?
                }
                uimt->setPosTeclado(sangria);
                uimt->randomizaTeclado();
            }
            else escritoX = margenbotones;
            //siendo 4 el numero maximo de iconos por linea
        }
    }
}
void Comanda::cancelaPedido()
{
    Pedido.clear();
    escritoX = margenizquierdo;
    escritoY = margensuperior;
    alto = altoini;
    setDimension(ancho, altoini);
    setPosition(getPosition().getX(), iniy);
    uimt->setPosTeclado(postecladoini);
    uimt->randomizaTeclado();
    clearPaellas();
    numeroPaellas = 0;


}
void Comanda::guardaTeclado()
{
    vector<Point2D<double>> sangria = uimt->getPosTeclado();
    for (int i = 0; i < sangria.size(); i++)
    {
        int ny = sangria[i].getY() + anchobotones * 0.7f;
        sangria[i].setY(ny);
        //en algun lugar vuelven a tener el valor default lo tengo que mirar
        //bajar teclado
        //lo bajará en uim?
    }
    //uimt->setPosTeclado(sangria);
    postecladoini = sangria;
    //pero al inicial le falta la primera sangria y queda por encima de la primera linea de pedido D:
   // de momento voy a forzar una sangria aqui s tnego tiempo mirare una manera mejor xd

}
void Comanda::aceptaPaella()
{
    //aqui esta lo dificil el vector de la paella que envias ya no lo podras editar pero deberia seguir siendo visible 
    //, tendra que mover margenes y vaciar el vector de pedido y que haya un render paellas , lo dificil va  a ser que 
    //se renderice otdo guay
    if (!Pedido.empty())
    {
        paellas.push_back(vector<UiButton*>());
        for (int j = 0; j < Pedido.size(); j++)
        {
            paellas[numeroPaellas].push_back(Pedido[j]);
          //  string s = Pedido[j]->getTextura();

            //paellas[numeroPaellas][j].push_back(*s.c_str()); //el vector qeuire chars raros por algun motivo
            ///paellas[numeroPaellas].push_back(s);
            //esta explotando ahi por algun motivo//el motivo : no se leer
        }
        Pedido.erase(Pedido.begin(), Pedido.begin() + Pedido.size());
        Pedido.clear();
        numeroPaellas++;
    }
}
void Comanda::enviaComanda()
{
 /*   if (numeroPaellas > 0)
    {
        


    }*/
    uimt->getBarra()->AñadeComanda(this);
    cancelaPedido();

}
void Comanda::renderizaPaellas()
{

    if (numeroPaellas > 0)
    {
        for (int i = 0; i < numeroPaellas; i++)
        {

            for (int j = 0; j < paellas[i].size(); j++)
            {
                paellas[i][j]->render(nullptr);//los strings de las texturas no llegan y se muere
            }
        }



    }

}
void Comanda::desplazacomandas(int d)
{

    if (numeroPaellas > 0)
    {
        for (int i = 0; i < numeroPaellas; i++)
        {

            for (int j = 0; j < paellas[i].size(); j++)
            {
                paellas[i][j]->setPosition(paellas[i][j]->getPosition().getX()+d,paellas[i][j]->getPosition().getY());
            }
        }



    }

}
void Comanda::clearPaellas()
{
    if (numeroPaellas > 0)
    {
        for (int i = 0; i < numeroPaellas; i++)
        {

            for (int j = 0; j < paellas[i].size(); j++)
            {
                delete paellas[i][j]; paellas[i][j] = nullptr;
            }
            paellas[i].clear();
        }

        paellas.clear();

    }
    numeroPaellas = 0;
}
vector<vector<UiButton*>> Comanda::copyPaellas()
{
    vector<vector <UiButton*>> paellascopiadas;
    for (int i = 0; i < paellas.size(); i++)
    {
        paellascopiadas.push_back( vector<UiButton*>());

        for (int j = 0; j < paellas[i].size(); j++)
        {
            UiButton u = *paellas[i][j];
            UiButton* up = new UiButton(gamet,u.getTextura(),u.getPosition().getX(),u.getPosition().getY(),u.getWidth(),u.getHeight());
            paellascopiadas[i].push_back(up);

        }
    }
    return paellascopiadas;
}