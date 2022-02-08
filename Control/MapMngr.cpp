#include "MapMngr.h"
#include "../sdlutils/SDLUtils.h"

#include "../components/Transform.h"
#include "../components/HamsterStateMachine.h"
#include "../components/Image.h"
#include "../components/Movement.h"
#include "../components/LightAttack.h"
#include "../components/StrongAttack.h"
#include "../components/Stroke.h"
#include "../components/UI.h"
#include "../components/Animator.h"
#include "../components/Roll.h"
#include "../components/Poison.h"
#include "../components/Pray.h"
#include "../components/WarCry.h"
#include "../components/Combos.h"
#include "../components/Turret.h"
#include "../components/MovementSimple.h"
#include "../components/EnemyAttack.h"
#include "../components/EnemyStateMachine.h"
#include "../components/MapMngr.h"
#include "../components/ControlHandler.h"
#include "../components/Item.h"
#include "../components/GetItem.h"
#include "../components/EnemyStun.h"
#include "../components/Stun.h"
#include "../components/Gravity.h"
#include "../components/Knockback.h"
#include "../components/HeartUI.h"
#include "../components/PossesionGame.h"
#include "../components/GhostCtrl.h"
#include "../components/ContactDamage.h"
#include "../components/HeightObject.h"
#include "../components//AnimHamsterStateMachine.h"
#include "../components/Swallow.h"
#include "../components/TriggerMusic.h"
#include "../ecs/Camera.h"
#include "../components/EnemyBehaviour.h"
#include "../components/IddleEnemy.h"
#include "../components/FollowPlayer.h"
#include "../components/AmbushPlayer.h"
#include "../components/FleeFromPlayer.h"
#include "../components/FirstBossBehaviour.h"
#include "../components/FirstBossAttack.h"
#include "../components/EnemyStrongAttack.h"
#include "../components/StrongFollowPlayer.h"
#include "../components/BackGround.h"
#include "../components/FairStrokeStrategy.h"
#include "../components/RandomStrokeStrategy.h"
#include "../components/Parallax.h"
#include "../components/CollisionDetec.h"
#include "../components/Shadow.h"
#include "../components/EnemyMother.h"
#include "../components/TriggerScene.h"
#include "../components/AnimEnemyStateMachine.h"
#include "../components/CatMovement.h"
#include "../components/StartChase.h"
#include "../components/Obstacle.h"
#include "../components/FinalBossManager.h"
#include "../components/TimeTrap.h"
#include "../components/MicroOndasManager.h"
#include "../components/dialogos.h"
#include "../components/ObstacleMoveable.h"
#include "../components/LifeTime.h"




MapMngr::~MapMngr() {
	clearColliders();
}

void MapMngr::update() {

	if (!Mix_PlayingMusic() && BossControlSpawn) {
		loadEnemyRoom();
		BossControlSpawn = false;
	}

	//cout << numberEnemyRoom << " " << entity_->getMngr()->getEnemies().size() <<endl;
	auto* camera = entity_->getMngr()->getHandler<Camera__>()->getComponent<Camera>();
	//	Comprobamos la colision con los triggers salas
	tmx::Object trigger;
	auto& players = entity_->getMngr()->getPlayers();
	if (!triggerFtCamera.empty())
		trigger = triggerFtCamera.front(); //Recorrer triggers

	auto& getProp = trigger.getProperties();
	for (Entity* player : players) {
		auto* pTr = player->getComponent<Transform>();
		if (player->getComponent<HamsterStateMachine>()->getState() != HamStates::INFARCTED && Collisions::collides(pTr->getPos(), pTr->getW(), pTr->getH(), Vector2D(trigger.getPosition().x, trigger.getPosition().y) * scale, trigger.getAABB().width * scale, trigger.getAABB().height * scale)) {
			RoundsPerRoom = getProp[1].getIntValue();

			if (!getProp[2].getBoolValue()) {
				loadEnemyRoom();
			}
			else {
				BossControlSpawn = true;
				Mix_FadeOutMusic(3000);
			}

			if (getProp[0].getIntValue() != -1) {
				camera->setGoToTracker(true);
				camera->changeCamFollowPos(getProp[0].getIntValue() * scale);
				camera->changeCamState(State::GoingTo);
			}
			//Borrar el punto de la camara del vector
			triggerFtCamera.pop();

			//TUTORIAL
			if (!sdlutils().tutorialDone() && stoi(trigger.getName()) < 4) {
				entity_->getMngr()->getHandler<dialogosMngr>()->getComponent<dialogos>()->dialogoStateChange();
			}
		}
	}

	//Si el estado de la camara es "Static" aka luchando con enemigos, y la cantidad de enemigos en la habitación es 0, volvemos a "Player1s"
	if (camera->getCamState() == State::Static && numberEnemyRoom <= 0) {
		numberEnemyRoom = 0;
		if (RoundsPerRoom == RoundsCount) {
			Room++;	//Una vez cargamos a los enemigos de la habitacion incrementamos el contador para poder cargar los enemigos de la siguiente
			RoundsCount = 0;
			camera->changeCamFollowPos(-1);	//Se pasa el punto medio de los jugadores
			camera->setGoToTracker(false);					//Se fija la transicion al punto medio de los jugadores al terminar GoTo
			camera->changeCamState(State::GoingTo);			//Se cambia el estado de la camara a GoTo
			//ha rerminado al sala de batalla
			auto wo = entity_->getMngr()->getWavesObjects();
			for (Entity* woe : wo) {
				woe->setActive(false);
			}
		}
		else {
			RoundsCount++;
			loadEnemyRoom();
		}
	}
}

