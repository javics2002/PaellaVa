#pragma once


#include "../ecs/Component.h"
#include <string>
#include <tmxlite\Types.hpp>
#include "../sdlutils/Texture.h"
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

#include "../game/Game.h"
#include "../ecs/Manager.h"
#include "../ecs/tile.h"

#include "../utils/Collisions.h"
#include <vector>
#include <queue>



using namespace std;
class Game;

const int TAM_CELDA = 64;

class MapMngr : public Component
{
private:
	tmx::Vector2u mapDimensions_;	//Guarda las dimensiones del mapa
	tmx::Vector2u tilesDimensions_;	//Guarda las dimensiones de las tiles
	Texture* tilesetsArr[1];
	tmx::Map map_;

	//Matriz de colisiones
	bool** collider;

	Vector2D mapCoorsToSDLPoint(Vector2D coords);
	Vector2D SDLPointToMapCoords(Vector2D p);
	tmx::ObjectGroup* objectLayer;		//Guardamos la capa de entidades para poder leer los enemigos
	int Room;							//Guarda la habitaci�n en la que se encuentran los jugadores
	int numberEnemyRoom;				//Guarda la cantidad de enemigos que hay por sala
	int RoundsPerRoom;					//Guarda la cantidad de rondas que hay en una sala
	int RoundsCount = 0;
	float mapHeight_ = 0;
	bool collisionCreated = false;
	float scale = 3; // 2.4 tren
	bool BossControlSpawn = false;

	std::queue<tmx::Object> triggerFtCamera;
	vector<string> hamstersToLoad_;

	void addHamster(string name, int i,const tmx::Object& object);
	void addParaxall(int lvl, bool front, bool train, int v1, int v2, int v3, int v4);
public:
	MapMngr() : Room(0) {};
	~MapMngr();

	void loadNewMap(string map);
	void update()override;
	Camera* cam;
	bool intersectWall(const SDL_Rect& hamster);
	bool intersectObstacles(const SDL_Rect& hamster);
	bool intersectBoss(const SDL_Rect& hamster);
	bool intersectFinalBoss(const SDL_Rect& hamster);

	inline float getScale() const { return scale; };
	inline int getCellSize() const { return TAM_CELDA; };
	inline float getMaxH() const { return mapHeight_; };
	inline int getMaxW() const { return mapDimensions_.x * TAM_CELDA * scale; };

	inline void setMaxH(float i) { mapHeight_ = i; };

	void loadEnemyRoom();
	inline void reduceNumberEnemyRoom() { numberEnemyRoom--; };
	void newSceneTrigger(string newName, const tmx::Object& obj);
	void startChaseTrigger(const tmx::Object& obj);
	void startMusic(const tmx::Object& obj);

	inline void addHamster(string name) { hamstersToLoad_.push_back(name); };
	inline void removeHamster() { hamstersToLoad_.pop_back(); };
	inline void clearHamstersVector() { hamstersToLoad_.clear(); };

	void addObject(const tmx::Object& object);
	void addTrap(const tmx::Object& object, int x, int y);
	void resetTriggerList();

	void clearColliders();
};

