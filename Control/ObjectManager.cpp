#include "ObjectManager.h"
#include "../../GameObjects/Muebles/InicioCinta.h"
#include "../../GameObjects/Muebles/FinalCinta.h"
#include  "../UiButton.h"
#include "../../Utils/Vector2D.h"


ObjectManager::ObjectManager(Game* game)
{
	ingredientes = new Pool<Ingrediente>(game, 3);

	muebles.push_back(new InicioCinta(game, ingredientes));
	muebles.push_back(new FinalCinta(game, ingredientes));
	interfaz.push_back(new UiButton(game, botonredacta,10,10,30,30));
}

ObjectManager::~ObjectManager()
{
	for (auto i : muebles) {
		delete i;
		i = nullptr;
	}

	delete ingredientes;
}

void ObjectManager::render()
{
	for (auto i : muebles)
		i->draw();
	for (auto j : interfaz)
	{ 
		j->draw();
	}

	ingredientes->render();	
}

void ObjectManager::debug()
{
	for (auto i : muebles)
		i->drawDebug();

	ingredientes->debug();
}

void ObjectManager::update()
{
	for (auto i : muebles)
		i->update();

	ingredientes->update();	
}

pair<TextureName, int> ObjectManager::getRandomIngridient()
{
	int n = rand() % (texturasIngredienes.size());

	return { texturasIngredienes[n], n};
}
void ObjectManager::Uievent(int mx, int my)
{
	for (auto i : interfaz)
	{
		i->OnClick(mx, my);
	}
}