void MapMngr::loadNewMap(string map) {
	cam = entity_->getMngr()->getHandler<Camera__>()->getComponent<Camera>();

	if (map_.load(map)) {
		if (map == "resources/images/tiled/Level1Boss.tmx") {
			scale = 2.407;
			entity_->getMngr()->getHandler<Camera__>()->getComponent<Camera>()->setcShake(true);
			entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->play("trainBackground");
		}
		else if (map == "resources/images/tiled/Level2.tmx") {
			scale = 3.0;
			entity_->getMngr()->getHandler<SoundManager>()->getComponent<SoundManager>()->play("birds");
		}

		mapHeight_ = map_.getProperties()[0].getIntValue() * TAM_CELDA * scale;

		mapDimensions_ = map_.getTileCount();
		const auto& layers = map_.getLayers();

		//Establecemos el tamaño de la matriz
		collider = new bool* [mapDimensions_.x];
		for (int i = 0; i < mapDimensions_.x; i++)
		{
			collider[i] = new bool[mapDimensions_.y]{ false };
		}
		//Hay colisiones
		collisionCreated = true;

		//Fondos
		if (map == "resources/images/tiled/Level1.tmx")
			addParaxall(1, true, false, 7, 10, 15, 10);
		else if (map == "resources/images/tiled/Level1Boss.tmx")
			addParaxall(1, false, true, 7, 10, 15, 0);
		else if (map == "resources/images/tiled/Level2.tmx")
			addParaxall(2, true, false, 7, 10, 1, 10);
		else if (map == "resources/images/tiled/Level3.tmx")
			addParaxall(3, false, false, 10, 0, 0, 0);

		//Dimensiones de los tiles
		tilesDimensions_ = map_.getTileSize();

		int i = 0;
		//Cargamos los tilesets y guardamos las texturas
		const auto& tilesets = map_.getTilesets();

		//Estblecemos el tamaño en funcion de los tilesets que tenga
		tilesetsArr[tilesets.size()];

		for (const auto& tileset : tilesets)
		{
			//Guardamos las texturas de los tilesets
			tilesetsArr[i] = &sdlutils().images().at(tileset.getName());	//El nombre del tileset en Tiled y la textura png DEBEN llamarse igual
			i++;
		}

		for (const auto& layer : layers)
		{
			if (layer->getType() == tmx::Layer::Type::Object)
			{
				const auto& objLayer = &layer->getLayerAs<tmx::ObjectGroup>();
				const auto& objects = objLayer->getObjects();
				if (layer->getName() == "Alturas") {
					for (const auto& object : objects)
					{
						auto* o = entity_->getMngr()->addMapHeight();
						o->addComponent<Transform>(Vector2D(object.getPosition().x * scale, object.getPosition().y * scale),
							Vector2D(), object.getAABB().width * scale, object.getAABB().height * scale, 0.0f, 1, 1);
						o->addComponent<HeightObject>()->setZ(stoi(object.getName()) * getCellSize() * scale);
					}
				}
				else if (layer->getName() == "Salas") {
					//Guardamos todos los triggers de cambio de sala
					for (auto object : objects)
					{
						triggerFtCamera.push(object);
					}
				}
				else if (layer->getName() == "entities") {
					//Guardamos la capa de objetos
					std::cout << layer->getName();

					objectLayer = &layer->getLayerAs<tmx::ObjectGroup>();

					//Boss nulo para evitar buscar colisiones con el
					entity_->getMngr()->setHandler<Boss>(nullptr);
					entity_->getMngr()->setHandler<FinalBoss>(nullptr);

					for (const auto& object : objects) {
						if (object.getName() == "spawnZone") {
							for (int i = 0; i < hamstersToLoad_.size(); ++i) {
								// Por si se generan mas de los que deberian
								if (i < MAXPLAYERS)
									addHamster(hamstersToLoad_[i], i, object);
							}
						}
						else if (object.getName() == "sardinilla" || object.getName() == "canelon" || object.getName() == "keta" || object.getName() == "monchi")
						{
						}//addHamster(object);
						else if (object.getName() == "newScene") {
							newSceneTrigger(object.getProperties()[0].getStringValue(), object);
						}
						else if (object.getName() == "startChase") {
							startChaseTrigger(object);
						}
						else if (object.getName() == "startMusic") {
							startMusic(object);
						}
						else if (object.getName() == "secondBoss") { //PROP[0] ES LA PROPIEDAD 0, EDITAR SI SE AÑADEN MAS
							auto* enemy = entity_->getMngr()->addTrap();
							enemy->addComponent<Transform>(
								Vector2D(object.getPosition().x * scale, object.getPosition().y * scale),
								Vector2D(), 300.0f * scale, 240.0f * scale, 0.0f, 0.4, 1);

							//Le dejamos durmiendo
							enemy->addComponent<Animator>(&sdlutils().images().at("cat"),
								440,
								350,
								3,
								3,
								220,
								Vector2D(),
								3)->play(sdlutils().anims().at("cat_sleeping"));

							//Le metemos gravedad
							enemy->getComponent<Transform>()->setGravity(enemy->addComponent<Gravity>());

							enemy->addComponent<CatMovement>();

							enemy->addComponent<EntityAttribs>();
							enemy->addComponent<ContactDamage>(30, 200, false, false, false);
							enemy->getMngr()->setHandler<Cat_>(enemy);
						}
						else if (object.getName() == "microondas") { //PROP[0] ES LA PROPIEDAD 0, EDITAR SI SE AÑADEN MAS
							auto* micro = entity_->getMngr()->addEntity();

							micro->addComponent<Transform>(
								Vector2D(object.getPosition().x * scale, object.getPosition().y * scale),
								Vector2D(), 256.0f, 2 * 256.0f, 0.0f, 1, 1);

							micro->addComponent<EntityAttribs>(900 + 150 * hamstersToLoad_.size(), 0.0, "soldier1", Vector2D(0, 0), 0, 0, 20, true, false, false);
							/*EntityAttribs(int life, float range, std::string id, Vector2D speed, int number, float poisonProb, int dmg, int marg);
							EntityAttribs(int life, float range, std::string id, Vector2D speed, int number, float poisonProb, int dmg, bool igMargin, bool invincibilty, bool canBPois);*/



							//enemy->addComponent<EntityAttribs>()->setIgnoreMargin(false);
							micro->addComponent<MicroOndasManager>(hamstersToLoad_.size(), &sdlutils().images().at("anaranjado"), &sdlutils().images().at("puntoDeLuz"));
							//enemy->addComponent<Image>(&sdlutils().images().at("catSmoking"));
							//enemy->addComponent<ContactDamage>(20, 30);
							//enemy->getMngr()->setHandler<Pussy>(enemy);
						}
						else if (object.getName() == "trap") {
							addTrap(object, object.getPosition().x, object.getPosition().y);
						}
						else if (object.getName() == "obstacle") {
							addObject(object);
						}
					}
				}
			}

			//SI ES UNA CAPA DE TILES
			int index = 0;	//Recorrer� los tilesets para saber a cual corresponde cada tile
			if (layer->getType() == tmx::Layer::Type::Tile)
			{
				const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
				//read out tile layer properties etc...
				for (int j = 0; j < mapDimensions_.y; j++) {
					for (int i = 0; i < mapDimensions_.x; i++) {

						//Guardamos el indice global del tile (nos servir� para saber en qu� tileset se encuentra)
						auto tileList = tileLayer.getTiles();
						auto globalIndexTile = tileList[i + j * mapDimensions_.x].ID;	//mapDimensions_.y+columna*elementos_enuna_fila

						//Necesitamos saber a cual de los tilesets pertenece esa posicion
						while (globalIndexTile > tilesets[index].getLastGID()) {
							index++;	//Marca la posicion del tileset al que pertenece el tile
						}

						//Calculamos la posicion del tile en la pantalla -> DestRect
						auto x = i * tilesDimensions_.x;
						auto y = j * tilesDimensions_.y;

						//Calculamos la posici�n del tile en el tileset -> SrcRect
						int tilesetSize = tilesets[index].getColumnCount();
						//Calculamos las coordenadas locales del tile
						//Hay que restar el valor del primer tile del tileset a la posicion global
						auto localIndexTile = globalIndexTile - tilesets[index].getFirstGID();

						auto Srcx = (localIndexTile % tilesets[index].getColumnCount()) * tilesDimensions_.x;
						auto Srcy = (localIndexTile / tilesets[index].getColumnCount()) * tilesDimensions_.y;

						//Sacamos el SDL_SrcRect y SDL_DestRect con el que imprimimos los tiles
						SDL_Rect src;
						src.x = Srcx; src.y = Srcy;
						src.w = src.h = tilesDimensions_.x;	//Las tiles son cuadradas


						SDL_Rect dest;
						dest.x = x * scale; dest.y = y * scale;
						dest.w = dest.h = tilesDimensions_.x * scale;


						/*Vector2D renderPos = Vector2D(dest.x - cam.x, dest.y - cam.y);
						dest.x = renderPos.getX();
						dest.y = renderPos.getY();*/

						if (globalIndexTile != 0 && index < tilesets.size()) {
							//ESTO ES EL NOMBRE DE LA LAYER QUE SE CREE SOLO DE LAS COLISIONES
							if (tileLayer.getName() == "Collision") {
								//Si el leemos la capa de colisiones ponemos la matriz a true
								collider[i][j] = true;
								Tile(entity_->getMngr(), src, dest, tilesetsArr[index], true);
							}
							else {
								//Si no es una capa de colisiones se renderiza sin más
								Tile(entity_->getMngr(), src, dest, tilesetsArr[index], false);
							}
						}
					}
				}
			}
		}

		//ENEMIGO
		//Una vez terminamos de cargar todas las entidades y tiles de las CAPAS, cargamos los enemigos de la sala 0
		//LoadEnemyRoom();
		Room = 1;
	}
}

