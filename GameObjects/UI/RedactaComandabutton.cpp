#include "RedactaComandabutton.h"
#include "../../Control/game.h"
#include "../../Control/ObjectManager.h";
#include "../../GameObjects/UI/UIManager.h"

RedactaComandabutton::RedactaComandabutton(Game* mGame, UIManager* uiManager, string claveTextura, int x, int y, int w, int h) : UiButton(mGame, claveTextura, x, y, w, h)
{
	uim = uiManager;
	
}

RedactaComandabutton::~RedactaComandabutton()
{
}
void RedactaComandabutton::seteabutton()
{
	uim->setRedactaboton(this);
}
void RedactaComandabutton::execute(bool& exit)
{
	if (uim->getComanda() == nullptr)
	{
		sdlutils().soundEffects().at("sacarComandas").play(0, mGame->UI);
		uim->creaComanda(mGame);

	}
	else
	{
		uim->getComanda()->toggleactive();
	}
//	Swichposition();
}

void RedactaComandabutton::Swichposition()
{
	if (getPosition().getY() < 100)
	{
		setPosition(100, 260);
		setDimension(130, 60);
		setTexture("redactabotonup");
	}
	else
	{
		setPosition(100, 20);
		setDimension(90,40);
		setTexture("redactaboton");
	}
}
void RedactaComandabutton::goDown()
{
	setPosition(100, getPosition().getY() + 30);
}
void RedactaComandabutton::resetPosition()
{
	setPosition(100, 260);

}