#include "NetworkManager.h"

#include "../Control/Game.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Muebles/Mueble.h"
#include "../GameObjects/Muebles/Puerta.h"
#include "../Control/ObjectManager.h"

#include "../Scenes/Lobby.h"
#include "../Scenes/Jornada.h"

#include "../Utils/Vector2D.h"

// SERVIDOR

// Función que acepta a los jugadores (se encuentra en un hilo)
// Problemas que da: ---
void NetworkManager::acceptPlayers()
{
	while (!exitThread) {
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

			strcpy_s(pkt.accept.player_name, myName.c_str());
			sdlutils().soundEffects().at("connected").play();

			if (player_sockets.size() > MAX_PLAYERS) { // TODO: CHECK IF A PLAYER IS DISCONNECTED
				pkt.packet_type = EPT_DENY;
			}
			else {
				if (id == -1) {
					id = id_count;

					/*Player* p = addPlayerHost();

					game_->getObjectManager()->addPlayer(p);*/

					player_ips.push_back(*remoteIP); // Como eres nuevo, se mete tu IP en el vector
				}

				player_sockets.push_back(csd); // Se mete tu socket en el vector
			}

			pkt.accept.player_id = id;

			if (SDLNet_TCP_Send(csd, &pkt, sizeof(Packet)) < sizeof(Packet))
			{
				std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		SDL_Delay(accept_frequency);
	}
}

// Función que se encarga de actualizar los jugadores (se encuentra en un hilo)
// Problemas que da: ---
void NetworkManager::receivePlayers()
{
	Packet pkt;
	Lobby* lobby;

	while (!exitThread) {
		for (int i = 1u; i < player_sockets.size(); i++) { // Comenzamos en uno porque el 0 somos nosotros mismos
			if (SDLNet_TCP_Recv(player_sockets[i], &pkt, sizeof(Packet)) > 0)
			{
				switch (pkt.packet_type) {
				case EPT_UPDATE:
					// game_->getObjectManager()->getPlayers()[i]->handleInput(Vector2D<double>(packet.player_horizontal, packet.player_vertical));
					break;
				case EPT_NAME:
					/*pkt = *(PacketName*)(void*)&packet;*/
					// name
					otherName = pkt.name.player_name;

					// actualizar lobby
					lobby = dynamic_cast<Lobby*>(game->getCurrentScene());

					lobby->clienteUnido(otherName);
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
					game->getUIManager()->addTween(game->getObjectManager()->getPlayerTwo()->getPosition().getX(), pkt.syncPlayers.posX, 300,true).via(easing::linear).onStep([this](tweeny::tween<float>& t, float) mutable {
						game->getObjectManager()->getPlayerTwo()->setPosition(t.peek(), game->getObjectManager()->getPlayerTwo()->getPosition().getY());

						return t.progress() == 1.0f;
					});

					game->getUIManager()->addTween(game->getObjectManager()->getPlayerTwo()->getPosition().getY(), pkt.syncPlayers.posY, 300,true).via(easing::linear).onStep([this](tweeny::tween<float>& t, float) mutable {
						game->getObjectManager()->getPlayerTwo()->setPosition(game->getObjectManager()->getPlayerTwo()->getPosition().getX(), t.peek());

						return t.progress() == 1.0f;
					});

					// game_->getObjectManager()->getPlayerTwo()->setPosition(Vector2D<double>(pkt.syncPlayers.posX, pkt.syncPlayers.posY));
					break;

				case EPT_SYNCPICKOBJECT:
					// recorrer la pool correspondiente a object type, encontrar el objeto con la id correspondiente y coger dicho objeto
					game->getObjectManager()->getPlayerTwo()->PickCustomObject(pkt.syncPickObject.object_type, pkt.syncPickObject.object_id, pkt.syncPickObject.mueble_id, pkt.syncPickObject.extra_info);

					break;
				case EPT_SYNCDROPOBJECT:
					game->getObjectManager()->getPlayerTwo()->DropCustomObject(pkt.syncDropObject.object_type, pkt.syncDropObject.object_id, pkt.syncDropObject.mueble_id);
					break;
				case EPT_SYNCPEDIDO:
					// crear pedido en x grupo de clientes
					for (int i = 0; i < game->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->getActiveObjects().size(); i++) {
						GrupoClientes* gC = game->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->getActiveObjects()[i];
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
				case EPT_QUIT:
					std::cout << ("Client disconnected: ID(%d)\n", i) << std::endl;

					SDLNet_TCP_Close(player_sockets[i]);

					// borrar con iterador su socket y su player
					// player_sockets[i] = NULL;

					break;
				}

				
			} // TODO: CHECK WHEN TIRAR DEL CABLE
			  // TODO: SYNC PLAYERS?
		}

		SDL_Delay(recv_frequency);
	}
}


// Función que se encarga de mandar la información de todos los jugadores a todos los jugadores (se encuentra en un hilo)
// Problemas que da: ---
void NetworkManager::sendPlayers()
{
	//while (!exitThread) {
	//	// UPDATE CLIENTS
	//	Player* p;
	//	Packet packet;
	//	packet.packet_type = EPT_UPDATE;

	//	// Send each player to the other players
	//	for (int i = 0u; i < game_->getObjectManager()->getPlayers().size(); i++) {
	//		p = game_->getObjectManager()->getPlayers()[i];

	//		int currentId = 0;

	//		// recorrer vector de ids hasta que i == player_ids[j]
	//		for (int j = 0u; j < player_ids.size(); j++) {
	//			if (i == player_ids[j]) currentId = player_ids[j];
	//		}

	//		packet.player_id = currentId;
	//		packet.player_horizontal = p->getAxis().getX();
	//		packet.player_vertical = p->getAxis().getY();

	//		for (int j = 1u; j < game_->getObjectManager()->getPlayers().size(); j++) { // Empezamos en uno porque el servidor ya tiene su propio hilo
	//																					// que recibe los datos de los clientes, por tanto no hace falta mandar nada.
	//			if (i != j) {
	//				if (SDLNet_TCP_Send(player_sockets[j], &packet, sizeof(PacketSend)) < sizeof(PacketSend))
	//				{
	//					std::cout << (stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
	//					exit(EXIT_FAILURE);
	//				}
	//			}
	//		}
	//	}

	//	SDL_Delay(send_frequency);
	//}
}


// Función que tienen los clientes (se encuentra en un hilo)
// Problemas que da: ---
void NetworkManager::updateClient()
{
	Packet server_pkt;

	Ingrediente* i;
	IngredienteLetal* iLetal;
	

	while (!exitThread) {
		// Receive info from server
		if (SDLNet_TCP_Recv(socket, &server_pkt, sizeof(Packet)) > 0) {
			Player* p = nullptr;

			switch (server_pkt.packet_type)
			{
			case EPT_START:
				// Start game
				game->sendMessageScene(new Jornada(game, "Jornada1", 0, false));
				startGameTimer();

				break;
			case EPT_CREATEING:
				i = game->getObjectManager()->getPool<Ingrediente>(_p_INGREDIENTE)->add(Vector2D<double>(server_pkt.ingrediente.posX, server_pkt.ingrediente.posY));
				i->setVel(Vector2D<double>(server_pkt.ingrediente.velX, server_pkt.ingrediente.velY));
				i->cambiaTipo(server_pkt.ingrediente.tipo_ingrediente);
				i->setId(server_pkt.ingrediente.ing_id);

				break;
			case EPT_CREATEINGLET:
				iLetal = game->getObjectManager()->getPool<IngredienteLetal>(_p_INGREDIENTELETAL)->add(Vector2D<double>(server_pkt.ingrediente.posX, server_pkt.ingrediente.posY));
				iLetal->setVel(Vector2D<double>(server_pkt.ingrediente.velX, server_pkt.ingrediente.velY));
				iLetal->cambiaTipo(server_pkt.ingrediente.tipo_ingrediente);

				break;

			case EPT_CREATECLIENTGROUP:
				{
				vector<Cliente*> v;

				Puerta* puerta = nullptr;

				for (auto m : game->getObjectManager()->getMuebles()) {
					if (m->getId() == server_pkt.grupoCliente.door_id) {
						puerta = dynamic_cast<Puerta*>(m);
						break;
					}
				}

				Vector2D<double> distancia = Vector2D<double>(server_pkt.grupoCliente.dirX, server_pkt.grupoCliente.dirY);
				Vector2D<double> pos = puerta->getPosition();

				for (int i = 0; i < server_pkt.grupoCliente.tamGrupo; i++) {
					Cliente* c = game->getObjectManager()->getPool<Cliente>(_p_CLIENTE)->add();
					c->setPosition(pos);
					c->cambiaTextura(texturasClientes[server_pkt.grupoCliente.textCliente[i]]);

					pos = pos - distancia;

					v.push_back(c);
				}

				GrupoClientes* g = game->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->add();
				g->setId(server_pkt.grupoCliente.group_id);
				g->setVel(Vector2D<double>(server_pkt.grupoCliente.velX, server_pkt.grupoCliente.velY));

				puerta->getCola()->add(g, server_pkt.grupoCliente.tamGrupo);
				g->initGrupo(puerta->getCola(), v);

				sdlutils().soundEffects().at("puerta").play();
				}

				break;
			case EPT_BUTTONBUFFER:
				{
				// Transformar array a vector
				vector<bool> buffer(4, false);
				for (int i = 0u; i < ih().getOtherKeyPressed().size(); i++) {
					buffer[i] = server_pkt.buttonBuffer.buttonBuffer[i];
				}

				// Procesar buffer
				ih().setOtherKeyPressed(buffer);
				ih().updateOtherAxis();

				}

				break;
			case EPT_SYNCPLAYER:
				game->getUIManager()->addTween(game->getObjectManager()->getPlayerTwo()->getPosition().getX(), server_pkt.syncPlayers.posX, 300,true).via(easing::linear).onStep([this](tweeny::tween<float>& t, float) mutable {
					game->getObjectManager()->getPlayerTwo()->setPosition(t.peek(), game->getObjectManager()->getPlayerTwo()->getPosition().getY());

					return t.progress() == 1.0f;
					});

				game->getUIManager()->addTween(game->getObjectManager()->getPlayerTwo()->getPosition().getY(), server_pkt.syncPlayers.posY, 300,true).via(easing::linear).onStep([this](tweeny::tween<float>& t, float) mutable {
					game->getObjectManager()->getPlayerTwo()->setPosition(game->getObjectManager()->getPlayerTwo()->getPosition().getX(), t.peek());

					return t.progress() == 1.0f;
					});
				break;
			case EPT_SYNCPICKOBJECT:
				// recorrer la pool correspondiente a object type, encontrar el objeto con la id correspondiente y coger dicho objeto
				game->getObjectManager()->getPlayerTwo()->PickCustomObject(server_pkt.syncPickObject.object_type, server_pkt.syncPickObject.object_id, server_pkt.syncPickObject.mueble_id, server_pkt.syncPickObject.extra_info);

				break;
			case EPT_SYNCDROPOBJECT:
				game->getObjectManager()->getPlayerTwo()->DropCustomObject(server_pkt.syncDropObject.object_type, server_pkt.syncDropObject.object_id, server_pkt.syncDropObject.mueble_id);
				break;
			case EPT_SYNCPEDIDO:
				// crear pedido en x grupo de clientes
				for (int i = 0; i < game->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->getActiveObjects().size(); i++) {
					GrupoClientes* gC = game->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->getActiveObjects()[i];
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

				break;
			}

		}

		SDL_Delay(client_frequency);
	}
}

// Función para asignar IDs
int NetworkManager::getClientID(const IPaddress& addr)
{
	int id = -1;

	for (int i = 0u; i < player_ips.size(); i++) {
		if (compareAddress(player_ips[i], addr)) {
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
	exitThread = false;
	gameStarted = false;

	nType = '0';
	id_count = 0;

	game = game_;

	//player_sockets = vector<TCPsocket>(MAX_PLAYERS);
	//player_ips = vector<IPaddress>(MAX_PLAYERS);
	//player_ids = vector<int>(MAX_PLAYERS);

	accept_frequency = 150;
	recv_frequency = 50;
	send_frequency = 100;

	client_frequency = 50;
}

NetworkManager::~NetworkManager()
{
	SDLNet_TCP_Close(socket);
	SDLNet_Quit();
}

// Función que inicializa SDL_Net y tu función (servidor o cliente)
// Problemas que da: ---

bool NetworkManager::init(char type, const char* ip_addr, std::string name)
{
	if (SDLNet_Init() < 0)
	{
		std::cout << ("SDLNet_Init: %s\n", SDLNet_GetError()) << std::endl;
		exit(EXIT_FAILURE);
	}

	nType = type;
	

	if (SDLNet_ResolveHost(&ip, ip_addr, 2000) < 0)
	{
		std::cout << "SDLNet_ResolveHost: " <<  SDLNet_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	if (!(socket = SDLNet_TCP_Open(&ip)))
	{
		std::cout << ("SDLNet_TCP_Open: %s\n", SDLNet_GetError()) << std::endl;
		exit(EXIT_FAILURE);
	}
	
	myName = name;
	
	Packet pkt;

	if (type == 'c') { // Si somos un cliente
		while (SDLNet_TCP_Recv(socket, &pkt, sizeof(Packet)) == 0); // Esperamos a que el servidor nos acepte

		if (pkt.packet_type == EPT_ACCEPT) { // Cuando nos acepte, se crea el personaje, etc.
			// recv info
			otherName = pkt.accept.player_name;

			// Mandarle el nombre
			// Send info
			Packet namePkt;
			namePkt.packet_type = EPT_NAME;

			// namePkt.player_name = myName;
			strcpy_s(namePkt.name.player_name, myName.c_str());

			if (SDLNet_TCP_Send(socket, &namePkt, sizeof(Packet)) < sizeof(Packet))
			{
				std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
				exit(EXIT_FAILURE);
			}

			//game_->getObjectManager()->addPlayer(addPlayerClient(pkt.player_id));
			//
			//client_id = pkt.player_id;

			//id_count = MAX_PLAYERS;

			updateclient_t = new std::thread(&NetworkManager::updateClient, this);
		}
		else if (pkt.packet_type == EPT_DENY) { // Si nos rechazan porque la partida está llena
			return false;
		}
	}
	else { // Si somos un host
		// game_->getObjectManager()->addPlayer(addPlayerHost());

		// Hilos

		accept_t = new std::thread(&NetworkManager::acceptPlayers, this);
		receiveplayers_t = new std::thread(&NetworkManager::receivePlayers, this);

		// sendplayers_t = new std::thread(&NetworkManager::sendPlayers, this);

		player_sockets.push_back(socket);
		player_ips.push_back(ip);

		client_id = 0;
	}

	
	return true;
}


// Currently testing
void NetworkManager::update() // SINCRONIZAR ESTADO DE JUEGO
{
	if (gameStarted) {
		if (lastUpdate_ + updateTime_ > sdlutils().currRealTime()) { // si no pasan
			return;
		}

		lastUpdate_ = sdlutils().currRealTime();

		// Sync Players
		syncPlayers();
	}
	
	
	//PacketSyncPlayer pkt_send;
	// Send status
	//for (int i = 0; i < id_count; i++) {
	//	
	//	pkt_send.packet_type = EPT_SYNCPLAYER;
	//	pkt_send.player_id = players[i]->getId();
	//	pkt_send.posX = players[i]->getPosition().getX();
	//	pkt_send.posY = players[i]->getPosition().getY();
	//	
	//	for (int j = 0; j < id_count; j++) {
	//		if (i != j) {
	//			if (SDLNet_TCP_Send(player_sockets[j], (void*)&pkt_send, sizeof(PacketSyncPlayer)) < sizeof(PacketSyncPlayer))
	//			{
	//				fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
	//				exit(EXIT_FAILURE);
	//			}
	//		}
	//	}
	//}

}

void NetworkManager::close()
{
	exitThread = true;

	if (nType == 'h') { // si se es host, tienes que mandar a todo el mundo que te has desconectado
		Packet pkt;

		pkt.packet_type = EPT_QUIT;
		pkt.send.player_id = 0;

		for (int i = 1u; i < player_sockets.size(); i++) { // empezamos en 1 porque el 0 eres tú mismo
			if (SDLNet_TCP_Send(player_sockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
			{
				std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		accept_t->join();
		receiveplayers_t->join();
		// sendplayers_t->join();

		delete accept_t;
		delete receiveplayers_t;
		// delete sendplayers_t;

	}
	else if (nType == 'c') {
		Packet pkt;

		pkt.packet_type = EPT_QUIT;
		pkt.send.player_id = client_id;

		if (SDLNet_TCP_Send(socket, &pkt, sizeof(Packet)) < sizeof(Packet))
		{
			std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
			exit(EXIT_FAILURE);
		}

		updateclient_t->join();
		delete updateclient_t;
	}

	player_ids.clear();
	player_ips.clear();
	player_sockets.clear();
	
	SDLNet_TCP_Close(socket);
}

Player* NetworkManager::addPlayerHost()
{
	Player* p = new Player(game, true,0,0);
	
	player_ids.push_back(id_count);
	id_count++;

	return p;
}

Player* NetworkManager::addPlayerClient(int id)
{
	Player* p = new Player(game, false,0,0);
	
	player_ids.push_back(id);

	return p;
}

void NetworkManager::sendStartGame(int numJornada) {
	// Lo manda el servidor al cliente para que empiece la partida
	startGameTimer();

	Packet pkt;
	pkt.packet_type = EPT_START;
	pkt.startGame.num_jornada = numJornada;

	for (int i = 1u; i < player_sockets.size(); i++) {
		if (SDLNet_TCP_Send(player_sockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
		{
			std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
			exit(EXIT_FAILURE);
		}
	}

}

void NetworkManager::startGameTimer() {
	gameStarted = true; // test
	lastUpdate_ = SDL_GetTicks();
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

	for (int i = 1u; i < player_sockets.size(); i++) {
		if (SDLNet_TCP_Send(player_sockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
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

	for (int i = 1u; i < player_sockets.size(); i++) {
		if (SDLNet_TCP_Send(player_sockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
		{
			std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

void NetworkManager::sendButtonsBuffer(vector<bool> keyPressed)
{
	if (gameStarted) {
		Packet pkt;
		pkt.packet_type = EPT_BUTTONBUFFER;

		for (int i = 0u; i < 4; i++) { // left up down right
			pkt.buttonBuffer.buttonBuffer[i] = keyPressed[i];
		}

		if (nType == 'h') {
			for (int i = 1u; i < player_sockets.size(); i++) {
				if (SDLNet_TCP_Send(player_sockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
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

	pkt.syncPlayers.posX = game->getObjectManager()->getPlayerOne()->getPosition().getX();
	pkt.syncPlayers.posY = game->getObjectManager()->getPlayerOne()->getPosition().getY();

	if (nType == 'h') {
		for (int i = 1u; i < player_sockets.size(); i++) {
			if (SDLNet_TCP_Send(player_sockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
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
		for (int i = 1u; i < player_sockets.size(); i++) {
			if (SDLNet_TCP_Send(player_sockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
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

	for (int i = 1u; i < player_sockets.size(); i++) {
		if (SDLNet_TCP_Send(player_sockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
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
		for (int i = 1u; i < player_sockets.size(); i++) {
			if (SDLNet_TCP_Send(player_sockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
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
		for (int i = 1u; i < player_sockets.size(); i++) {
			if (SDLNet_TCP_Send(player_sockets[i], &pkt, sizeof(Packet)) < sizeof(Packet))
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