void MapMngr::addParaxall(int lvl, bool front, bool train, int v1, int v2, int v3, int v4) {
	auto upH = mapHeight_ - cam->getCam().h + cam->getUpOffset() - 380;

	string l = "level" + to_string(lvl);

	auto* o = entity_->getMngr()->addBackGround();
	o->addComponent<Transform>(Vector2D(0, 0), Vector2D(0, 0), 1920, 1459, 0.0, 1, 1);
	//Para meter un fondo meter esto									velocidad		tamaño			posicion
	o->addComponent<Parallax>(&sdlutils().images().at(l + "background1"), v1, Vector2D(1920, 1839), Vector2D(0, upH), false, train);

	auto* p = entity_->getMngr()->addBackGround();
	p->addComponent<Transform>(Vector2D(0, 0), Vector2D(0, 0), 1920, 1459, 0.0, 1, 1);
	//Para meter un fondo meter esto									velocidad		tamaño			posicion
	p->addComponent<Parallax>(&sdlutils().images().at(l + "background2"), v2, Vector2D(1920, 1839), Vector2D(0, upH), false, train);

	auto* q = entity_->getMngr()->addBackGround();
	q->addComponent<Transform>(Vector2D(0, 0), Vector2D(0, 0), 1920, 1459, 0.0, 1, 1);
	//Para meter un fondo meter esto									velocidad		tamaño			posicion
	q->addComponent<Parallax>(&sdlutils().images().at(l + "background3"), v3, Vector2D(1920, 1839), Vector2D(0, upH), false, train);

	if (front) {
		auto* r = entity_->getMngr()->addFrontGround();
		r->addComponent<Transform>(Vector2D(0, 0), Vector2D(0, 0), 1920, 1459, 0.0, 1, 1);
		//Para meter un fondo meter esto									velocidad		tamaño			posicion
		r->addComponent<Parallax>(&sdlutils().images().at(l + "background4"), v4, Vector2D(1920, 1839), Vector2D(0, upH), true, train);
	}
}

