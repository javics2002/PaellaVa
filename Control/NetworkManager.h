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
	EPT_SYNCPICKOBJECT,
	EPT_SYNCDROPOBJECT,
	EPT_SYNCPEDIDO,
	EPT_SYNCMUEBLEROTO,
	EPT_SYNCPAUSE,
	EPT_SYNCCOMANDA,
	EPT_QUIT
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
	Uint16 ing_id;
	Sint16 posX;
	Sint16 posY;
	Uint8 velX;
	Uint8 velY;
};

// Paquete para un grupo de clientes
struct PacketGrupoCliente {
	Uint8 textCliente[8];
	Uint8 tamGrupo;
	
	Uint16 group_id;
	Uint16 door_id;

	Uint8 velX;
	Uint8 velY;

	Sint16 dirX;
	Sint16 dirY;

	float tolerancia;
};

// Paquete para input jugador (axis)
struct PacketButtonBuffer {
	bool buttonBuffer[4];
};

// Paquete para sincronizar a los jugadores
struct PacketSyncPlayers {
	Sint16 posX;
	Sint16 posY;

	float velX;
	float velY;

};

// currently testing
struct PacketSyncPickObject {
	Uint8 object_type; // 0 - ingredientes, 1 - clientes, 2 - paella, 3 - arroz 4 - herramienta
	Uint8 extra_info; // additional info
	Uint16 object_id; // id
	Sint16 mueble_id; // id mueble
};

struct PacketSyncDropObject {
	Uint8 object_type; // 0 - ingredientes, 1 - clientes, 2 - paella, 3 - arroz 4 - herramienta
	Uint16 object_id; // id

	Sint16 mueble_id; // mueble id
};

struct PacketSyncPedido {
	Uint8 group_id; // id del grupo de clientes

	Uint8 paella_number; // numero de paellas
	Uint8 paella_size[4];

	Uint8 ing_pedidos[12]; // 9 ingredientes
};

struct PacketSyncMuebleRoto {
	Uint8 mueble_id;
};

struct PacketSyncComanda {
	Uint8 numMesa;

	Uint8 paella_size[4];
	Uint8 ing_pedidos[12]; // 9 ingredientes
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
		PacketSyncPickObject syncPickObject;
		PacketSyncDropObject syncDropObject;
		PacketSyncPedido syncPedido;
		PacketSyncMuebleRoto syncMuebleRoto;
		PacketSyncComanda syncComanda;
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
	Game* game;

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
	Uint32 updateTime_ = 150; //los segundos que tarda en actualizarse el reloj

	// Host
	bool host;

public:
	NetworkManager(Game* game_);
	~NetworkManager();

	bool init(char type, const char* ip_addr = nullptr, std::string name = "");
	void update();

	void close();

	bool isHost() { return host; }
	Player* addPlayerHost();
	Player* addPlayerClient(int id);

	std::string getOtherName() { return otherName;}
	std::string getMyName() { return myName; }

	void startGameTimer();

	void sendStartGame(int numJornada);
	void sendCreateIngrediente(int tipoIngrediente, int ingId, Vector2D<double> pos, Vector2D<double> vel);
	void sendCreateIngredienteLetal(int tipoIngrediente, Vector2D<double> pos, Vector2D<double> vel);
	
	void sendGrupoCliente(int tamGrupo, int idPuerta, int idGrupoClientes, Vector2D<double> vel, Vector2D<double> distancia, std::vector<int> textureNumber, float tolerancia);
	void sendButtonsBuffer(std::vector<bool> keyPressed);

	void syncPlayers();
	void syncPickObject(int objectType, int objectId, int muebleId, int extraInfo);
	void syncDropObject(int objectType, int objectId, int muebleId);
	void syncPedido(int idGrupoCliente, int numPaellas, std::vector<int> tamPaella, std::vector<int> ingPedidos);
	void syncMuebleRoto(int muebleId);
	void syncPause();
	void syncComanda(int numMesa, std::vector<int> tamPaella, std::vector<int> ingPedidos);

	void setGameStarted(bool gameStarted_) { gameStarted = gameStarted_; }
};