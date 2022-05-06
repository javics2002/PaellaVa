#include "NetworkManager.h"

#include "../Control/Game.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Muebles/Mueble.h"
#include "../GameObjects/Muebles/Puerta.h"
#include "../Control/ObjectManager.h"

#include "../Data/ListaComandas.h"

#include "../Scenes/Lobby.h"
#include "../Scenes/Jornada.h"
#include "../Scenes/GameOver.h"
#include "../Scenes/Menu.h"
#include "../Scenes/Disconnected.h"
#include "../Scenes/FinalScene.h"

#include "../Utils/Vector2D.h"

// SERVIDOR

// Función que se encarga de aceptar a los jugadores (se encuentra en un hilo)

void NetworkManager::acceptPlayers()
{
	while (!mExitThread) {
		TCPsocket csd;

		if ((csd = SDLNet_TCP_Accept(socket)))
		{
			IPaddress* remoteIP;

			if ((remoteIP = SDLNet_TCP_GetPeerAddress(csd)))
				std::cout << ("Host connected: %x %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port)) << std::endl;
			else
				std::cout << ("SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError()) << std::endl;

			int id = getClientID(*remoteIP);

			Packet pkt;
			pkt.packet_type = EPT_ACCEPT;

			strcpy_s(pkt.accept.player_name, mMyName.c_str());
			sdlutils().soundEffects().at("connected").play();

			if (mPlayerSockets.size() > MAX_PLAYERS) { // TODO: CHECK IF A PLAYER IS DISCONNECTED
				pkt.packet_type = EPT_DENY;
			}
			else {
				if (id == -1) {
					id = mIdCount;

					mPlayerIps.push_back(*remoteIP); // Como eres nuevo, se mete tu IP en el vector
				}

				mPlayerSockets.push_back(csd); // Se mete tu socket en el vector
			}

			pkt.accept.player_id = id;

			if (SDLNet_TCP_Send(csd, &pkt, sizeof(Packet)) < sizeof(Packet))
			{
				std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		SDL_Delay(mAcceptFrequency);
	}
}

// Función que se encarga de actualizar los jugadores (se encuentra en un hilo)
// Recibe los paquetes de todos los jugadores

void NetworkManager::receivePlayers()
{
	Packet pkt;
	Lobby* lobby;

	while (!mExitThread) {
		for (int i = 1u; i < mPlayerSockets.size(); i++) { // Comenzamos en uno porque el 0 somos nosotros mismos
			if (SDLNet_TCP_Recv(mPlayerSockets[i], &pkt, sizeof(Packet)) > 0)
			{
				switch (pkt.packet_type) {
				case EPT_NAME:
					// Asignamos el nombre
					mOtherName = pkt.name.player_name;

					// Actualizamos el lobby
					lobby = dynamic_cast<Lobby*>(mGame->getCurrentScene());

					lobby->clienteUnido(mOtherName);
					break;
				case EPT_BUTTONBUFFER:
					{
					// Transformar array a vector
					vector<bool> buffer(8, false);
					for (int i = 0u; i < ih().getOtherKeyPressed().size(); i++) {
						buffer[i] = pkt.buttonBuffer.buttonBuffer[i];
					}

					// Procesar buffer
					ih().setOtherKeyPressed(buffer);
					ih().updateOtherAxis();

					}
					break;
				case EPT_SYNCPLAYER:
					if (mGameStarted) {
						mGame->getObjectManager()->getPlayerTwo()->setPosition(pkt.syncPlayers.posX, pkt.syncPlayers.posY);
						mGame->getObjectManager()->getPlayerTwo()->setVel(Vector2D<double>(pkt.syncPlayers.velX, pkt.syncPlayers.velY));
					}
					break;
				case EPT_SYNCPICKOBJECT:
					mGame->getObjectManager()->getPlayerTwo()->PickCustomObject(pkt.syncPickObject.object_type, pkt.syncPickObject.object_id, pkt.syncPickObject.mueble_id, pkt.syncPickObject.extra_info);
					break;
				case EPT_SYNCDROPOBJECT:
					mGame->getObjectManager()->getPlayerTwo()->DropCustomObject(pkt.syncDropObject.object_type, pkt.syncDropObject.object_id, pkt.syncDropObject.mueble_id);
					break;
				case EPT_SYNCPEDIDO:
					// Crear pedido en el grupo de clientes correspondiente
					for (int i = 0; i < mGame->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->getActiveObjects().size(); i++) {
						GrupoClientes* gC = mGame->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->getActiveObjects()[i];
						if (gC->getId() == pkt.syncPedido.group_id) {
							vector<int> tamPaellas;
							vector<int> ingPedidos(12, LAST);

							for (int i = 0; i < pkt.syncPedido.paella_number; i++) {
								tamPaellas.push_back(pkt.syncPedido.paella_size[i]);
							}

							for (int i = 0; i < ingPedidos.size(); i++) {
								ingPedidos[i] = pkt.syncPedido.ing_pedidos[i];
							}

							gC->modificaPedido(pkt.syncPedido.paella_number, tamPaellas, ingPedidos);
						}
					}
					break;
				case EPT_SYNCPAUSE:
					dynamic_cast<Jornada*>(mGame->getCurrentScene())->togglePause();
					break;
				case EPT_SYNCCOMANDA:
					{
					vector<int> tamPaellas;

					for (int i = 0; i < pkt.syncComanda.paella_number; i++) {
						tamPaellas.push_back(pkt.syncComanda.paella_size[i]);
					}

					vector<int> ingPedidos(begin(pkt.syncComanda.ing_pedidos), end(pkt.syncComanda.ing_pedidos));

					Comanda* com = new Comanda(mGame, pkt.syncComanda.numMesa, tamPaellas, ingPedidos);
					mGame->getUIManager()->getBarra()->AñadeComanda(com);
					}
					break;
				case EPT_QUIT:
					setGameStarted(false);

					SDLNet_TCP_Close(mPlayerSockets[i]);

					mGame->sendMessageScene(new Disconnected(mGame));
					break;
				case EPT_SYNCBARRA:
					if (mGameStarted)
					{
						mGame->getUIManager()->getBarra()->borralaComandaQueteMandan(pkt.syncBarra.idComanda);
					}
					break;
				}

				
			}
		}

		SDL_Delay(mRecvFrequency);
	}
}

// Función que tienen los clientes (se encuentra en un hilo)
// Se encarga de recibir los paquetes del servidor
void NetworkManager::updateClient()
{
	Packet server_pkt;

	Ingrediente* i;
	IngredienteLetal* iLetal;
	
	while (!mExitThread) {
		// Receive info from server
		if (SDLNet_TCP_Recv(socket, &server_pkt, sizeof(Packet)) > 0) {
			Player* p = nullptr;

			switch (server_pkt.packet_type)
			{
			case EPT_START:
				// Start game
				mGame->sendMessageScene(new Jornada(mGame, "Jornada" + to_string(server_pkt.startGame.num_jornada + 1), server_pkt.startGame.num_jornada, false));
				startGameTimer();
				break;
			case EPT_CREATEING:
				if (mGameStarted) {
					i = mGame->getObjectManager()->getPool<Ingrediente>(_p_INGREDIENTE)->add(Vector2D<double>(server_pkt.ingrediente.posX, server_pkt.ingrediente.posY));
					i->setVel(Vector2D<double>(server_pkt.ingrediente.velX, server_pkt.ingrediente.velY));
					i->cambiaTipo(server_pkt.ingrediente.tipo_ingrediente);
					i->setId(server_pkt.ingrediente.ing_id);
				}
				break;
			case EPT_CREATEINGLET:
				if (mGameStarted) {
					iLetal = mGame->getObjectManager()->getPool<IngredienteLetal>(_p_INGREDIENTELETAL)->add(Vector2D<double>(server_pkt.ingrediente.posX, server_pkt.ingrediente.posY));
					iLetal->setVel(Vector2D<double>(server_pkt.ingrediente.velX, server_pkt.ingrediente.velY));
					iLetal->cambiaTipo(server_pkt.ingrediente.tipo_ingrediente);
				}
				break;
			case EPT_CREATECLIENTGROUP:
				{
				if (mGameStarted) {
					vector<Cliente*> v;

					Puerta* puerta = nullptr;

					for (auto m : mGame->getObjectManager()->getMuebles()) {
						if (m->getId() == server_pkt.grupoCliente.door_id) {
							puerta = dynamic_cast<Puerta*>(m);
							break;
						}
					}

					Vector2D<double> distancia = Vector2D<double>(server_pkt.grupoCliente.dirX, server_pkt.grupoCliente.dirY);
					Vector2D<double> pos = puerta->getPosition();

					for (int i = 0; i < server_pkt.grupoCliente.tamGrupo; i++) {
						Cliente* c = mGame->getObjectManager()->getPool<Cliente>(_p_CLIENTE)->add();
						c->setPosition(pos);
						c->setAnimResources(server_pkt.grupoCliente.textCliente[i]);
						puerta->setOrientation(c);

						pos = pos - distancia;

						v.push_back(c);
					}

					GrupoClientes* g = mGame->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->add();
					g->setId(server_pkt.grupoCliente.group_id);
					g->setVel(Vector2D<double>(server_pkt.grupoCliente.velX, server_pkt.grupoCliente.velY));

					puerta->getCola()->add(g, server_pkt.grupoCliente.tamGrupo);
					g->initGrupo(puerta->getCola(), v);

					sdlutils().soundEffects().at("puerta").play();
				}
				}
				break;
			case EPT_BUTTONBUFFER:
				{
				if (mGameStarted) {
					// Transformar array a vector
					vector<bool> buffer(4, false);
					for (int i = 0u; i < ih().getOtherKeyPressed().size(); i++) {
						buffer[i] = server_pkt.buttonBuffer.buttonBuffer[i];
					}

					// Procesar buffer
					ih().setOtherKeyPressed(buffer);
					ih().updateOtherAxis();
				}
				}

				break;
			case EPT_SYNCPLAYER:
				if (mGameStarted) {
					mGame->getObjectManager()->getPlayerTwo()->setPosition(server_pkt.syncPlayers.posX, server_pkt.syncPlayers.posY);
					mGame->getObjectManager()->getPlayerTwo()->setVel(Vector2D<double>(server_pkt.syncPlayers.velX, server_pkt.syncPlayers.velY));
				}
				
				break;
			case EPT_SYNCPICKOBJECT:
				if (mGameStarted) {
					// recorrer la pool correspondiente a object type, encontrar el objeto con la id correspondiente y coger dicho objeto
					mGame->getObjectManager()->getPlayerTwo()->PickCustomObject(server_pkt.syncPickObject.object_type, server_pkt.syncPickObject.object_id, server_pkt.syncPickObject.mueble_id, server_pkt.syncPickObject.extra_info);
				}
				break;
			case EPT_SYNCDROPOBJECT:
				if (mGameStarted) {
					mGame->getObjectManager()->getPlayerTwo()->DropCustomObject(server_pkt.syncDropObject.object_type, server_pkt.syncDropObject.object_id, server_pkt.syncDropObject.mueble_id);
				}
				break;
			case EPT_SYNCPEDIDO:
				if (mGameStarted) {
					// crear pedido en x grupo de clientes
					for (int i = 0; i < mGame->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->getActiveObjects().size(); i++) {
						GrupoClientes* gC = mGame->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->getActiveObjects()[i];
						if (gC->getId() == server_pkt.syncPedido.group_id) {
							vector<int> tamPaellas;
							vector<int> ingPedidos(12, LAST);

							for (int i = 0; i < server_pkt.syncPedido.paella_number; i++) {
								tamPaellas.push_back(server_pkt.syncPedido.paella_size[i]);
							}

							for (int i = 0; i < ingPedidos.size(); i++) {
								ingPedidos[i] = server_pkt.syncPedido.ing_pedidos[i];
							}

							gC->modificaPedido(server_pkt.syncPedido.paella_number, tamPaellas, ingPedidos);
						}
					}
				}

				break;
			case EPT_SYNCMUEBLEROTO:
				if (mGameStarted) {
					// romper mueble con la id que toque
					for (auto m : mGame->getObjectManager()->getMuebles()) {
						if (server_pkt.syncMuebleRoto.mueble_id == m->getId()) {
							m->romperMueble();
						}
					}
				}
				break;

			case EPT_SYNCPAUSE:
				if (mGameStarted) {
					dynamic_cast<Jornada*>(mGame->getCurrentScene())->togglePause();
				}
				break;

			case EPT_SYNCCOMANDA:
				{
				if (mGameStarted) {
					vector<int> tamPaellas;

					for (int i = 0; i < server_pkt.syncComanda.paella_number; i++) {
						tamPaellas.push_back(server_pkt.syncComanda.paella_size[i]);
					}

					vector<int> ingPedidos(begin(server_pkt.syncComanda.ing_pedidos), end(server_pkt.syncComanda.ing_pedidos));

					Comanda* com = new Comanda(mGame, server_pkt.syncComanda.numMesa, tamPaellas, ingPedidos);
					mGame->getUIManager()->getBarra()->AñadeComanda(com);
				}
				}
				break;
			case EPT_FINISHGAME:
				setGameStarted(false);
				mGame->sendMessageScene(new GameOver(mGame, server_pkt.finishGame.punctuation, server_pkt.finishGame.numJornada));

				break;

			case EPT_ENDGAME:
				setGameStarted(false);
				mGame->sendMessageScene(new FinalScene(mGame));

				break;
			case EPT_QUIT:
				setGameStarted(false);

				SDLNet_TCP_Close(socket);

				mGame->sendMessageScene(new Disconnected(mGame));
				break;
			case EPT_SYNCBARRA:
				if (mGameStarted) {
					mGame->getUIManager()->getBarra()->borralaComandaQueteMandan(server_pkt.syncBarra.idComanda);
				}
				break;
			}

		}

		SDL_Delay(mClientFrequency);
	}
}

// Función para asignar IDs
int NetworkManager::getClientID(const IPaddress& addr)
{
	int id = -1;

	for (int i = 0u; i < mPlayerIps.size(); i++) {
		if (compareAddress(mPlayerIps[i], addr)) {
			id = i;
			break;
		}
	}

	return id;
}

bool NetworkManager::compareAddress(const IPaddress& addr1, const IPaddress& addr2)
{
	return addr1.host == addr2.host && addr1.port == addr2.port;
}

NetworkManager::NetworkManager(Game* game_)
{
	mExitThread = false;
	mGameStarted = false;

	nType = '0';
	mIdCount = 0;

	mGame = game_;

	mAcceptFrequency = 150;
	mRecvFrequency = 50;
	mSendFrequency = 100;

	mClientFrequency = 50;
}

NetworkManager::~NetworkManager()
{
	SDLNet_TCP_Close(socket);
	SDLNet_Quit();
}

// Función que inicializa SDL_Net y tu función (servidor o cliente)

bool NetworkManager::init(char type, const char* ip_addr, std::string name)
{
	if (SDLNet_Init() < 0)
	{
		std::cout << ("SDLNet_Init: %s\n", SDLNet_GetError()) << std::endl;
		return false;
	}

	nType = type;
	

	if (SDLNet_ResolveHost(&ip, ip_addr, 2000) < 0)
	{
		std::cout << "SDLNet_ResolveHost: " <<  SDLNet_GetError() << std::endl;
		return false;
	}

	if (!(socket = SDLNet_TCP_Open(&ip)))
	{
		std::cout << ("SDLNet_TCP_Open: %s\n", SDLNet_GetError()) << std::endl;
		return false;
	}
	
	mMyName = name;
	
	Packet pkt;

	if (type == 'c') { // Si somos un cliente
		mHost = false;

		while (SDLNet_TCP_Recv(socket, &pkt, sizeof(Packet)) == 0); // Esperamos a que el servidor nos acepte

		if (pkt.packet_type == EPT_ACCEPT) { // Cuando nos acepte
			// recv info
			mOtherName = pkt.accept.player_name;

			// Mandarle el nombre
			// Send info
			Packet namePkt;
			namePkt.packet_type = EPT_NAME;

			strcpy_s(namePkt.name.player_name, mMyName.c_str());

			if (SDLNet_TCP_Send(socket, &namePkt, sizeof(Packet)) < sizeof(Packet))
			{
				std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
				exit(EXIT_FAILURE);
			}

			updateclient_t = new std::thread(&NetworkManager::updateClient, this);
		}
		else if (pkt.packet_type == EPT_DENY) { // Si nos rechazan porque la partida está llena
			return false;
		}
	}
	else { // Si somos un host

		mHost = true;
		
		// Hilos
		accept_t = new std::thread(&NetworkManager::acceptPlayers, this);
		receiveplayers_t = new std::thread(&NetworkManager::receivePlayers, this);

		mPlayerSockets.push_back(socket);
		mPlayerIps.push_back(ip);

		mClientId = 0;
	}

	
	return true;
}


void NetworkManager::update() // HILO PARA SINCRONIZAR ESTADO DE JUEGO (lo tienen los 2 jugadores)
{
	if (mGameStarted) {
		if (mLastUpdate + mUpdateTime > sdlutils().currRealTime()) { // Cada 200 ms
			return;
		}

		mLastUpdate = sdlutils().currRealTime();

		// Sync Players
		syncPlayers();
	}

}

void NetworkManager::close()
{
	mExitThread = true;

	if (nType == 'h') { // si se es host, tienes que mandar a todo el mundo que te has desconectado

		Packet pkt;

		pkt.packet_type = EPT_QUIT;

		for (int i = 1u; i < mPlayerSockets.size(); i++) { // empezamos en 1 porque el 0 eres tú mismo
			if (SDLNet_TCP_Send(mPlayerSockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
			{
				std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		accept_t->join();
		receiveplayers_t->join();

		delete accept_t;
		delete receiveplayers_t;
	}
	else if (nType == 'c') {

		Packet pkt;

		pkt.packet_type = EPT_QUIT;

		if (SDLNet_TCP_Send(socket, &pkt, sizeof(Packet)) < sizeof(Packet))
		{
			std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
			exit(EXIT_FAILURE);
		}

		updateclient_t->join();
		delete updateclient_t;
	}

	mPlayerIds.clear();
	mPlayerIps.clear();
	mPlayerSockets.clear();
	
	SDLNet_TCP_Close(socket);
}

void NetworkManager::sendStartGame(int numJornada) {
	// Lo manda el servidor al cliente para que empiece la partida
	startGameTimer();

	Packet pkt;
	pkt.packet_type = EPT_START;
	pkt.startGame.num_jornada = numJornada;

	for (int i = 1u; i < mPlayerSockets.size(); i++) {
		if (SDLNet_TCP_Send(mPlayerSockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
		{
			std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
			exit(EXIT_FAILURE);
		}
	}

}

void NetworkManager::startGameTimer() {
	mGameStarted = true;
	mLastUpdate = SDL_GetTicks();
}

void NetworkManager::sendCreateIngrediente(int tipoIngrediente, int ingId, Vector2D<double> pos, Vector2D<double> vel) {

	Packet pkt;
	pkt.packet_type = EPT_CREATEING;
	pkt.ingrediente.tipo_ingrediente = tipoIngrediente;
	pkt.ingrediente.ing_id = ingId;

	pkt.ingrediente.posX = pos.getX();
	pkt.ingrediente.posY = pos.getY();

	pkt.ingrediente.velX = vel.getX();
	pkt.ingrediente.velY = vel.getY();

	for (int i = 1u; i < mPlayerSockets.size(); i++) {
		if (SDLNet_TCP_Send(mPlayerSockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
		{
			std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

void NetworkManager::sendCreateIngredienteLetal(int tipoIngrediente, Vector2D<double> pos, Vector2D<double> vel) {

	Packet pkt;
	pkt.packet_type = EPT_CREATEINGLET;
	pkt.ingrediente.tipo_ingrediente = tipoIngrediente;

	pkt.ingrediente.posX = pos.getX();
	pkt.ingrediente.posY = pos.getY();

	pkt.ingrediente.velX = vel.getX();
	pkt.ingrediente.velY = vel.getY();

	for (int i = 1u; i < mPlayerSockets.size(); i++) {
		if (SDLNet_TCP_Send(mPlayerSockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
		{
			std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

void NetworkManager::sendButtonsBuffer(vector<bool> keyPressed)
{
	if (mGameStarted) {
		Packet pkt;
		pkt.packet_type = EPT_BUTTONBUFFER;

		for (int i = 0u; i < 4; i++) { // left up down right
			pkt.buttonBuffer.buttonBuffer[i] = keyPressed[i];
		}

		if (nType == 'h') {
			for (int i = 1u; i < mPlayerSockets.size(); i++) {
				if (SDLNet_TCP_Send(mPlayerSockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
				{
					std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
					exit(EXIT_FAILURE);
				}
			}
		}
		else {
			if (SDLNet_TCP_Send(socket, &pkt, sizeof(Packet)) < sizeof(Packet))
			{
				std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
}

void NetworkManager::syncPlayers()
{
	Packet pkt;
	pkt.packet_type = EPT_SYNCPLAYER;

	pkt.syncPlayers.posX = mGame->getObjectManager()->getPlayerOne()->getPosition().getX();
	pkt.syncPlayers.posY = mGame->getObjectManager()->getPlayerOne()->getPosition().getY();

	pkt.syncPlayers.velX = mGame->getObjectManager()->getPlayerOne()->getVel().getX();
	pkt.syncPlayers.velY = mGame->getObjectManager()->getPlayerOne()->getVel().getY();

	if (nType == 'h') {
		for (int i = 1u; i < mPlayerSockets.size(); i++) {
			if (SDLNet_TCP_Send(mPlayerSockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
			{
				std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	else {
		if (SDLNet_TCP_Send(socket, &pkt, sizeof(Packet)) < sizeof(Packet))
		{
			std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

void NetworkManager::syncDropObject(int objectType, int objectId, int muebleId)
{
	Packet pkt;

	pkt.packet_type = EPT_SYNCDROPOBJECT;

	pkt.syncDropObject.object_type = objectType;
	pkt.syncDropObject.object_id = objectId;
	pkt.syncDropObject.mueble_id = muebleId;

	if (nType == 'h') {
		for (int i = 1u; i < mPlayerSockets.size(); i++) {
			if (SDLNet_TCP_Send(mPlayerSockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
			{
				std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	else {
		if (SDLNet_TCP_Send(socket, &pkt, sizeof(Packet)) < sizeof(Packet))
		{
			std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}


void NetworkManager::sendGrupoCliente(int tamGrupo, int idPuerta, int idGrupoClientes, Vector2D<double> vel, Vector2D<double> distancia, std::vector<int> textureNumber, float tolerancia)
{
	Packet pkt;

	pkt.packet_type = EPT_CREATECLIENTGROUP;
	pkt.grupoCliente.tamGrupo = tamGrupo;

	pkt.grupoCliente.door_id = idPuerta;
	pkt.grupoCliente.group_id = idGrupoClientes;

	pkt.grupoCliente.velX = vel.getX();
	pkt.grupoCliente.velY = vel.getY();

	pkt.grupoCliente.dirX = distancia.getX();
	pkt.grupoCliente.dirY = distancia.getY();

	for (int i = 0u; i < tamGrupo; i++) {
		pkt.grupoCliente.textCliente[i] = textureNumber[i];
	}

	pkt.grupoCliente.tolerancia = tolerancia;

	for (int i = 1u; i < mPlayerSockets.size(); i++) {
		if (SDLNet_TCP_Send(mPlayerSockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
		{
			std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

void NetworkManager::syncPickObject(int objectType, int objectId, int muebleId, int extraInfo)
{
	Packet pkt;
	pkt.packet_type = EPT_SYNCPICKOBJECT;
	pkt.syncPickObject.object_type = objectType;
	pkt.syncPickObject.object_id = objectId;
	pkt.syncPickObject.mueble_id = muebleId;
	pkt.syncPickObject.extra_info = extraInfo;

	if (nType == 'h') {
		for (int i = 1u; i < mPlayerSockets.size(); i++) {
			if (SDLNet_TCP_Send(mPlayerSockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
			{
				std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	else {
		if (SDLNet_TCP_Send(socket, &pkt, sizeof(Packet)) < sizeof(Packet))
		{
			std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

void NetworkManager::syncMuebleRoto(int muebleId)
{
	Packet pkt;
	pkt.packet_type = EPT_SYNCMUEBLEROTO;
	pkt.syncMuebleRoto.mueble_id = muebleId;

	for (int i = 1u; i < mPlayerSockets.size(); i++) {
		if (SDLNet_TCP_Send(mPlayerSockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
		{
			std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

void NetworkManager::sendFinishGame(int finalPunctuation, int nJornada, bool gameEnded)
{
	Packet pkt;

	if (gameEnded)
		pkt.packet_type = EPT_ENDGAME;
	else
		pkt.packet_type = EPT_FINISHGAME;

	pkt.finishGame.punctuation = finalPunctuation;
	pkt.finishGame.numJornada = nJornada;

	for (int i = 1u; i < mPlayerSockets.size(); i++) {
		if (SDLNet_TCP_Send(mPlayerSockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
		{
			std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

void NetworkManager::syncPedido(int idGrupoCliente, int numPaellas, std::vector<int> tamPaella, std::vector<int> ingPedidos)
{
	Packet pkt;

	pkt.packet_type = EPT_SYNCPEDIDO;
	pkt.syncPedido.group_id = idGrupoCliente;
	pkt.syncPedido.paella_number = numPaellas;

	for (int i = 0; i < tamPaella.size(); i++) {
		pkt.syncPedido.paella_size[i] = tamPaella[i];
	}

	for (int i = 0; i < ingPedidos.size(); i++) {
		pkt.syncPedido.ing_pedidos[i] = ingPedidos[i];
	}
	

	if (nType == 'h') {
		for (int i = 1u; i < mPlayerSockets.size(); i++) {
			if (SDLNet_TCP_Send(mPlayerSockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
			{
				std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	else {
		if (SDLNet_TCP_Send(socket, &pkt, sizeof(Packet)) < sizeof(Packet))
		{
			std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

void NetworkManager::syncPause() {
	Packet pkt;
	pkt.packet_type = EPT_SYNCPAUSE;

	if (nType == 'h') {
		for (int i = 1u; i < mPlayerSockets.size(); i++) {
			if (SDLNet_TCP_Send(mPlayerSockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
			{
				std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	else {
		if (SDLNet_TCP_Send(socket, &pkt, sizeof(Packet)) < sizeof(Packet))
		{
			std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

void NetworkManager::syncComanda(int numMesa, vector<int> tamPaella, vector<int> ingPedidos) {
	Packet pkt;

	pkt.packet_type = EPT_SYNCCOMANDA;
	pkt.syncComanda.numMesa = numMesa;
	pkt.syncComanda.paella_number = tamPaella.size();

	for (int i = 0; i < tamPaella.size(); i++) {
		pkt.syncComanda.paella_size[i] = tamPaella[i];
	}

	for (int i = 0; i < ingPedidos.size(); i++) {
		pkt.syncComanda.ing_pedidos[i] = ingPedidos[i];
	}

	if (nType == 'h') {
		for (int i = 1u; i < mPlayerSockets.size(); i++) {
			if (SDLNet_TCP_Send(mPlayerSockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
			{
				std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	else {
		if (SDLNet_TCP_Send(socket, &pkt, sizeof(Packet)) < sizeof(Packet))
		{
			std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}
void NetworkManager::syncBarra(int c)
{
	Packet pkt;

	pkt.packet_type = EPT_SYNCBARRA;
	pkt.syncBarra.idComanda = c;
	if (nType == 'h') {
		for (int i = 1u; i < mPlayerSockets.size(); i++) {
			if (SDLNet_TCP_Send(mPlayerSockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
			{
				std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	else {
		if (SDLNet_TCP_Send(socket, &pkt, sizeof(Packet)) < sizeof(Packet))
		{
			std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
			exit(EXIT_FAILURE);
		}
	}

}