//Devuelve true si se está chocando con alguna colision
bool MapMngr::intersectWall(const SDL_Rect& hamster) {

	//Cogemos arriba izquierda y abajo derecha
	Vector2D topLeftCoords = SDLPointToMapCoords(Vector2D((hamster.x) / scale, (hamster.y) / scale));
	Vector2D bottomRightCoords = SDLPointToMapCoords(Vector2D((hamster.x + hamster.w - 1) / scale, (hamster.y + hamster.h - 1) / scale));

	for (int x = topLeftCoords.getX(); x <= bottomRightCoords.getX(); x++) {
		for (int y = topLeftCoords.getY(); y <= bottomRightCoords.getY(); y++) {
			if (x < mapDimensions_.x && y < mapDimensions_.y && x >= 0 && y >= 0) {
				//Si hay una colision cercana...
				if (collider[x][y])
					//Se choca
					return true;
			}
		}
	}
	//Si no se choca con nada devuelve false
	return false;
}

bool MapMngr::intersectObstacles(const SDL_Rect& hamster) {
	auto& obstacles = entity_->getMngr()->getObstacles();
	bool collide = false;
	int i = 0;
	while (!collide && i < obstacles.size()) {
		auto obstacleRect = obstacles[i]->getComponent<Transform>()->getRectCollideFeet();
		collide = Collisions::collides(Vector2D(hamster.x, hamster.y), hamster.w, hamster.h,
			Vector2D(obstacleRect.x, obstacleRect.y), obstacleRect.w, obstacleRect.h);
		++i;
	}
	return collide;
}
bool MapMngr::intersectBoss(const SDL_Rect& hamster) {
	auto boss = entity_->getMngr()->getHandler<Boss>();

	bool collide = false;

	if (boss != nullptr && boss->isActive() && boss->getComponent<FirstBossAttack>()->getCollide()) {
		auto bossRect = boss->getComponent<Transform>()->getRectCollide();
		collide = Collisions::collides(Vector2D(hamster.x, hamster.y), hamster.w, hamster.h,
			Vector2D(bossRect.x, bossRect.y), bossRect.w, bossRect.h);
	}
	return collide;
}
bool MapMngr::intersectFinalBoss(const SDL_Rect& hamster) {
	auto boss = entity_->getMngr()->getHandler<FinalBoss>();

	bool collide = false;

	if (boss != nullptr && boss->isActive() && boss->getComponent<FinalBossManager>()->getHand()->getComponent<FinalBossAttack>()->getCollide()) {
		auto bossRect = boss->getComponent<Transform>()->getRectCollide();
		collide = Collisions::collides(Vector2D(hamster.x, hamster.y), hamster.w, hamster.h,
			Vector2D(bossRect.x, bossRect.y), bossRect.w, bossRect.h);
	}
	return collide;
}

