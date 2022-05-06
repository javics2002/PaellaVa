#include "Ingredientebutton.h"
#include "../UI/UIManager.h"

IngredienteButton::IngredienteButton(UIManager* uim, Game* mGame, string texturename, int x, int y, int w, int h,int id) : UiButton(mGame, texturename, x, y, w, h)
{
	this->uim = uim;
	idonline = id;
}
IngredienteButton::~IngredienteButton()
{
}
void IngredienteButton::execute(bool& exit)
{
	uim->getComanda()->añadiraPedido(textura,idonline);
	uim->randomizaTeclado();

	//Sonido de escribir aleatorio
	sdlutils().soundEffects().at("escribir" + std::to_string(sdlutils().rand().nextInt(2, 5))).play(0, mGame->UI);

	//comandat->añadiraPedido(textuname);
	//HAY QUE CORTAR EL BUCLE DE INTERAZ AQUI POR QEU SI NO SE AÑADEN 6 INGREDIENTES AL PULDSAR UN BOTON XD
	//well podria ser un while en ligar d eun for
}
void IngredienteButton::update()
{
}
