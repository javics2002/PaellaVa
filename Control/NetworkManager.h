#pragma once
#include "../SDL2_net-2.0.1/include/SDL_net.h"

#include "../GameObjects/Player.h"
#include "../Control/Game.h"

#include <vector>

// #include "Timer.h"

using namespace std;

//Estructurar como singleton
class NetworkManager
{/*
	enum PacketType {};

	struct Packet {

	};

private:
	int id_count;
	SDL_Surface* surface;
	Game* game_;

	Player* players[MAX_PLAYERS];
	TCPsocket player_sockets[MAX_PLAYERS];
	IPaddress player_ips[MAX_PLAYERS];

	// SERVER

	std::thread* accept_t;
	std::thread* receiveplayers_t;
	std::thread* sendplayers_t;

	void AcceptPlayers();
	void UpdatePlayers();
	void UpdateClients();

	char nType;
	IPaddress ip;
	TCPsocket socket;

	float accept_frequency;
	float send_frequency;
	float recv_frequency;

	float client_frequency;

	// Timer network_timer;
public:
	NetworkManager(Game* game);
	~NetworkManager();

	void Init(char type, SDL_Surface* srfc, const char* ip_addr);
	void Update();
	void Render();

	void Close();

	Player* AddPlayer();
	Player* GetPlayer(int id);
	Player* GetFirst();*/
};