//Devuelve la posicion en pantalla
Vector2D MapMngr::mapCoorsToSDLPoint(Vector2D coords) {
	return Vector2D(coords.getX() * TAM_CELDA, coords.getY() * TAM_CELDA);
}

//Devuelve la posicion del vector en coordenadas
Vector2D MapMngr::SDLPointToMapCoords(Vector2D p) {
	return Vector2D(p.getX() / TAM_CELDA, p.getY() / TAM_CELDA);
}


void MapMngr::loadEnemyRoom() {

	auto& enemies = entity_->getMngr()->getEnemies();

	const auto& objects = objectLayer->getObjects();
	numberEnemyRoom = 0;	//Guarda el numero de enemigos por sala
	for (const auto& object : objects)
	{
		auto& name = object.getName();
		auto mngr_ = entity_->getMngr();
		auto& prop = object.getProperties();

		if (name == "enemigo" && prop[1].getIntValue() == Room && prop[2].getIntValue() == RoundsCount) { //PROP[0] ES LA PROPIEDAD 0, EDITAR SI SE AÑADEN MAS
			auto* enemy = mngr_->addEntity();

			auto* enTr = enemy->addComponent<Transform>(
				Vector2D(object.getPosition().x * scale, object.getPosition().y * scale),
				Vector2D(), 86 * scale, 86 * scale, 0.0f, 0, 0, 0.3, 0.35);
			/*
			hamster1->addComponent<Transform>(
				Vector2D(object.getPosition().x * scale, object.getPosition().y * scale),
				Vector2D(), tam * scale, tam * scale, 0.0f, 0, 0, 0.5, 0.5);*/



			enTr->setFloor(prop[0].getIntValue() * TAM_CELDA * scale);
			enTr->setZ(prop[0].getIntValue() * TAM_CELDA * scale);
			enTr->getFlip() = true;
			enemy->addComponent<EnemyStateMachine>();
			//1º: False porque no es un hamster //2º: True porque usa de referencia el rect de colision
			enemy->addComponent<Shadow>(false, true);

			enemy->setGroup<Enemy>(true);

			enemy->addComponent<EntityAttribs>(150 + ((hamstersToLoad_.size() - 1) * 75), 0.0, prop[3].getStringValue(), Vector2D(3.6, 2), 0, 0, 5, 70);

			enemy->addComponent<Animator>(
				&sdlutils().images().at(prop[3].getStringValue() + "Sheet"),
				86,
				86,
				3,
				3,
				220,
				Vector2D(0, 0),
				3
				);
			enemy->addComponent<AnimEnemyStateMachine>();

			//enemy->addComponent<UI>("canelon", 4);

			enemy->addComponent<EnemyAttack>();
			enemy->addComponent<Knockback>();
			enTr->setGravity(enemy->addComponent<Gravity>());
			enemy->addComponent<CollisionDetec>();
			enemy->addComponent<MovementSimple>();

			enemy->addComponent<EnemyBehaviour>(new IddleEnemy());

			enemies.push_back(enemy);
			//anyadir a los cuidados de la madre
			mngr_->getHandler<Mother>()->getComponent<EnemyMother>()->addEnemy(enemy);

			enemy->addComponent<EnemyStun>();
			numberEnemyRoom++;
		}
		else if (name == "enemigoFuerte" && prop[1].getIntValue() == Room && prop[2].getIntValue() == RoundsCount) { //PROP[0] ES LA PROPIEDAD 0, EDITAR SI SE AÑADEN MAS	
			auto* enemy = mngr_->addEntity();
			auto* enTr = enemy->addComponent<Transform>(
				Vector2D(object.getPosition().x * scale, object.getPosition().y * scale),
				Vector2D(), 128 * scale, 128 * scale, 0.0f, 0, 0, 0.35, 0.4);

			enTr->setFloor(prop[0].getIntValue() * TAM_CELDA * scale);
			enTr->setFloor(prop[0].getIntValue() * TAM_CELDA * scale);
			enTr->setZ(prop[0].getIntValue() * TAM_CELDA * scale);
			enTr->getFlip() = true;

			enemy->addComponent<EnemyStateMachine>();
			enemy->addComponent<Shadow>(false, true);
			enemy->setGroup<Enemy>(true);

			enemy->addComponent<EntityAttribs>(250 + ((hamstersToLoad_.size() - 1) * 75), 0.0, prop[3].getStringValue(), Vector2D(3.6, 2), 0, 0, 10, true, false, true);

			enemy->addComponent<Animator>(
				&sdlutils().images().at(prop[3].getStringValue() + "Sheet"),
				128,
				128,
				3,
				3,
				220,
				Vector2D(0, 0),
				3
				);
			enemy->addComponent<AnimEnemyStateMachine>();

			if (prop[3].getStringValue() == "rata")
				enemy->addComponent<EnemyAttack>();
			else
				enemy->addComponent<EnemyAttack>(300, 500);

			enemy->addComponent<EnemyStrongAttack>();
			enemy->addComponent<Knockback>();
			enTr->setGravity(enemy->addComponent<Gravity>());
			enemy->addComponent<CollisionDetec>();
			enemy->addComponent<MovementSimple>();

			enemy->addComponent<EnemyBehaviour>(new IddleEnemy());

			enemies.push_back(enemy);

			//anyadir a los cuidados de la madre
			mngr_->getHandler<Mother>()->getComponent<EnemyMother>()->addEnemy(enemy);

			enemy->addComponent<EnemyStun>();
			numberEnemyRoom++;
		}
		else if (name == "firstBoss" && prop[1].getIntValue() == Room && prop[2].getIntValue() == RoundsCount) { //PROP[0] ES LA PROPIEDAD 0, EDITAR SI SE AÑADEN MAS
			auto* enemy = mngr_->addTrap();
			enemy->addComponent<Transform>(
				Vector2D(object.getPosition().x * scale, (object.getPosition().y - 300) * scale),
				Vector2D(), scale * 164.0f, scale * 600.0f, 0.0f, 0.5f, 1.0f)->getFlip() = true;

			enemy->addComponent<EnemyStateMachine>();
			enemy->setGroup<Enemy>(true);

			enemy->addComponent<EntityAttribs>(600 + (hamstersToLoad_.size() * 100), 0.0, "calcetin", Vector2D(4.5, 2), 0, 0, 20, true, true, false);

			//enemy->addComponent<Image>(&sdlutils().images().at("firstBoss"));
			enemy->addComponent<Animator>(
				&sdlutils().images().at("calcetinSheet"),
				164,
				600,
				3,
				3,
				100,
				Vector2D(0, 0),
				3
				);
			enemy->addComponent<AnimEnemyStateMachine>();
			enemy->addComponent<UI>("calcetin", 4);

			enemy->addComponent<FirstBossAttack>();
			enemy->addComponent<MovementSimple>();

			enemy->addComponent<EnemyBehaviour>(new FirstBossBehaviour());

			enemies.push_back(enemy);

			mngr_->setHandler<Boss>(enemy);

			numberEnemyRoom++;
		}
		else if (name == "finalBoss" && prop[1].getIntValue() == Room && prop[2].getIntValue() == RoundsCount) { //PROP[0] ES LA PROPIEDAD 0, EDITAR SI SE AÑADEN MAS
			auto* enemy = mngr_->addEntity();
			enemy->addComponent<Transform>(
				Vector2D(object.getPosition().x * scale, (object.getPosition().y - 300) * scale),
				Vector2D(), 164.0 * scale, 330.0f * scale, 0.0f, 0.8f, 0.8f)->getFlip() = true;

			enemy->addComponent<FinalBossManager>(hamstersToLoad_.size(), scale);
			mngr_->setHandler<FinalBoss>(enemy);
			numberEnemyRoom++;
		}
		else if (name == "escalectris" && prop[1].getIntValue() == Room && prop[3].getIntValue() == RoundsCount) { //PROP[0] ES LA PROPIEDAD 0, EDITAR SI SE AÑADEN MAS
			auto* escalectris = mngr_->addWaveObject();

			escalectris->addComponent<Transform>(
				Vector2D(object.getPosition().x * scale, object.getPosition().y * scale),
				Vector2D(),/* 5*23.27f*/256.0f, 5 * 256.0f, 0.0f, 0.8f, 0.8f)->getFlip() = true;

			escalectris->addComponent<ObstacleMoveable>(&sdlutils().images().at("warningSign"),
				object.getPosition().x * scale, object.getPosition().y * scale, 0, 0, scale,
				prop[2].getFloatValue(), prop[0].getFloatValue()
				);
		}
	}
}

