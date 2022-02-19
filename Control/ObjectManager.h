#pragma once
#include "../Data/Pool.h"
#include "../GameObjects/Ingrediente.h"
#include "../GameObjects/Cliente.h"
#include "../GameObjects/GrupoClientes.h"
#include <queue>
#include <vector>


///class Ingrediente;
//class GrupoClientes;
//class Paella;
class Game;

using namespace std;

class ObjectManager
{
	//Vector de paelleras
	// 
	//vector<Paella*> paelleras;

	//Pool de clientes
	// 
	Pool<Cliente> *clientes;
	Pool<GrupoClientes> *grupoClientes;


	//Pool de ingredientes
	
	Pool<Ingrediente> *ingredientes;
	vector<TextureName> texturasIngredienes = { ingrAlcachofa, ingrCalamar, ingrCangrejo, 
		ingrGamba, ingrLangosta, ingrMejillon, ingrPimientoRojo, ingrPimientoVerde, ingrPollo
	};

	vector<GameObject*> muebles;
	vector<GameObject*> interfaz;
	vector<GameObject*> comandas;//esto va a ser nuestra version de capas 
	//en el orden en elq ue lo renderizamos nos aseguramos que la ui esta por encima de todo
public:
	ObjectManager(Game* game);
	~ObjectManager();

	void Render();
	void Debug();
	void Update();

	pair<TextureName, int> GetRandomIngridient();
	void Uievent(int mx,int my);
	void creaComanda(Game*game);

	vector<Collider*> getClientes(SDL_Rect gOC);
};

