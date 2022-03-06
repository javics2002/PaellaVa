#include "Comanda.h"
#include "../GameObjects/UI/Ingredientebutton.h"
#include "../Control/Game.h"
#include "../Control/ObjectManager.h"
#include "../GameObjects/UI/UIManager.h"
Comanda::Comanda(Game* game, uint nmesa,UIManager* uim) :GameObject(game)
{

    setTexture("cuadernillo");
    Vector2D<double> p;
    p.setX(100);
    p.setY(110);
    setPosition(p);
    setDimension(ancho, alto);
    gamet = game;
    uimt = uim;
 //   objectmanager = gamet->getObjectManager();
    double ix = p.getX() / 2 + margenbotones + anchobotones / 2;
    double iy = p.getY() / 2 + 2*anchobotones;
    escritoX = ix;
    escritoY = iy - anchobotones - margenbotones;
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
        alto = alto + anchobotones / 2 + 2*margenbotones;
        setDimension(ancho, alto);
        setPosition(getPosition().getX(), getPosition().getY() + 2*margenbotones);
        vector<Point2D<double>> sangria = uimt->getPosTeclado();
        for (int i=  0; i<sangria.size();i++)
        {
            int ny =sangria[i].getY() + anchobotones*0.7f;
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
        i->render();
    }
}