void MapMngr::addHamster(string name, int i, const tmx::Object& object) {

	//do stuff with object properties
	//auto& name = obj.getName();
	auto mngr_ = entity_->getMngr();
	auto& players = mngr_->getPlayers();
	auto* hamster1 = mngr_->addEntity();

	int tam = 0;
	//Habilidad
	if (name == "sardinilla") {
		tam = 86;
		hamster1->addComponent<Transform>(Vector2D(object.getPosition().x * scale, object.getPosition().y * scale),
			Vector2D(), tam * scale, tam * scale, 0.0f, 0, 0, 0.5, 0.5);
		hamster1->addComponent<HamsterStateMachine>();
		hamster1->addComponent<EntityAttribs>(150, 0.0, name, Vector2D(7, 3.5), i, 0, 20, 70);
	}
	else if (name == "keta") {
		tam = 100;
		hamster1->addComponent<Transform>(Vector2D((object.getPosition().x + object.getAABB().width) * scale, object.getPosition().y * scale),
			Vector2D(), tam * scale, tam * scale, 0.0f, 0, 0, 0.5, 0.25);
		hamster1->addComponent<HamsterStateMachine>();
		hamster1->addComponent<EntityAttribs>(10, 0.0, name, Vector2D(9, 5.5), i, 10, 80, 70);
	}
	else if (name == "monchi") {
		tam = 86;
		hamster1->addComponent<Transform>(Vector2D((object.getPosition().x + object.getAABB().width / 3) * scale, (object.getPosition().y + object.getAABB().height / 2) * scale),
			Vector2D(), tam * scale, tam * scale, 0.0f, 0, 0, 0.4, 0.4);
		hamster1->addComponent<HamsterStateMachine>();
		hamster1->addComponent<EntityAttribs>(100, 0.0, name, Vector2D(7, 3.5), i, 0, 20, 70);
	}
	else if (name == "canelon") {
		tam = 128;
		hamster1->addComponent<Transform>(Vector2D((object.getPosition().x + (object.getAABB().width * 1.3)) * scale, (object.getPosition().y + object.getAABB().height / 2) * scale),
			Vector2D(), tam * scale, tam * scale, 0.0f, 0, 0, 0.25, 0.3);
		hamster1->addComponent<HamsterStateMachine>();
		hamster1->addComponent<EntityAttribs>(100, 1.2, name, Vector2D(7, 3.5), i, 0, 20, 70);
	}
	else {
		tam = 86;
		hamster1->addComponent<Transform>(Vector2D((object.getPosition().x + object.getAABB().width / 2) * scale, (object.getPosition().y + object.getAABB().height / 2) * scale),
			Vector2D(), tam * scale, tam * scale, 0.0f, 0, 0, 0.5, 0.5);
		hamster1->addComponent<HamsterStateMachine>();
		hamster1->addComponent<EntityAttribs>(100, 0.0, name, Vector2D(7, 3.5), i, 0, 20, 70);
	}
	//1º: True, porque es un hamster //2º: False, porque usa de referencia el rect del Animator
	hamster1->addComponent<Shadow>(true, true);

	Transform* tr = hamster1->getComponent<Transform>();

	hamster1->addComponent<Animator>(
		&sdlutils().images().at(name + "Sheet"),
		tam,
		tam,
		3,
		3,
		220,
		Vector2D(0, 0),
		3
		);
	hamster1->addComponent<AnimHamsterStateMachine>();
	hamster1->addComponent<Gravity>();
	hamster1->addComponent<CollisionDetec>();
	hamster1->addComponent<MovementInChase>()->setActive(false);
	hamster1->addComponent<Movement>();

	tr->setGravity(hamster1->getComponent<Gravity>());

	//Ataques Basicos
	hamster1->addComponent<LightAttack>();
	hamster1->addComponent<StrongAttack>();
	hamster1->addComponent<Combos>();

	//Interfaz
	hamster1->addComponent<UI>(name, i);
	hamster1->addComponent<HeartUI>(name, i);

	//Habilidad
	if (name == "sardinilla") hamster1->addComponent<Roll>();
	else if (name == "canelon") hamster1->addComponent<Pray>(30, 10);
	else if (name == "keta") hamster1->addComponent<Poison>(7);
	else if (name == "monchi") {
		hamster1->addComponent<Turret>();
		//hamster1->addComponent<Swallow>(5);
	}
	else hamster1->addComponent<WarCry>(0.25, 1.5);

	//Gestion de infartos
	hamster1->addComponent<PossesionGame>();
	hamster1->addComponent<GhostCtrl>();
	//ES NECESARIO PASAR LA ESTRATEGIA QUE DEBE USAR EL STROKE O SE VA A LA PUTA (RandomStrokeStrategy o FairStrokeStrategy)
	if (hamstersToLoad_.size() > 1) {
		FairStrokeStrategy* rndStrat = new FairStrokeStrategy();
		hamster1->addComponent<Stroke>(rndStrat);
	}
	else {
		FairStrokeStrategy* farirStrat = new FairStrokeStrategy();
		hamster1->addComponent<Stroke>(farirStrat);
	}
	hamster1->addComponent<Knockback>();

	//Handlr
	hamster1->addComponent<ControlHandler>(hamster1->getComponent<EntityAttribs>()->getNumber());
	hamster1->addComponent<Stun>();

	//Lepermite recoger objetos
	hamster1->addComponent<GetItem>();

	hamster1->setGroup<Ally>(true);

	//Lo a�adimos al vector de entidades
	players.push_back(hamster1);

	//añadirlo tmb a la lista de control de enemyMother
	mngr_->getHandler<Mother>()->getComponent<EnemyMother>()->addObjetive(hamster1);

	// Asignación de handler
	switch (i) {
	case 0:
		mngr_->setHandler<Hamster1>(hamster1);
		break;
	case 1:
		mngr_->setHandler<Hamster2>(hamster1);
		break;
	case 2:
		mngr_->setHandler<Hamster3>(hamster1);
		break;
	case 3:
		mngr_->setHandler<Hamster4>(hamster1);
		break;
	}
}

