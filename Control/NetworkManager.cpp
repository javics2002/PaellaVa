#include "NetworkManager.h"

#include "../Control/Game.h"
#include "../GameObjects/Player.h"
#include "../Control/ObjectManager.h"

void NetworkManager::AcceptPlayers()
{
	while (1) {
		TCPsocket csd;
		if ((csd = SDLNet_TCP_Accept(socket)))
		{
			IPaddress* remoteIP;

			if ((remoteIP = SDLNet_TCP_GetPeerAddress(csd)))
				printf("Host connected: %x %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
			else
				fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());

			int id = GetClientID(*remoteIP);

			PacketAccept pkt;
			pkt.packet_type = EPT_ACCEPT;

			if (id_count >= MAX_PLAYERS) { // TODO: CHECK IF A PLAYER IS DISCONNECTED
				pkt.packet_type = EPT_DENY;
			}
			else {
				if (id == -1) {
					Player* p = AddPlayerHost();
					// id = p->GetId();
				}

				player_sockets[id] = csd;
				player_ips[id] = *remoteIP;
			}

			pkt.player_id = id;

			if (SDLNet_TCP_Send(csd, (void*)&pkt, sizeof(PacketAccept)) < sizeof(PacketAccept))
			{
				fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
				exit(EXIT_FAILURE);
			}
		}

		SDL_Delay(accept_frequency);
	}
}

void NetworkManager::ReceivePlayers()
{
	while (1) {
		PacketRecv packet;

		for (int i = 0; i < id_count; i++) {
			if (SDLNet_TCP_Recv(player_sockets[i], &packet, sizeof(PacketRecv)) > 0)
			{
				switch (packet.packet_type) {
				case EPT_UPDATE:
					UpdatePlayer(i, packet.player_horizontal, packet.player_vertical);
					break;
				case EPT_QUIT:
					printf("Client disconnected: ID(%d)\n", i);

					SDLNet_TCP_Close(player_sockets[i]);
					player_sockets[i] = NULL;
					break;
				}
			} // TODO: CHECK WHEN TIRAR DEL CABLE
		}

		SDL_Delay(recv_frequency);
	}
}

void NetworkManager::SendPlayers()
{
	while (1) {
		// UPDATE CLIENTS
		Player* p;
		PacketSend packet;
		packet.packet_type = EPT_UPDATE;

		// Send each player to the other players
		for (int i = 0; i < id_count; i++) {
			p = players[i];
			packet.player_id = i;
			packet.posX = p->getX();
			packet.posY = p->getY();

			for (int j = 0; j < id_count; j++) {
				if (SDLNet_TCP_Send(player_sockets[j], (void*)&packet, sizeof(PacketSend)) < sizeof(PacketSend))
				{
					fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
					exit(EXIT_FAILURE);
				}
			}
		}

		SDL_Delay(send_frequency);
	}
}

void NetworkManager::UpdateClient()
{
	while (1) {
		PacketSend server_pkt;
		
		// Receive info
		if (SDLNet_TCP_Recv(socket, &server_pkt, sizeof(PacketSend)) > 0) {
			Player* p = players[server_pkt.player_id];

			if (p == NULL) {
				p = AddPlayerClient(server_pkt.player_id);
			}

			switch (server_pkt.packet_type) {
			case EPT_UPDATE:
				p->setPosition(server_pkt.posX, server_pkt.posY);
				break;
			}
		}

		// Send info
		PacketRecv client_pkt;
		client_pkt.packet_type = EPT_UPDATE;
		//client_pkt.player_horizontal = players[client_id]->GetHorizontal();
		//client_pkt.player_vertical = players[client_id]->GetVertical();

		if (SDLNet_TCP_Send(socket, &client_pkt, sizeof(PacketRecv)) < sizeof(PacketRecv))
		{
			fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}

		SDL_Delay(client_frequency);
	}
}

int NetworkManager::GetClientID(const IPaddress& addr)
{
	int id = -1;

	for (int i = 0; i < id_count; i++) {
		if (CompareAddress(player_ips[i], addr)) {
			id = i;
			break;
		}
	}

	return id;
}

bool NetworkManager::CompareAddress(const IPaddress& addr1, const IPaddress& addr2)
{
	return addr1.host == addr2.host && addr1.port == addr2.port;
}

void NetworkManager::UpdatePlayer(int id, Sint8 horizontal, Sint8 vertical)
{
	Player* p = players[id];
	int movespeed = 10;

	int x = p->getX() + horizontal * movespeed;
	int y = p->getY() + vertical * movespeed;

	p->setPosition(x, y);
}

NetworkManager::NetworkManager(Game* game)
{
	nType = '0';
	id_count = 0;

	game_ = game;

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

bool NetworkManager::Init(char type, const char* ip_addr)
{
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	nType = type;

	if (SDLNet_ResolveHost(&ip, ip_addr, 2000) < 0)
	{
		fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	if (!(socket = SDLNet_TCP_Open(&ip)))
	{
		fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	
	PacketAccept pkt;

	if (type == 'c') {
		while (SDLNet_TCP_Recv(socket, &pkt, sizeof(PacketAccept)) == 0);

		if (pkt.packet_type == EPT_ACCEPT) {
			Player* player = AddPlayerClient(pkt.player_id);
			// player->SetColor(0xFFFFFF);

			client_id = pkt.player_id;

			id_count = MAX_PLAYERS;
		}
		else if (pkt.packet_type == EPT_DENY) {
			return false;
		}
	}
	else {
		game_->getObjectManager()->addPlayer(AddPlayerHost());
		// player->SetColor(0xFFFFFF);

		accept_t = new std::thread(&NetworkManager::AcceptPlayers, this);
		receiveplayers_t = new std::thread(&NetworkManager::ReceivePlayers, this);
		sendplayers_t = new std::thread(&NetworkManager::SendPlayers, this);

		player_sockets[0] = socket;
		player_ips[0] = ip;

		client_id = 0;
	}

	updateclient_t = new std::thread(&NetworkManager::UpdateClient, this);

	return true;
}

void NetworkManager::Update()
{
}

void NetworkManager::Render()
{
	//for (int i = 0; i < id_count; i++) {
	//	if (players[i]) {
	//		players[i]->Render();
	//	}
	//}
}

void NetworkManager::Close()
{
	Player* player = players[client_id];

	PacketSend pkt;
	pkt.packet_type = EPT_QUIT;
	// pkt.player_id = player->GetId();

	if (SDLNet_TCP_Send(socket, (void*)&pkt, sizeof(PacketSend)) < sizeof(PacketSend))
	{
		fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	if (nType == 'h') {
		delete accept_t;
		delete receiveplayers_t;
		delete sendplayers_t;
	}

	delete updateclient_t;
}

Player* NetworkManager::AddPlayerHost()
{
	Player* p = new Player(game_);

	// p->Init(surface);
	// p->SetId(id_count);

	players[id_count] = p;

	id_count++;

	return p;
}

Player* NetworkManager::AddPlayerClient(int id)
{
	Player* p = new Player(game_);
	/*p->Init(surface);
	p->SetId(id);*/

	players[id] = p;

	return p;
}

Player* NetworkManager::GetClientPlayer()
{
	return players[client_id];
}
