#include "NetworkManager.h"

#include "../Control/Game.h"
#include "../GameObjects/Player.h"
#include "../Control/ObjectManager.h"

#include "../Scenes/Lobby.h"
#include "../Scenes/Jornada.h"

#include <vector>

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

			PacketAccept pkt;
			pkt.packet_type = EPT_ACCEPT;

			strcpy_s(pkt.player_name, myName.c_str());

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

			pkt.player_id = id;

			if (SDLNet_TCP_Send(csd, &pkt, sizeof(PacketAccept)) < sizeof(PacketAccept))
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
	PacketName pkt;
	Lobby* lobby;

	while (!exitThread) {
		for (int i = 1u; i < player_sockets.size(); i++) { // Comenzamos en uno porque el 0 somos nosotros mismos
			if (SDLNet_TCP_Recv(player_sockets[i], &pkt, sizeof(PacketName)) > 0)
			{
				switch (pkt.packet_type) {
				case EPT_UPDATE:
					// game_->getObjectManager()->getPlayers()[i]->handleInput(Vector2D<double>(packet.player_horizontal, packet.player_vertical));
					break;
				case EPT_NAME:
					/*pkt = *(PacketName*)(void*)&packet;*/
					// name
					otherName = pkt.player_name;

					// actualizar lobby
					lobby = dynamic_cast<Lobby*>(game_->getCurrentScene());

					lobby->clienteUnido(otherName);
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
	while (!exitThread) {
		// UPDATE CLIENTS
		Player* p;
		PacketSend packet;
		packet.packet_type = EPT_UPDATE;

		// Send each player to the other players
		for (int i = 0u; i < game_->getObjectManager()->getPlayers().size(); i++) {
			p = game_->getObjectManager()->getPlayers()[i];

			int currentId = 0;

			// recorrer vector de ids hasta que i == player_ids[j]
			for (int j = 0u; j < player_ids.size(); j++) {
				if (i == player_ids[j]) currentId = player_ids[j];
			}

			packet.player_id = currentId;
			packet.player_horizontal = p->getAxis().getX();
			packet.player_vertical = p->getAxis().getY();

			for (int j = 1u; j < game_->getObjectManager()->getPlayers().size(); j++) { // Empezamos en uno porque el servidor ya tiene su propio hilo
																						// que recibe los datos de los clientes, por tanto no hace falta mandar nada.
				if (i != j) {
					if (SDLNet_TCP_Send(player_sockets[j], &packet, sizeof(PacketSend)) < sizeof(PacketSend))
					{
						std::cout << (stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
						exit(EXIT_FAILURE);
					}
				}
			}
		}

		SDL_Delay(send_frequency);
	}
}


// Función que tienen los clientes (se encuentra en un hilo)
// Problemas que da: ---
void NetworkManager::updateClient()
{
	while (!exitThread) {
		PacketSend server_pkt;
		
		// Receive info from server
		if (SDLNet_TCP_Recv(socket, &server_pkt, sizeof(PacketSend)) > 0) {
			Player* p = nullptr;

			switch (server_pkt.packet_type)
			{
			case EPT_START:
				// Start game
				game_->changeScene(new Jornada(game_, "Jornada1", 0, false));

				break;
			default:
				break;
			}

			//// recorrer el vector de players hasta que pkt.player_id == player_ids[i]
			//for (int i = 0u; i < player_ids.size(); i++) {
			//	if (server_pkt.player_id == player_ids[i]) {
			//		p = game_->getObjectManager()->getPlayers()[i];
			//	}
			//}

			//if (p == nullptr) {
			//	p = addPlayerClient(server_pkt.player_id);
			//	game_->getObjectManager()->addPlayer(p);
			//}

			//switch (server_pkt.packet_type) {
			//case EPT_UPDATE:
			//	p->handleInput(Vector2D<double>(server_pkt.player_horizontal, server_pkt.player_vertical));
			//	break;
			//case EPT_SYNCPLAYER:
			//	// parsear pkt
			//	PacketSyncPlayer newPkt = *(PacketSyncPlayer*)(void*)&server_pkt;
			//	p->setPosition(newPkt.posX, newPkt.posY);

			//	break;
			//}
		}

		// Send info
		//PacketRecv client_pkt;
		//client_pkt.packet_type = EPT_UPDATE;
		//
		//client_pkt.player_horizontal = game_->getObjectManager()->getHost()->getAxis().getX(); // Nosotros mismos
		//client_pkt.player_vertical = game_->getObjectManager()->getHost()->getAxis().getY();

		//if (SDLNet_TCP_Send(socket, &client_pkt, sizeof(PacketRecv)) < sizeof(PacketRecv))
		//{
		//	std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
		//	exit(EXIT_FAILURE);
		//}

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

NetworkManager::NetworkManager(Game* game)
{
	exitThread = false;

	nType = '0';
	id_count = 0;

	game_ = game;

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
	
	PacketAccept pkt;

	if (type == 'c') { // Si somos un cliente
		while (SDLNet_TCP_Recv(socket, &pkt, sizeof(PacketAccept)) == 0); // Esperamos a que el servidor nos acepte

		if (pkt.packet_type == EPT_ACCEPT) { // Cuando nos acepte, se crea el personaje, etc.
			// recv info
			otherName = pkt.player_name;

			// Mandarle el nombre
			// Send info
			PacketName namePkt;
			namePkt.packet_type = EPT_NAME;

			// namePkt.player_name = myName;
			strcpy_s(namePkt.player_name, myName.c_str());

			if (SDLNet_TCP_Send(socket, &namePkt, sizeof(PacketName)) < sizeof(PacketName))
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


// Duda: Aquí es donde se sincronizarían las cosas?
void NetworkManager::update() // SINCRONIZAR ESTADO DE JUEGO CADA 0.5 SEGS
{
	if (lastUpdate_ + updateTime_ > sdlutils().currRealTime()) { //si no pasan
		return;
	}

	lastUpdate_ = sdlutils().currRealTime();
	
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
		PacketSend pkt;

		pkt.packet_type = EPT_QUIT;
		pkt.player_id = 0;

		for (int i = 1u; i < player_sockets.size(); i++) { // empezamos en 1 porque el 0 eres tú mismo
			if (SDLNet_TCP_Send(player_sockets[i], &pkt, sizeof(PacketSend)) < sizeof(PacketSend))
			{
				std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		accept_t->join();
		receiveplayers_t->join();
		sendplayers_t->join();

		delete accept_t;
		delete receiveplayers_t;
		delete sendplayers_t;

	}
	else if (nType == 'c') {
		PacketSend pkt;

		pkt.packet_type = EPT_QUIT;
		pkt.player_id = client_id;

		if (SDLNet_TCP_Send(socket, &pkt, sizeof(PacketSend)) < sizeof(PacketSend))
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
	Player* p = new Player(game_);
	
	player_ids.push_back(id_count);
	id_count++;

	return p;
}

Player* NetworkManager::addPlayerClient(int id)
{
	Player* p = new Player(game_);
	
	player_ids.push_back(id);

	return p;
}

void NetworkManager::sendStartGame(int numJornada) {
	// Lo manda el servidor al cliente para que empiece la partida

	PacketStartGame pkt;
	pkt.packet_type = EPT_START;
	pkt.num_jornada = numJornada;

	for (int i = 0u; i < player_sockets.size(); i++) {
		if (SDLNet_TCP_Send(player_sockets[i], &pkt, sizeof(PacketSend)) < sizeof(PacketSend))
		{
			std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
			exit(EXIT_FAILURE);
		}
	}

}
