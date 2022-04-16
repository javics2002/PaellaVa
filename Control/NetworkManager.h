#pragma once
#include "../SDL2_net-2.0.1/include/SDL_net.h"

#include "../Utils/Vector2D.h"

#include <vector>
#include <thread>
#include <iostream>
#include <mutex>

class Game;
class Player;


enum EPacketType {
	EPT_START,
	EPT_ACCEPT,
	EPT_DENY,
	EPT_NAME,
	EPT_UPDATE,
	EPT_CREATEING,
	EPT_CREATEINGLET,
	EPT_CREATECLIENTGROUP,
	EPT_BUTTONBUFFER,
	EPT_SYNCPLAYER,
	EPT_QUIT
};

enum EInputType {
	EIT_UP,
	EIT_DOWN,
	EIT_RIGHT,
	EIT_LEFT,
	EIT_NONE
};

struct PacketSend {
	Uint8 player_id;
	Sint8 player_horizontal;
	Sint8 player_vertical;
};

// Paquete que recibe el servidor, lo mandan los clientes
struct PacketRecv {
	Sint8 player_horizontal;
	Sint8 player_vertical;
};

// Paquete que manda el servidor cuando acepta a un jugador
struct PacketAccept {
	Uint8 player_id;
	char player_name[15];
};

// Paquete para mandar nombres
struct PacketName {
	char player_name[15];
};

// Paquete para comenzar juego
struct PacketStartGame {
	Uint8 num_jornada;
};

// Paquete para ingrediente
struct PacketIngrediente {
	Uint8 tipo_ingrediente;
	Sint16 posX;
	Sint16 posY;
	Uint8 velX;
	Uint8 velY;
};

// Paquete para un grupo de clientes
struct PacketGrupoCliente {
	Uint8 textCliente[8];
	Uint8 tamGrupo;
	Sint16 posPuertaX;
	Sint16 posPuertaY;
	Uint8 velX;
	Uint8 velY;

	Sint16 dirX;
	Sint16 dirY;

	float tolerancia;
};

struct PacketButtonBuffer {
	bool buttonBuffer[8];
};

struct PacketSyncPlayers {
	Sint16 posX;
	Sint16 posY;
};

struct Packet {
	Uint8 packet_type;

	union
	{
		PacketSend send;
		PacketRecv recieve;
		PacketAccept accept;
		PacketName name;
		PacketStartGame startGame;
		PacketIngrediente ingrediente;
		PacketGrupoCliente grupoCliente;
		PacketButtonBuffer buttonBuffer;
		PacketSyncPlayers syncPlayers;
	};

};


class NetworkManager
{
private:

	std::vector<TCPsocket> player_sockets;
	std::vector<IPaddress> player_ips;
	std::vector<int> player_ids;

	// SERVER

	std::thread* accept_t;
	std::thread* receiveplayers_t;
	std::thread* sendplayers_t;

	void acceptPlayers();
	void receivePlayers();
	void sendPlayers();

	// CLIENT

	int client_id;

	std::thread* updateclient_t;

	void updateClient();

	// UTILS

	int getClientID(const IPaddress& addr);

	bool compareAddress(const IPaddress& addr1, const IPaddress& addr2);

	// VARIABLES

	char nType;

	const int MAX_PLAYERS = 15;

	int id_count;
	Game* game_;

	bool exitThread;
	bool gameStarted;

	std::string myName;
	std::string otherName;

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
	Uint32 updateTime_ = 4000; //los segundos que tarda en actualizarse el reloj

public:
	NetworkManager(Game* game);
	~NetworkManager();

	bool init(char type, const char* ip_addr = nullptr, std::string name = "");
	void update();

	void close();

	Player* addPlayerHost();
	Player* addPlayerClient(int id);

	std::string getOtherName() { return otherName;}
	std::string getMyName() { return myName; }

	void startGameTimer();

	void sendStartGame(int numJornada);
	void sendCreateIngrediente(int tipoIngrediente, Vector2D<double> pos, Vector2D<double> vel);
	void sendCreateIngredienteLetal(int tipoIngrediente, Vector2D<double> pos, Vector2D<double> vel);

	void sendGrupoCliente(int tamGrupo, Vector2D<double> puertaPos, Vector2D<double> vel, Vector2D<double> distancia, std::vector<int>textureNumber, float tolerancia);
	
	void sendButtonsBuffer(std::vector<bool> keyPressed);

	void syncPlayers();
};