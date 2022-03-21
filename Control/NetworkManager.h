#pragma once
#include "../SDL2_net-2.0.1/include/SDL_net.h"

#include <vector>
#include <thread>
#include <iostream>
#include <mutex>

// #include "Timer.h"

#define MAX_PLAYERS 15

class Game;
class Player;

enum EPacketType {
	EPT_UPDATE,
	EPT_QUIT,
	EPT_ACCEPT,
	EPT_SYNCPLAYER,
	EPT_DENY
};

enum EInputType {
	EIT_UP,
	EIT_DOWN,
	EIT_RIGHT,
	EIT_LEFT,
	EIT_NONE
};

// Currently testing
struct PacketSyncPlayer {
	Uint8 packet_type;
	Uint8 player_id;
	Sint16 posX;
	Sint16 posY;
};


struct PacketSend {
	Uint8 packet_type;
	Uint8 player_id;
	Sint8 player_horizontal;
	Sint8 player_vertical;
};

// Paquete que recibe el servidor, lo mandan los clientes
struct PacketRecv {
	Uint8 packet_type;
	Sint8 player_horizontal;
	Sint8 player_vertical;
};

// Paquete que manda el servidor cuando acepta a un juegador
struct PacketAccept {
	Uint8 packet_type;
	Uint8 player_id;
};

class NetworkManager
{
private:
	

	int id_count;
	Game* game_;

	Player* players[MAX_PLAYERS];
	TCPsocket player_sockets[MAX_PLAYERS];
	IPaddress player_ips[MAX_PLAYERS];

	// SERVER

	std::thread* accept_t;
	std::thread* receiveplayers_t;
	std::thread* sendplayers_t;

	void AcceptPlayers();
	void ReceivePlayers();
	void SendPlayers();

	// CLIENT

	int client_id;

	std::thread* updateclient_t;

	void UpdateClient();

	// UTILS

	int GetClientID(const IPaddress& addr);

	bool CompareAddress(const IPaddress& addr1, const IPaddress& addr2);

	// VARIABLES

	char nType;

	// For clients ip=server_address, for host ip=local_address
	IPaddress ip;

	// For clients socket=socket_server, for host socket=local_socket
	TCPsocket socket;

	float accept_frequency;
	float send_frequency;
	float recv_frequency;

	float client_frequency;

	// Timer
	Uint32 lastUpdate_; //tiempo desde el último update
	Uint32 updateTime_ = 500; //los segundos que tarda en actualizarse el reloj

public:
	NetworkManager(Game* game);
	~NetworkManager();

	bool Init(char type, const char* ip_addr = NULL);
	void Update();

	void Close();

	Player* AddPlayerHost();
	Player* AddPlayerClient(int id);

	Player* GetFirst() { return players[0]; }
};