//void MapMngr::Refresh() {
//	roomTrigger.erase( //
//		std::remove_if( //
//			roomTrigger.begin(), //
//			roomTrigger.end(), //
//			[](const Entity* e) { //
//				return !e->isActive();
//			}), //
//		roomTrigger.end());
//}

void MapMngr::newSceneTrigger(string newScene, const tmx::Object& object) {

	//Creamos una entidad
	auto trigger = entity_->getMngr()->addEntity();
	trigger->addComponent<Transform>(Vector2D(object.getPosition().x * scale, object.getPosition().y * scale),
		Vector2D(), object.getAABB().width * scale, object.getAABB().height * scale, 0.0f, 1, 1);
	trigger->addComponent<TriggerScene>(newScene, object.getProperties()[1].getIntValue());
	trigger->getMngr()->setHandler<TriggetCat>(trigger);
}

void MapMngr::startChaseTrigger(const tmx::Object& object) {

	//Creamos una entidad
	auto trigger = entity_->getMngr()->addEntity();
	trigger->addComponent<Transform>(Vector2D(object.getPosition().x * scale, object.getPosition().y * scale),
		Vector2D(), object.getAABB().width * scale, object.getAABB().height * scale, 0.0f, 1, 1);
	trigger->addComponent<StartChase>();
}


void MapMngr::startMusic(const tmx::Object& object) {

	//Creamos una entidad
	auto trigger = entity_->getMngr()->addEntity();
	trigger->addComponent<Transform>(Vector2D(object.getPosition().x * scale, object.getPosition().y * scale),
		Vector2D(), object.getAABB().width * scale, object.getAABB().height * scale, 0.0f, 1, 1);
	trigger->addComponent<TriggerMusic>(object.getProperties()[0].getStringValue());
}

void MapMngr::addObject(const tmx::Object& object) {
	auto* obstacle = entity_->getMngr()->addEntity();

	auto& prop = object.getProperties();
	// bool: rompible? true : false
	// string : id del objeto "Box", "..."
	// int : nº de golpes. Si no es rompible, se ignora
	// int : pos en Z. Necesario meterlo a mano desde Tile

	obstacle->addComponent<Transform>(Vector2D(object.getPosition().x * scale, object.getPosition().y * scale),
		Vector2D(), object.getAABB().width * scale, object.getAABB().height * scale, 0.0f, prop[3].getIntValue(), false, 0.75, 0.6);



	string id = prop[1].getStringValue();

	obstacle->addComponent<Shadow>(false, true);

	obstacle->addComponent<Animator>(&sdlutils().images().at("obstacle" + id),
		100,
		78,
		9,
		2,
		220,
		Vector2D(0, 0),
		3
		)->play(sdlutils().anims().at("obstacleStatic"));

	if (prop[0].getBoolValue()) {
		obstacle->addComponent<Obstacle>(id, prop[2].getIntValue());
		//1º: False, porque no es un hamster //2º: False, porque usa de referencia el rect del Animator
	}
	else {
		obstacle->addComponent<Obstacle>(id);
	}

	entity_->getMngr()->getObstacles().push_back(obstacle);
}

void MapMngr::addTrap(const tmx::Object& object, int x, int y) {
	auto& prop = object.getProperties();


	auto* trap = entity_->getMngr()->addTrap();

	if (prop[1].getIntValue() == 0) {

		trap->addComponent<Transform>(Vector2D(x * scale, y * scale),
			Vector2D(), 64 * scale, 64 * scale, 0.0f, 1, 1);

		trap->addComponent<ContactDamage>(3, 30, true, true, true);
		trap->addComponent<TimeTrap>(&sdlutils().images().at("vitroPeqOn"), &sdlutils().images().at("vitroPeqOff"), prop[0].getFloatValue());
	}
	if (prop[1].getIntValue() == 1) {

		trap->addComponent<Transform>(Vector2D(x * scale, y * scale),
			Vector2D(), 128 * scale, 128 * scale, 0.0f, 1, 1);

		trap->addComponent<ContactDamage>(3, 30, true, true, true);
		trap->addComponent<TimeTrap>(&sdlutils().images().at("vitroGranOn"), &sdlutils().images().at("vitroGranOff"), prop[0].getFloatValue());
	}

	trap->addComponent<EntityAttribs>(1, 10.0f, "trap1", Vector2D(), 1, 0.0f, 1, true, false, false);

}

void MapMngr::resetTriggerList()
{
	while (!triggerFtCamera.empty())
	{
		triggerFtCamera.pop();
	}
}

void MapMngr::clearColliders() {
	if (collisionCreated) {
		for (int i = 0; i < mapDimensions_.x; i++)
		{
			delete[] collider[i];
		}
		delete[] collider;
		collisionCreated = false;
	}
}


